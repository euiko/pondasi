#ifndef __PONDASI_APP_APP_HPP__
#define __PONDASI_APP_APP_HPP__

namespace pondasi {
struct App {
  virtual ~App() = default;
  virtual bool run() = 0;
  virtual bool stop() = 0;
};
}  // namespace pondasi

#endif