#ifndef __PONDASI_LOGGER_LOGGER_HPP__
#define __PONDASI_LOGGER_LOGGER_HPP__

#include <iosfwd>
#include <memory>

namespace pondasi {

struct Logger {
  // static function
  static Logger* get(const std::string& name);
  static Logger* add(const std::string& name, std::unique_ptr<Logger> instance);

  // interface
  virtual ~Logger() = default;
  virtual void info(const char* message) = 0;
  virtual void error(const char* message) = 0;
  virtual void debug(const char* message) = 0;
  virtual void warning(const char* message) = 0;
  virtual void log(const char* message, int log_level) = 0;
};

}  // namespace pondasi

#endif