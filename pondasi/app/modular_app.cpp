#include "modular_app.hpp"

namespace pondasi {


std::vector<ModuleFactory>& StaticModule::factories() {
  static std::vector<ModuleFactory> factories;
  return factories;
}

ModularApp::ModularApp() {
  // copies static registered factories
  for(auto& m: StaticModule::factories()) {
    modules_factories_.push_back(m);
  }
}

ModularApp::~ModularApp() {
  stop(); 
}
bool ModularApp::start() {
  for(auto&& fn : modules_factories_) {
    modules_.emplace_back(fn());
    if(!modules_.back()->init()) {
      return false;
    }
  }
  return true;
}
bool ModularApp::stop() {
  for(auto it = modules_.rbegin(); it != modules_.rend(); it++) {
    auto& m = *it;
    m->close();
    modules_.pop_back(); 
  }
  return true;
}
void ModularApp::addModule(ModuleFactory&& factory) {
  modules_factories_.emplace_back(std::move(factory));
}
}