#ifndef __YF_TYPE_TRAITS__
#define __YF_TYPE_TRAITS__

#include <type_traits>

namespace yf {
template <typename Func, typename... Args>
struct is_result_void {
  constexpr static bool value = std::is_void<decltype(std::declval<Func>()(
      std::declval<Args>()...))>::value;
};

template <typename T, typename U, typename... Args>
struct is_same_byte_size;

template <typename T, typename U>
struct is_same_byte_size<T, U> {
  constexpr static bool value = !(sizeof(T) ^ sizeof(U));
};

template <typename T, typename U, typename... Args>
struct is_same_byte_size {
  constexpr static bool value = is_same_byte_size<T, U>::value
                                    ? true
                                    : is_same_byte_size<T, Args...>::value;
};
}  // namespace yf

#endif
