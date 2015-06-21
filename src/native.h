#ifndef __NODE_NATIVE_EXAMPLE__
#define __NODE_NATIVE_EXAMPLE__

#include <functional>
#include <node.h>
#include <node_object_wrap.h>
#include <uv.h>

class Native : public node::ObjectWrap {
 public:
  static void Init();
  static void NewInstance(const v8::FunctionCallbackInfo<v8::Value>& args);
  
 private:
  Native();
  ~Native();

  static v8::Persistent<v8::Function> constructor;
  static void New(const v8::FunctionCallbackInfo<v8::Value>& args);
  
  // async
  static void Fib(const v8::FunctionCallbackInfo<v8::Value>& args);
  static void UV_Fib(uv_work_t * req);
  static void UV_FibAfter(uv_work_t * req);

  // sync
  static void FibSync(const v8::FunctionCallbackInfo<v8::Value>& args);

 private:
  double  FibImpl(double  n);
 private:
  std::function<double (double)> f;
};
#endif
