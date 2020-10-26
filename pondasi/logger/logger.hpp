#ifndef __PONDASI_LOGGER_LOGGER_HPP__
#define __PONDASI_LOGGER_LOGGER_HPP__

#include <string>
#include <memory>

namespace pondasi {

struct Logger {
  enum class Level {
    kTrace,
    kDebug,
    kInfo,
    kWarning,
    kError,
    kCritical,
  };

  static constexpr const char* kDefaultName = "default";
  // static function
  static Logger* get(const std::string& name = kDefaultName);
  static void add(std::unique_ptr<Logger> instance, const std::string& name = kDefaultName);

  // interface
  virtual ~Logger() = default;
  virtual void critical(const char* message) = 0;
  virtual void error(const char* message) = 0;
  virtual void debug(const char* message) = 0;
  virtual void warning(const char* message) = 0;
  virtual void info(const char* message) = 0;
  virtual void trace(const char* message) = 0;
  virtual void log(const char* message, int level) = 0;
  virtual void log(const char* message, Level level) = 0;
};

}  // namespace pondasi

#endif