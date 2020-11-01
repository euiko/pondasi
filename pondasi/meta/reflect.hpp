#ifndef __PONDASI_META_REFLECT_HPP__
#define __PONDASI_META_REFLECT_HPP__

#include <pondasi/meta/metadata.hpp>

namespace pondasi {

namespace reflect {

// defined on metadata.hpp
// struct Tag {};

template <typename T>
struct Visitor;

}  // namespace reflect
template <typename T>
struct MetadataVisitor<T, reflect::Tag> {
  using type = reflect::Visitor<T>;
};

namespace reflect {

template <typename T>
struct Visitor : public ::pondasi::Visitor {
  template <typename MetaType>
  void visit(const Meta<MetaType>& meta) {}
};
}  // namespace reflect

}  // namespace pondasi

#endif