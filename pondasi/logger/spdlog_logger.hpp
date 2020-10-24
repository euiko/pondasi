#ifndef __PONDASI_LOGGER_SPDLOG_LOGGER_HPP__
#define __PONDASI_LOGGER_SPDLOG_LOGGER_HPP__

#include <pondasi/logger/logger.hpp>

namespace pondasi {

class SpdlogLogger : public Logger {
public:
  void debug(const char *message) override;
  void error(const char *message) override;
  void info(const char *message) override;
  void warning(const char *message) override;
  void log(const char *message, int log_level) override;
};

}

#endif