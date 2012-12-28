#include <node.h>
#include <v8.h>
#include "native.h"

using namespace v8;

Handle<Value> Create(const Arguments& args) 
{
  HandleScope scope;
  return scope.Close(mcreutz::examples::Native::NewInstance(args));
}

void Init(Handle<Object> target) 
{
  mcreutz::examples::Native::Init();
  target->Set(String::NewSymbol("create"),
	      FunctionTemplate::New(Create)->GetFunction());
}

NODE_MODULE(native_module, Init)
