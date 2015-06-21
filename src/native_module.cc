#include <node.h>
#include "native.h"

using namespace v8;

void CreateObject(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = Isolate::GetCurrent();
  HandleScope scope(isolate);
  Native::NewInstance(args);
}

void InitAll(Handle<Object> exports, Handle<Object> module) {
  Native::Init();
  NODE_SET_METHOD(module, "exports", CreateObject);
}

NODE_MODULE(addon, InitAll)



