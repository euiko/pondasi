#ifndef __PONDASI_APP_MODULAR_APP_HPP__
#define __PONDASI_APP_MODULAR_APP_HPP__

#include <pondasi/app/app.hpp>
#include <functional>
#include <vector>

namespace pondasi {

class Module;
class ModularApp;
using ModuleFactory = std::function<Module*()>;

struct Module {
  virtual ~Module() = default;
  virtual bool init() = 0;
  virtual void close() = 0;
};

class StaticModule : public Module {
// protect being misused outside
protected:
  friend class ModularApp;
  static std::vector<ModuleFactory>& factories();
};

class ModularApp : public App {
 public:
  ModularApp();
  ~ModularApp() override;

  void addModule(ModuleFactory&& factory);
  bool start() override;
  bool stop() override;

 private:
  std::vector<std::unique_ptr<Module>> modules_;
  std::vector<ModuleFactory> modules_factories_;
};

template <typename T>
struct ModuleRegistrator {
  static Module* factory();
};

template<typename T>
class SystemModule : public StaticModule {
private:
  struct Registrator {
    inline Registrator() {
      StaticModule::factories().emplace_back(ModuleRegistrator<T>::factory);
    }
  };

  static Registrator registrator_;
  virtual Registrator* touch() {
    return &registrator_;
  }
};

template<typename T>
typename SystemModule<T>::Registrator SystemModule<T>::registrator_;

}  // namespace pondasi

#endif