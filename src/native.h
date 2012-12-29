#ifndef __NODE_NATIVE_EXAMPLE__
#define __NODE_NATIVE_EXAMPLE__

#include <functional>
#include <node.h>
#include <v8.h>

namespace mcreutz{
namespace examples{
          class Native : public node::ObjectWrap {
          public:
            static void Init();
            static v8::Handle<v8::Value> NewInstance(const v8::Arguments& args);
  
          private:
            Native();
            ~Native();

            static v8::Persistent<v8::Function> constructor;
            static v8::Handle<v8::Value> New(const v8::Arguments& args);

	    // async
	    static v8::Handle<v8::Value> Fib(const v8::Arguments& args);
            static void UV_Fib(uv_work_t * req);
            static void UV_FibAfter(uv_work_t * req);

            // sync
            static v8::Handle<v8::Value> FibSync(const v8::Arguments& args);

	  private:
	    double  FibImpl(double  n);
	  private:
	    std::function<double (double)> f;
          };
}}
#endif
