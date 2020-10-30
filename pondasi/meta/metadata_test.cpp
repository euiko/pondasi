#include "metadata.hpp"
#include <memory>

struct MyTag {};
struct MyStruct {
  int a;
  int b;
  int c;
};

template <>
struct pondasi::WithMetadata<MyStruct, pondasi::JsonTag> {
  static Metadata* metadata() {
    static constexpr auto kMetadata = MetadataBuilder<MyStruct, pondasi::JsonTag>()
        .add(Meta("a", &MyStruct::a))
        .add(Meta("c", &MyStruct::c))
        .add(Meta("b", &MyStruct::b));

    return kMetadata.build();
  };
};



int main() {
  MyStruct s;
  s.a = 10;
  s.b = 20;

  auto json_str = pondasi::JsonSerializer().serialize(s);
  std::cout << json_str << std::endl;

  return 0;
}