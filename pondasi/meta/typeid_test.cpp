#include "typeid.hpp"
#include <cassert>
#include <iostream>

int main() {
  auto t1 = pondasi::typeId<int>();
  auto t2 = pondasi::typeId<float>();
  std::cout << "typeid of int = " << t1 << std::endl;
  std::cout << "typeid of float = " << t2 << std::endl;
  assert(t1 != t2);

  using myint = int;
  auto t3 = pondasi::typeId<myint>();
  assert(t1 == t3);
  std::cout << "typeid of myint = " << t3 << std::endl;

  struct A {

  };
  auto t4 = pondasi::typeId<A>();
  std::cout << "typeid of A = " << t4 << std::endl;
  
  return 0;
}