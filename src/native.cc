#include <string>
#include <functional>
#include <cstddef>
#include <memory>
#include <limits>
#include <node.h>
#include "utils.h"
#include "common.h"
#include "native.h"

using namespace v8;

const char * const K_DOUBLE_OVERFLOW_ERROR = "overflow, result does not fit in double";
Persistent<Function> Native::constructor;

Native::Native()
{};
Native::~Native() {};

void Native::Init() 
{
  Isolate* isolate = Isolate::GetCurrent();

  // Prepare constructor template
  Local<FunctionTemplate> tpl = FunctionTemplate::New(isolate, New);
  tpl->SetClassName(String::NewFromUtf8(isolate, "Native"));
  tpl->InstanceTemplate()->SetInternalFieldCount(1);

  // Prototype
  NODE_SET_PROTOTYPE_METHOD(tpl, "fibsync", FibSync);
  NODE_SET_PROTOTYPE_METHOD(tpl, "fib", Fib);

  constructor.Reset(isolate, tpl->GetFunction());
}

double Native::FibImpl(double n)
{
  if  (n < 2) return n;
  return f(n-1) + f(n-2);
}
      
void Native::New(const v8::FunctionCallbackInfo<v8::Value>& args) 
{
  Isolate* isolate = Isolate::GetCurrent();
  HandleScope scope(isolate);

  if (args.IsConstructCall())
  {
    Native * instance = new Native();
    instance->Wrap(args.This());
    std::function<double(double)> wrapped_fibimpl = 
      [=](double n) 
      {
	return instance->FibImpl(n);
      };
    instance->f = memoize(wrapped_fibimpl);  
    args.GetReturnValue().Set(args.This());
  }
  else
  {
    // Invoked as plain function `Native(...)`, turn into construct call.
    NewInstance(args);
  }
}

void Native::NewInstance(const FunctionCallbackInfo<v8::Value>& args)
{
  Isolate* isolate = Isolate::GetCurrent();
  HandleScope scope(isolate);

  const unsigned argc = 1;
  Handle<Value> argv[argc] = { args[0] };
  Local<Function> cons = Local<Function>::New(isolate, constructor);
  Local<Object> instance = cons->NewInstance(argc, argv);

  args.GetReturnValue().Set(instance);
}

void Native::FibSync(const v8::FunctionCallbackInfo<v8::Value>& args)
{
  Isolate* isolate = Isolate::GetCurrent();
  v8::EscapableHandleScope scope(isolate);
  if (args.Length() < 1)
    {
      isolate->ThrowException(Exception::Error(String::NewFromUtf8(isolate,"wrong number of arguments - need a number")));
      return;
    }
  if (!args[0]->IsInt32())
    {
      isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate,"expecting an integer")));
      return;
    }

  Local<Number> number = Local<Number>::Cast(args[0]);
  Native * native = ObjectWrap::Unwrap<Native>(args.This());
  const double result = native->f(number->Value());
  if(result > std::numeric_limits<double>::max())
    {
      isolate->ThrowException(Exception::Error(String::NewFromUtf8(isolate,K_DOUBLE_OVERFLOW_ERROR)));
      return;
    }
  args.GetReturnValue().Set(Number::New(isolate,(native->f(number->Value()))));
}

void Native::Fib(const v8::FunctionCallbackInfo<v8::Value>& args)
{
  Isolate* isolate = Isolate::GetCurrent();
  HandleScope scope(isolate);
  if (args.Length() != 2)
    {
      isolate->ThrowException(Exception::Error(String::NewFromUtf8(isolate,"wrong number of arguments - need a number and a function(err, result)")));
      return;
    }
  if (!args[0]->IsUint32())
    {
      isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate,"expecting an integer")));
      return;
    }
  if (!args[1]->IsFunction())
    {
      isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate,"second argument should be a function(err, result)")));
      return;
    }
    
  Native * native = ObjectWrap::Unwrap<Native>(args.This());
  std::unique_ptr<fib_baton> baton(new fib_baton());
  if (0 == baton.get())
    {
      isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate,"memory allocation error in Native::Fib")));
      return;
    }
  
  Local<Number> number = Local<Number>::Cast(args[0]);
  Local<Function> callback = Local<Function>::Cast(args[1]);

  Persistent<Function> fn(isolate, callback);
  baton->callback.Reset(isolate, fn);
  baton->number = number->Value();
  baton->answer = 0;
  baton->native_obj = native;
  
  baton->req.data = reinterpret_cast<void*>(baton.get());
  fib_baton * baton_ptr = baton.release();

  uv_queue_work(uv_default_loop(), &(baton_ptr->req), Native::UV_Fib, (uv_after_work_cb)Native::UV_FibAfter); 
  native->Ref();
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
  Isolate* isolate = Isolate::GetCurrent();
  HandleScope scope(isolate);

  std::unique_ptr<fib_baton> baton(reinterpret_cast<fib_baton*>(req->data));
  Native * native = baton->native_obj;
  Handle<Value> argv[2];
  if (false == baton->error.empty())
    {
      argv[0] = Exception::Error(String::NewFromUtf8(isolate,baton->error.c_str()));
      argv[1] = Null(isolate);
    }
  else
    {
      argv[0] = Null(isolate);
      argv[1] = Number::New(isolate,baton->answer);
    }
  TryCatch try_catch;
  Local<Function> callback = Local<Function>::New(isolate,baton->callback);
  callback->Call(isolate->GetCurrentContext()->Global(), 2, argv);

  // cleanup
  baton->callback.Reset();
  //callback->Dispose()
  native->Unref();

  if (try_catch.HasCaught())
    {
      node::FatalException(try_catch);
    }
}

