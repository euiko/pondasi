#include "json.hpp"
#include <iostream>

struct MyStruct {
  int a;
  int b;
  int c;
  float d;
};

namespace pondasi {

template<>
constexpr auto define_metadata<MyStruct, json::Tag>() {
  return MetadataBuilder<MyStruct, json::Tag>()
        .add("a", &MyStruct::a)
        .add("c", &MyStruct::c)
        .add("b", &MyStruct::b)
        .add("floating", &MyStruct::d);
}

}


int main() {
  
  MyStruct s;
  s.a = 10;
  s.b = 20;
  s.d = 100;

  auto json_str = pondasi::json::marshal(s);
  std::cout << json_str << std::endl;
}