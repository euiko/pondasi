#ifndef __PONDASI_LOGGER_SPDLOG_LOGGER_HPP__
#define __PONDASI_LOGGER_SPDLOG_LOGGER_HPP__

#include <pondasi/logger/logger.hpp>
#include <spdlog/fwd.h>
#include <spdlog/common.h>
#include <memory>

namespace pondasi {

class SpdlogLogger : public Logger {
public:
  explicit SpdlogLogger(std::string name);
  void debug(const char *message) override;
  void critical(const char* message) override;
  void error(const char *message) override;
  void info(const char *message) override;
  void trace(const char* message) override;
  void warning(const char *message) override;
  void log(const char *message, int level) override;
  void log(const char *message, Level level) override;
private:

  std::unique_ptr<spdlog::logger> logger_;
};

}

#endif