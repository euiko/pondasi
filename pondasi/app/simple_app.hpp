#ifndef __PONDASI_APP_SIMPLE_APP_HPP__
#define __PONDASI_APP_SIMPLE_APP_HPP__

#include <functional>
#include <pondasi/app/app.hpp>

namespace pondasi {
class SimpleApp : public App {
public:
  template<typename Fn>
  inline explicit SimpleApp(Fn run_fn) : run_fn_(run_fn) {}

  bool run() override;
  bool stop() override;
private:
  std::function<bool()> run_fn_;
};
}  // namespace pondasi

#endif