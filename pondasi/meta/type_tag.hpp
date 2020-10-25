#ifndef __PONDASI_TRAITS_TYPE_TAG_HPP__
#define __PONDASI_TRAITS_TYPE_TAG_HPP__

namespace pondasi {
template<typename T>
struct TypeTag {
  using type = T;
};
}

#endif