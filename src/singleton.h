#ifndef __YF_SINGLETON_H__
#define __YF_SINGLETON_H__

#include <utility>

namespace yf {
#define SINGLETON_FRIEND_CLASS(type) friend class singleton<type>;
template <typename T>
class singleton {
 public:
  typedef T value_type;
  typedef singleton<T> type;

 public:
  template <typename... Args>
  static T *instance(Args &&...args) {
    static T t{std::forward<Args>(args)...};
    return &t;
  }

 private:
  singleton() = delete;
  singleton(const singleton &) = delete;
  singleton &operator=(const singleton &) = delete;
  singleton(singleton &&) = delete;
  singleton &operator=(singleton &&) = delete;
};
}  // namespace yf

#endif