#include "app.hpp"
#include <unordered_map>

namespace pondasi {

static std::unordered_map<std::string, std::unique_ptr<App>> apps;

App* app(const std::string& name) {
  return apps[name].get();
}
App* app(std::unique_ptr<App> a, const std::string& name) {
  App* a_ptr = a.get();
  apps[name] = std::move(a);
  return a_ptr;
}

}