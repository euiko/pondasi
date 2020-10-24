#include "logger.hpp"
#include <string>
#include <unordered_map>

namespace pondasi {
static std::unordered_map<std::string, std::unique_ptr<Logger>> logger_map = {};

Logger* Logger::get(const std::string& name) {
  return logger_map[name].get();
}

Logger* Logger::add(const std::string& name, std::unique_ptr<Logger> logger) {
  Logger* logger_ptr = logger.get();
  logger_map[name] = std::move(logger);
  return logger_ptr;
}

}