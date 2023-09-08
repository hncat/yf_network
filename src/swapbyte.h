#ifndef __YF_BYTESWAP_H__
#define __YF_BYTESWAP_H__

#include <byteswap.h>
#include <stdint.h>

#include "type_traits.h"

/**
 * @brief 字节序相关
 */
namespace yf {
#if __BYTE_ORDER == __LITTLE_ENDIAN
#define YF_LITTLE_ENDIAN
#elif __BYTE_ORDER == __BIG_ENDIAN
#define YF_BIG_ENDIAN
#else
#error "byte order of location"
#endif

template <typename T>
inline std::enable_if_t<sizeof(T) == sizeof(int16_t), T> byteswap(T v) {
  return bswap_16(v);
}

template <typename T>
inline std::enable_if_t<sizeof(T) == sizeof(int32_t), T> byteswap(T v) {
  return bswap_32(v);
}

template <typename T>
inline std::enable_if_t<sizeof(T) == sizeof(int64_t), T> byteswap(T v) {
  return bswap_64(v);
}

template <typename T>
using is_same_byte_order = is_same_byte_size<T, int16_t, int32_t, int64_t>;

template <typename T>
constexpr static bool is_same_byte_order_v = is_same_byte_order<T>::value;

#ifdef YF_LITTLE_ENDIAN
template <typename T>
inline std::enable_if_t<is_same_byte_order_v<T>, T> big_endian(T v) {
  return byteswap(v);
}

template <typename T>
inline std::enable_if_t<is_same_byte_order_v<T>, T> little_endian(T v) {
  return v;
}
#elif YF_BIG_ENDIAN
inline std::enable_if_t<is_same_byte_order_v<T>, T> big_endian(T v) {
  return v;
}

template <typename T>
inline std::enable_if_t<is_same_byte_order_v<T>, T> little_endian(T v) {
  return byteswap(v);
}
#endif
}  // namespace yf

#endif