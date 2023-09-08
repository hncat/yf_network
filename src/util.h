#ifndef __YF_UTIL_H__
#define __YF_UTIL_H__

namespace yf {
#define likely(x) __builtin_expect(!!(x), 1)
#define unlikely(x) __builtin_expect(!!(x), 0)
}  // namespace yf

#endif