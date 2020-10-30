#ifndef __PONDASI_META_METADATA_HPP__
#define __PONDASI_META_METADATA_HPP__

#include <bits/stdint-uintn.h>
#include <pondasi/meta/typeid.hpp>
#include <functional>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <string_view>
#include <tuple>
#include <type_traits>
#include <unordered_map>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/prettywriter.h>

namespace pondasi {

template<typename T>
struct RetType {};

template<typename Ret, typename Class>
struct RetType<Ret Class::*> {
  using type = Ret;
};

template<typename Ret, typename Class>
struct RetType<Ret Class::* const> {
  using type = Ret;
};

template<typename Ret, typename Class, typename ...Args>
struct RetType<Ret (Class::*)(Args...)> {
  using type = Ret;
};

template<typename Ret, typename Class, typename ...Args>
struct RetType<Ret (Class::*)(Args...) const> {
  using type = Ret;
};

template <TypeId Id, typename T>
struct MetaInfo {};

template <typename T>
struct Meta {
  constexpr Meta(std::string_view name, T&& ptr)
      : type_id(typeId<typename RetType<T>::type>()),
        type_name(nameOf<typename RetType<T>::type>()),
        name(name),
        ptr(std ::forward<T>(ptr)) {}
  MetaInfo<typeId<typename RetType<T>::type>(), T> type_info;
  TypeId type_id;
  std::string_view type_name;
  std::string_view name;
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
  [[nodiscard]] Metadata* build() const;

 private:
  std::tuple<Meta<Ms>...> metadata_;
};

struct IValue {
  virtual ~IValue() = default;
};
template<typename T>
class ValueWrapper {
public:
  explicit ValueWrapper(T& val) : value_(val) {}
  T& get() {
    return value_;
  }
private:
  T& value_;
};

struct IMetadataHandler {
 virtual ~IMetadataHandler() = default;
};

template<typename T, typename Tag>
struct MetadataHandler {
  static_assert(std::is_base_of_v<IMetadataHandler, T>, "metadata handler must be derived from IMetadataHandler");
  using type = T;
};

class Metadata {
 public:

  using callback_t = std::function<IMetadataHandler*()>;

  template <typename T, typename Tag, typename... Ms>
  void registerMeta(const Meta<Ms>&... ms) {
    visit_func_ = [=](callback_t&& callback) {
      using Handler = typename MetadataHandler<T, Tag>::type;
      std::unique_ptr<IMetadataHandler> handler{callback()};
      (([handler = handler.get()](auto&& meta) {
        auto* typed_handler = static_cast<Handler*>(handler);

        typed_handler->template handle<Ms>(meta);
       }(ms)),
       ...);
    };
  }

  void visit(callback_t&& callback) {
    this->visit_func_(std::move(callback));
  }

 private:

  template<typename T, typename Tag, typename MetaType>
  void handle(const Meta<MetaType>& meta) {
    MetadataHandler<T, Tag>{}.visit(meta);
  }
  std::function<void(callback_t&&)> visit_func_;
};

template <typename T, typename Tag, typename... Ms>
Metadata* MetadataBuilder<T, Tag, Ms...>::build() const {
  auto* metadata = new Metadata;
  std::apply([metadata](auto&&... args) { metadata->registerMeta<T, Tag>(args...); },
             metadata_);
  return metadata;
}
template<typename T, typename MetaType>
void visit(const Meta<MetaType>& meta) {
  std::cout << meta.name << "(" << meta.type_id << ", " << meta.type_name << "), ";
}

template <typename T, typename Tag>
struct WithMetadata {
  static Metadata* metadata() { return nullptr; }
};


struct JsonTag {};
class JsonSerializer;
using Json = rapidjson::PrettyWriter<rapidjson::StringBuffer>;

template<typename T>
struct JsonHandler : public IMetadataHandler {
  JsonSerializer* serializer;
  Json* json;
  T* value;

  template<typename MetaType>
  void handle(const Meta<MetaType>& meta);
};

template<typename T>
struct MetadataHandler<T, JsonTag> {
  using type = JsonHandler<T>;
};

struct JsonSerializer {

  template <typename T>
  std::string serialize(const T& t) {
    using namespace rapidjson;

    std::unique_ptr<Metadata> metadata{WithMetadata<T, JsonTag>::metadata()};

    StringBuffer sb;
    PrettyWriter<StringBuffer> json(sb);
    json.StartObject();
    metadata->visit([this, &t, &json](){
      auto* handler = new JsonHandler<std::add_const_t<T> >{};
      handler->serializer = this;
      handler->value = &t;
      handler->json = &json;
      return handler;
    });
    json.EndObject();

    return sb.GetString();
  }

  void handle(Json& json, std::string_view name, const int& a) {
    json.String({name.data(), name.size()});
    json.Int(a);
  }
  template <typename T>
  void handle(Json& json, std::string_view name, const T& t) {
    std::stringstream ss;
    ss << t;
    json.String({name.data(), name.size()});
    json.String(ss.str());

  }
};


template<typename T>
template<typename MetaType>
void JsonHandler<T>::handle(const Meta<MetaType>& meta){
  this->serializer->handle(*json, meta.name, (*value).*meta.ptr);
}


};  // namespace pondasi

#endif