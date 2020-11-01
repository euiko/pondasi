#ifndef __PONDASI_META_METADATA_HPP__
#define __PONDASI_META_METADATA_HPP__

#include <pondasi/meta/typeid.hpp>
#include <functional>
#include <initializer_list>
#include <string_view>
#include <tuple>
#include <type_traits>
#include <unordered_map>

namespace pondasi {

template <typename T>
struct RetType {};

template <typename Ret, typename Class>
struct RetType<Ret Class::*> {
  using type = Ret;
};

template <typename Ret, typename Class>
struct RetType<Ret Class::*const> {
  using type = Ret;
};

template <typename Ret, typename Class, typename... Args>
struct RetType<Ret (Class::*)(Args...)> {
  using type = Ret;
};

template <typename Ret, typename Class, typename... Args>
struct RetType<Ret (Class::*)(Args...) const> {
  using type = Ret;
};

template <TypeId Id, typename T>
struct MetaInfo {};

template <typename T>
struct Meta {
  constexpr Meta(std::string_view name, T&& ptr,
                 std::initializer_list<std::string_view> attributes = {})
      : type_id(typeId<typename RetType<T>::type>()),
        type_name(nameOf<typename RetType<T>::type>()),
        name(name),
        attributes(attributes),
        ptr(std ::forward<T>(ptr)) {}
  MetaInfo<typeId<typename RetType<T>::type>(), T> type_info;
  TypeId type_id;
  std::string_view type_name;
  std::string_view name;
  std::initializer_list<std::string_view> attributes;
  T ptr;
};

class Metadata;

template <typename T, typename Tag, typename... Ms>
class MetadataBuilder {
 public:
  friend class Metadata;
  constexpr MetadataBuilder() = default;
  constexpr explicit MetadataBuilder(std::tuple<Meta<Ms>...> metadata)
      : metadata_(metadata) {}

  template <typename M>
  constexpr MetadataBuilder<T, Tag, M, Ms...> add(Meta<M> meta) {
    return MetadataBuilder<T, Tag, M, Ms...>(
        std::tuple_cat(std::tuple<Meta<M>>{meta}, metadata_));
  }

  template <typename M>
  constexpr MetadataBuilder<T, Tag, M, Ms...> add(
      std::string_view name, M&& ptr,
      std::initializer_list<std::string_view> attributes = {}) {
    return this->add(Meta(name, std::forward<M>(ptr), attributes));
  }
  [[nodiscard]] constexpr size_t size() const { return sizeof...(Ms); }
  [[nodiscard]] Metadata* build() const;

 private:
  std::tuple<Meta<Ms>...> metadata_;
};

struct Visitor {
  virtual ~Visitor() = default;
};

template <typename T, typename Tag>
struct MetadataVisitor;

class Metadata {
 public:
  Metadata() = default;
  template <typename Fn>
  explicit Metadata(Fn fn) {
    fn(this);
  }

  template <typename T, typename Tag, typename... Ms>
  void registerMeta(const Meta<Ms>&... ms) {
    visit_func_ = [=](Visitor& visitor) {
      using Visitor = typename MetadataVisitor<T, Tag>::type;
      (([&visitor](auto&& meta) {
         auto typed_handler = static_cast<Visitor&>(visitor);
         typed_handler.template visit<Ms>(meta);
       }(ms)),
       ...);
    };
  }

  void visit(Visitor& visitor) const { this->visit_func_(visitor); }

 private:
  std::function<void(Visitor&)> visit_func_;
};

template <typename T, typename Tag, typename... Ms>
Metadata* MetadataBuilder<T, Tag, Ms...>::build() const {
  static Metadata metadata([this](auto* metadata) {
    std::apply(
        [metadata](auto&&... args) {
          metadata->template registerMeta<T, Tag>(args...);
        },
        this->metadata_);
  });

  return &metadata;
}

template <typename T, typename Tag>
constexpr auto define_metadata() {
  return MetadataBuilder<T, Tag>();
}

namespace reflect {
  struct Tag {};
}

template <typename T, typename Tag>
Metadata* metadata() {
  constexpr auto kMetadata = define_metadata<T, Tag>();

  if constexpr (kMetadata.size() == 0 && !std::is_same_v<Tag, reflect::Tag>) {
    return metadata<T, reflect::Tag>();
  } else if (kMetadata.size() == 0) {
    return nullptr;
  }

  return kMetadata.build();
}

};  // namespace pondasi

#endif