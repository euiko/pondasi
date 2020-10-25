#ifndef __PONDASI_META_TYPEID_HPP__
#define __PONDASI_META_TYPEID_HPP__

#include <pondasi/meta/nameof.hpp>
#include <string_view>

namespace pondasi {

using TypeId = uint64_t;

template <typename T>
constexpr TypeId typeId();

namespace detail {
// using constexpr FNV-1a stringhash
// reference https://simoncoenen.com/blog/programming/StaticReflection.html
constexpr size_t kValConst{0xcbf29ce484222325};
constexpr size_t kPrimeConst{0x100000001b3};
constexpr size_t constantStrHash(size_t size, const char* const str,
                                 const size_t value = kValConst) noexcept {
  return (size == 0) ? value
                    : constantStrHash(size - 1, &str[1],
                                      (value ^ size_t(str[0])) * kPrimeConst);
}

constexpr TypeId getTypeId(std::string_view name) {
  return detail::constantStrHash(name.length(), name.data());
}


}  // namespace detail

template <typename T>
inline constexpr TypeId typeId() {
  constexpr TypeId kId = detail::getTypeId(nameOf<T>());
  return kId;
}

}  // namespace pondasi

#endif  // !__PONDASI_META_TYPEID_HPP__