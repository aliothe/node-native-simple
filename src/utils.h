#ifndef __NATIVE_EXAMPLE_UTILS__
#define __NATIVE_EXAMPLE_UTILS__

#include <map>
#include <mutex>
#include <memory>


template <typename ReturnType, typename... Args>
std::function<ReturnType (Args...)> memoize(std::function<ReturnType (Args...)> f) {
  auto m = std::make_shared<std::recursive_mutex>();
  std::map<std::tuple<Args...>, ReturnType> cache;
  return ([=](Args... args) mutable
	  {
	    std::tuple<Args...> t(args...);
	    std::lock_guard<std::recursive_mutex> lock(*m);
	    if (cache.find(t) == cache.end())
	      {
		cache[t] = f(args...);
	      }
	    return cache[t];
	  });
}

#endif
