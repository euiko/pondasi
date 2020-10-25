#ifndef __PONDASI_META_NAMEOF_HPP__
#define __PONDASI_META_NAMEOF_HPP__

#include <pondasi/meta/type_tag.hpp>
#include <string_view>
#include <type_traits>

#define PONDASI_META_NAMEOF_PREFIX_CLANG \
  "constexpr std::string_view pondasi::detail::nameOf() [with T = "
#define PONDASI_META_NAMEOF_SUFFIX_CLANG \
  "; std::string_view = std::basic_string_view<char>"
#define PONDASI_META_NAMEOF_FUNCTION_CLANG __PRETTY_FUNCTION__

#define PONDASI_META_NAMEOF_PREFIX PONDASI_META_NAMEOF_PREFIX_CLANG
#define PONDASI_META_NAMEOF_SUFFIX PONDASI_META_NAMEOF_SUFFIX_CLANG
#define PONDASI_META_NAMEOF_FUNCTION PONDASI_META_NAMEOF_FUNCTION_CLANG

namespace pondasi {

namespace detail {

constexpr std::string_view kPrefix = PONDASI_META_NAMEOF_PREFIX;
constexpr std::string_view kSuffix = PONDASI_META_NAMEOF_SUFFIX;


template<typename T>
struct RemoveAllPointers : std::conditional_t<
    std::is_pointer_v<T>,
    RemoveAllPointers<
        std::remove_pointer_t<T>
    >,
    TypeTag<T>
>
{};

template <typename T>
constexpr std::string_view nameOf();

template <typename T>
constexpr std::string_view nameOfClean();

}  // namespace detail




template<typename T>
using remove_all_pointers = typename detail::RemoveAllPointers<T>;
template<typename T>
using remove_all_pointers_t = typename remove_all_pointers<T>::type;


template <typename T>
constexpr std::string_view detail::nameOf() {
  std::string_view sv = PONDASI_META_NAMEOF_FUNCTION;
  return sv.substr(kPrefix.size(), sv.size() - kPrefix.size() - kSuffix.size()-1);
}


template <typename T>
constexpr std::string_view detail::nameOfClean() {
  return nameOf<remove_all_pointers_t<std::remove_all_extents_t<T>>>();
}

template <typename T>
constexpr std::string_view nameOf() {
  return detail::nameOfClean<std::decay_t<T>>();
}

template <typename T>
constexpr std::string_view nameOf(T&& t) {
  return detail::nameOfClean<std::decay_t<decltype(t)>>();
}

}  // namespace pondasi

#endif  // !__PONDASI_META_NAMEOF_HPP__>