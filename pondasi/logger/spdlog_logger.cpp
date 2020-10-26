#include "spdlog_logger.hpp"
#include <spdlog/common.h>
#include <spdlog/logger.h>


namespace pondasi {

SpdlogLogger::SpdlogLogger(std::string name)
  : logger_(new spdlog::logger(std::move(name))) {
}
void SpdlogLogger::debug(const char *message) {
  logger_->debug(message);
}
void SpdlogLogger::critical(const char *message) {
  logger_->critical(message);
}
void SpdlogLogger::error(const char *message) {
  logger_->error(message);
}
void SpdlogLogger::info(const char *message) {
  logger_->info(message);
}
void SpdlogLogger::trace(const char *message) {
  logger_->trace(message);
}
void SpdlogLogger::warning(const char *message) {
  logger_->warn(message);
}
void SpdlogLogger::log(const char *message, Level level) {
  this->log(message, static_cast<int>(level));
}
void SpdlogLogger::log(const char *message, int level) {
  logger_->log(static_cast<spdlog::level::level_enum>(level), message);
}

}