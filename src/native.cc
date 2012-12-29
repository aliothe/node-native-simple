#include <string>
#include <functional>
#include <cstddef>
#include <memory>
#include <limits>
#include <v8.h>
#include <node.h>
#include "utils.h"
#include "common.h"
#include "native.h"

using namespace v8;

namespace mcreutz{
namespace examples{

const char * const K_DOUBLE_OVERFLOW_ERROR = "overflow, result does not fit in double";
Persistent<Function> Native::constructor;

Native::Native()
{};
Native::~Native() {};

void Native::Init() 
{
  // Prepare constructor template
  Local<FunctionTemplate> tpl = FunctionTemplate::New(New);
  tpl->SetClassName(String::NewSymbol("Native"));
  tpl->InstanceTemplate()->SetInternalFieldCount(1);
  // Prototype
  tpl->PrototypeTemplate()->Set(String::NewSymbol("fibsync"),
                                FunctionTemplate::New(FibSync)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewSymbol("fib"),
                                FunctionTemplate::New(Fib)->GetFunction());
  constructor = Persistent<Function>::New(tpl->GetFunction());
}

double Native::FibImpl(double n)
{
  if  (n < 2) return n;
  return f(n-1) + f(n-2);
}
      
Handle<Value> Native::New(const Arguments& args) 
{
  HandleScope scope;
  Native * instance = new Native();
  instance->Wrap(args.This());

  std::function<double(double)> wrapped_fibimpl = 
  [=](double n) 
  {
    return instance->FibImpl(n);
  };
  instance->f = memoize(wrapped_fibimpl);
  return args.This();
}

Handle<Value> Native::NewInstance(const v8::Arguments& args) 
{
  HandleScope scope;
  const unsigned argc = 1;
  Handle<Value> argv[argc] = { args[0] };
  Local<Object> instance = constructor->NewInstance(argc, argv);
  return scope.Close(instance);
}

Handle<Value> Native::FibSync(const v8::Arguments& args)
{
  HandleScope scope;
  if (args.Length() < 1)
    {
      ThrowException(Exception::Error(String::New("wrong number of arguments - need a number")));
      return scope.Close(Undefined());
    }
  if (!args[0]->IsInt32())
    {
      ThrowException(Exception::TypeError(String::New("expecting an integer")));
      return scope.Close(Undefined());
    }

  Local<Number> number = Local<Number>::Cast(args[0]);
  Native * native = ObjectWrap::Unwrap<Native>(args.This());
  const double result = native->f(number->Value());
  if(result > std::numeric_limits<double>::max())
    {
      ThrowException(Exception::Error(String::New(K_DOUBLE_OVERFLOW_ERROR)));
      return scope.Close(Undefined());
    }
  return scope.Close(Number::New((native->f(number->Value()))));
}

Handle<Value> Native::Fib(const v8::Arguments& args)
{
  HandleScope scope;
  if (args.Length() != 2)
    {
      ThrowException(Exception::Error(String::New("wrong number of arguments - need a number and a function(err, result)")));
      return scope.Close(Undefined());
    }
  if (!args[0]->IsUint32())
    {
      ThrowException(Exception::TypeError(String::New("expecting an integer")));
      return scope.Close(Undefined());
    }
  if (!args[1]->IsFunction())
    {
      ThrowException(Exception::TypeError(String::New("second argument should be a function(err, result)")));
      return scope.Close(Undefined());
    }
    
  Native * native = ObjectWrap::Unwrap<Native>(args.This());
  std::unique_ptr<fib_baton> baton(new fib_baton());
  if (0 == baton.get())
    {
      ThrowException(Exception::TypeError(String::New("memory allocation error in Native::Fib")));
      return scope.Close(Undefined());
    }
  
  Local<Number> number = Local<Number>::Cast(args[0]);
  Local<Function> callback = Local<Function>::Cast(args[1]);
  
  baton->callback = Persistent<Function>::New(callback);
  baton->number = number->Value();
  baton->answer = 0;
  baton->native_obj = native;
  
  baton->req.data = reinterpret_cast<void*>(baton.get());
  fib_baton * baton_ptr = baton.release();
  uv_queue_work(uv_default_loop(), &(baton_ptr->req), Native::UV_Fib, Native::UV_FibAfter); 
  native->Ref();
  return scope.Close(Undefined());
}

// uv stuff
// note: runs on thread pool, no v8 hanky panky in here 
void Native::UV_Fib(uv_work_t * req)
{
      fib_baton * baton = reinterpret_cast<fib_baton*>(req->data);
      Native * native = baton->native_obj;
      baton->answer = native->f(baton->number);
      if(baton->answer > std::numeric_limits<double>::max())
	{
	  baton->error = K_DOUBLE_OVERFLOW_ERROR;
	}
}

// back on main thread - feel free to use v8 stuff
void Native::UV_FibAfter(uv_work_t * req)
{
  HandleScope scope;
  std::unique_ptr<fib_baton> baton(reinterpret_cast<fib_baton*>(req->data));
  Native * native = baton->native_obj;
  Handle<Value> argv[2];
  if (false == baton->error.empty())
    {
      argv[0] = String::New(baton->error.c_str());
      argv[1] = Null();
    }
  else
    {
      argv[0] = Null();
      argv[1] = Number::New(baton->answer);
    }
  TryCatch try_catch; 
  baton->callback->Call(Context::GetCurrent()->Global(), 2, argv);

  // cleanup
  baton->callback.Dispose();
  native->Unref();

  if (try_catch.HasCaught())
    {
      node::FatalException(try_catch);
    }
}

}}
