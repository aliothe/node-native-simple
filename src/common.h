#ifndef __NATIVE_EXAMPLE_COMMON__
#define __NATIVE_EXAMPLE_COMMON__

#include <node.h>
#include <string>
#include <uv.h>

  class Native;

  struct fib_baton {
    uv_work_t req;
    v8::Persistent<v8::Function> callback;
    double number;
    double answer;    
    std::string error;
    Native * native_obj;
  };

#endif
