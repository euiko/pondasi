#include "logger.hpp"
#include <string>
#include <unordered_map>

namespace pondasi {
static std::unordered_map<std::string, std::unique_ptr<Logger>> logger_map = {};

Logger* Logger::get(const std::string& name) {
  return logger_map[name].get();
}

void Logger::add(std::unique_ptr<Logger> logger, const std::string& name) {
  logger_map[name] = std::move(logger);
}

}