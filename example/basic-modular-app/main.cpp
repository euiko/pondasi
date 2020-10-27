#include <pondasi/app/modular_app.hpp>
#include <iostream>

struct MyModule : public pondasi::SystemModule<MyModule> {
  bool init() override {
    std::cout << "init " << std::endl;
    return true;
  }
  void close() override {
    std::cout << "closing" << std::endl;
  }
};

template<>
struct pondasi::ModuleRegistrator<MyModule> {
  static Module* factory() {
    return new MyModule;
  }
};

int main() {
  using pondasi::app;

  auto* a = app(std::make_unique<pondasi::ModularApp>());
  a->start();

  return 0;
}