#ifndef __YF_NONCOPYABLE_H__
#define __YF_NONCOPYABLE_H__

namespace yf {
class noncopyable {
 protected:
  noncopyable() = default;
  virtual ~noncopyable() = default;

 private:
  noncopyable(const noncopyable &) = delete;
  noncopyable &operator=(const noncopyable &) = delete;
};
}  // namespace yf

#endif