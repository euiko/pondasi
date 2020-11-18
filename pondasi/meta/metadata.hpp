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
struct MethodType {};

template <typename Ret, typename Class>
struct MethodType<Ret Class::*> {
  using ret_type = Ret;
  using class_type = Class;
  static constexpr bool kIsConst = false;
};

template <typename Ret, typename Class>
struct MethodType<Ret Class::*const> {
  using ret_type = Ret;
  using class_type = Class;
  static constexpr bool kIsConst = true;
};

template <typename Ret, typename Class, typename... Args>
struct MethodType<Ret (Class::*)(Args...)> {
  using ret_type = Ret;
  using class_type = Class;
  using arg_types = std::tuple<Args...>;
  static constexpr bool kIsConst = false;
};

template <typename Ret, typename Class, typename... Args>
struct MethodType<Ret (Class::*)(Args...) const> {
  using ret_type = Ret;
  using class_type = Class;
  using arg_types = std::tuple<Args...>;
  static constexpr bool kIsConst = true;
};

template <TypeId Id, typename T>
struct MetaInfo {};

template <typename T>
struct Meta {
  constexpr Meta(std::string_view name, T&& ptr,
                 std::initializer_list<std::string_view> attributes = {})
      : type_id(typeId<typename MethodType<T>::ret_type>()),
        type_name(nameOf<typename MethodType<T>::ret_type>()),
        name(name),
        attributes(attributes),
        ptr(std ::forward<T>(ptr)) {}
  MetaInfo<typeId<typename MethodType<T>::ret_type>(), T> type_info;
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

template <typename T, typename Tag>
struct ADLMetadata {
  static constexpr auto metadata() { return MetadataBuilder<T, Tag>(); }
};


namespace reflect {
// define reflection Tag
struct Tag {};
// forward reflection Visitor type
template <typename T>
struct Visitor;
}  // namespace reflect

// specialize metadata visitor for reflect tag
template <typename T>
struct MetadataVisitor<T, reflect::Tag> {
  using type = reflect::Visitor<T>;
};


namespace detail {

template <typename T, typename Tag>
Metadata* metadata_from_adl_function() {
  // get metadata with the reqeusted tag
  constexpr auto kMetadata = define_metadata<T, Tag>();

  // recursive call when reqeusted metadata empty, fallback using reflection tag
  if constexpr (kMetadata.size() == 0 && !std::is_same_v<Tag, reflect::Tag>) {
    return metadata_from_adl_function<T, reflect::Tag>();
  } else if (kMetadata.size() == 0) {
    // still empty? return nullptr instead
    return nullptr;
  }

  // found? returns
  return kMetadata.build();
}

template <typename T, typename Tag, typename OrigTag = Tag>
Metadata* metadata_from_adl_class() {
  // get metadata with the reqeusted tag using ADLClass
  constexpr auto kMetadata = ADLMetadata<T, Tag>::metadata();

  // recursive call when reqeusted metadata empty, fallback using reflection tag
  if constexpr (kMetadata.size() == 0 && !std::is_same_v<Tag, reflect::Tag>) {
    return metadata_from_adl_class<T, reflect::Tag, OrigTag>();
  } else if (kMetadata.size() == 0) {
    // still empty? try find with specialized adl function
    return metadata_from_adl_function<T, OrigTag>();
  }

  // found? returns
  return kMetadata.build();
}

}  // namespace detail

template <typename T, typename Tag>
Metadata* metadata() {
  // try find with specialized adl class first
  return detail::metadata_from_adl_class<T, Tag>();
}

};  // namespace pondasi

#endif