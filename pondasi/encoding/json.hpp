#ifndef __PONDASI_ENCODING_JSON_HPP__
#define __PONDASI_ENCODING_JSON_HPP__

#include <pondasi/meta/metadata.hpp>
#include <sstream>
#include <rapidjson/prettywriter.h>
#include <rapidjson/stringbuffer.h>

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
template <typename T>
inline void marshal(JsonWriter& writer, T&& t) {
  std::stringstream ss;
  ss << t;
  writer.String(ss.str());
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