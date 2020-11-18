#include "json.hpp"
#include <vector>
#include <iostream>
#include <unordered_map>
#include <memory>



struct MyStruct {
  int a;
  int b;
  int c;
  float d;
  std::vector<int> nums;
  std::unordered_map<std::string, std::shared_ptr<MyStruct>> nested;
};

template <>
constexpr auto pondasi::define_metadata<MyStruct, pondasi::json::Tag>() {
  return pondasi::MetadataBuilder<MyStruct, pondasi::json::Tag>()
      .add("a", &MyStruct::a)
      .add("c", &MyStruct::c)
      .add("b", &MyStruct::b)
      .add("floating", &MyStruct::d)
      .add("nums", &MyStruct::nums)
      .add("nested", &MyStruct::nested)
      ;
}

int main() {
  MyStruct s;
  s.a = 10;
  s.b = 20;
  s.d = 100;
  s.nums = {1, 2, 3, 4, 5};
  s.nested = {
    {"halo", std::make_unique<MyStruct>(s)}
  };

  std::cout << "is map: " << pondasi::json::detail::IsKeyValue<decltype(s.nested)>::value << std::endl;

  auto json_str = pondasi::json::marshal(s);
  std::cout << json_str << std::endl;
}