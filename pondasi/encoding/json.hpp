#ifndef __PONDASI_ENCODING_JSON_HPP__
#define __PONDASI_ENCODING_JSON_HPP__

#include <pondasi/meta/metadata.hpp>
#include <rapidjson/prettywriter.h>
#include <rapidjson/stringbuffer.h>
#include <sstream>
#include <type_traits>

namespace pondasi {

// forward declaration and define tag
namespace json {
struct Tag {};
template <typename T>
struct JsonVisitor;
template <typename T>
std::string marshal(const T& t);

}  // namespace json

template <typename T>
struct MetadataVisitor<T, json::Tag> {
  using type = json::JsonVisitor<T>;
};

namespace json {

using JsonWriter = rapidjson::PrettyWriter<rapidjson::StringBuffer>;

template <typename T>
class JsonVisitor : public Visitor {
  enum Type {
    kUnmarshall,
    kMarshall,
  };

 public:
  explicit JsonVisitor(JsonWriter* writer, const T* value)
      : type_(kMarshall), writer_(writer), const_value_(value) {}

  template <typename MetaType>
  void visit(const Meta<MetaType>& meta);

 private:
  JsonWriter* writer_ = nullptr;
  const T* const_value_ = nullptr;

  T* value_ = nullptr;

  const Type type_;
};

namespace detail {
template <typename T>
inline std::string marshal(const T& t) {
  Metadata* metadata = ::pondasi::metadata<T, json::Tag>();
  assert(metadata != nullptr);

  rapidjson::StringBuffer sb;
  JsonWriter writer(sb);
  writer.StartObject();

  auto visitor = JsonVisitor(&writer, &t);
  metadata->visit(visitor);

  writer.EndObject();

  return sb.GetString();
}

void marshal(JsonWriter& writer, bool i);
void marshal(JsonWriter& writer, int i);
void marshal(JsonWriter& writer, int64_t i);
void marshal(JsonWriter& writer, uint i);
void marshal(JsonWriter& writer, uint64_t i);
void marshal(JsonWriter& writer, double i);
inline void marshal(JsonWriter& writer, float i) {
  marshal(writer, static_cast<double>(i));
}

template <typename T, typename = void>
struct IsIterable : std::false_type {};
template <typename T>
struct IsIterable<T, std::void_t<decltype(std::declval<T>().begin()),
                                 decltype(std::declval<T>().end())>>
    : std::true_type {};

template <typename T, typename = void>
struct IsKeyValue : std::false_type {};
template <typename T>
struct IsKeyValue<T, std::void_t<decltype(std::declval<T>().begin()),
                                 decltype(std::declval<T>().end()),
                                 decltype(std::declval<T>().find(""))>>
    : std::true_type {};
template <typename T, typename = void>
struct IsBoxed : std::false_type {};
template <typename T>
struct IsBoxed<T, std::void_t<decltype(std::declval<T>().get()),
                                 decltype(std::declval<T>().operator->())>>
    : std::true_type {};

template <typename T>
inline auto marshal(JsonWriter& writer, const T& t) {
  if constexpr (IsKeyValue<T>::value) {
    writer.StartObject();

    for (auto&& [k, v] : t) {
      writer.String(k);
      marshal(writer, v);
    }
    writer.EndObject();
  } else if constexpr (IsIterable<T>::value) {
    writer.StartArray();
    for (const auto& item : t) {
      marshal(writer, item);
    }
    writer.EndArray();
  } else if constexpr (IsBoxed<T>::value) {
    marshal(writer, *t.get());
  } else if constexpr (::pondasi::define_metadata<T, json::Tag>().size() != 0) {
    Metadata* metadata = ::pondasi::metadata<T, json::Tag>();

    writer.StartObject();
    auto visitor = JsonVisitor(&writer, &t);
    metadata->visit(visitor);
    writer.EndObject();
  } else {
    std::stringstream ss;
    ss << t;
    writer.String(ss.str());
  }
}
}  // namespace detail

template <typename T>
inline std::string marshal(const T& t) {
  return detail::marshal(t);
}

template <typename T>
template <typename MetaType>
void JsonVisitor<T>::visit(const Meta<MetaType>& meta) {
  switch (type_) {
    case kMarshall:
      assert(writer_ && const_value_);
      writer_->String({meta.name.data(), meta.name.size()});
      detail::marshal(*writer_, (*const_value_).*meta.ptr);
      break;
    case kUnmarshall:
      break;
  }
}
}  // namespace json
}  // namespace pondasi

#endif