#include "nameof.hpp"
#include <cassert>
#include <iostream>

int main() {
  std::cout << "name of int = " << pondasi::nameOf<int>() << std::endl;

  using myint = int;
  std::cout << "name of myint = " << pondasi::nameOf<myint>() << std::endl;


  std::cout << "name of myint* = " << pondasi::nameOf<myint***>() << std::endl;

  return 0;
}