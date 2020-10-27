#ifndef __PONDASI_APP_APP_HPP__
#define __PONDASI_APP_APP_HPP__

#include <memory>
#include <string>

namespace pondasi {
struct App {
  virtual ~App() = default;
  virtual bool start() = 0;
  virtual bool stop() = 0;
};

constexpr const char* kDefaultAppName = "default";
App* app(const std::string& name = kDefaultAppName);
App* app(std::unique_ptr<App> a, const std::string& name = kDefaultAppName);

}  // namespace pondasi

#endif