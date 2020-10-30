option(LOGGER_ENABLE_SPDLOG "Enable spdlog logger" ON)

set(SOURCES
  logger.cpp
)

add_library(logger ${SOURCES})
set(DEPENDENCIES)

if(LOGGER_ENABLE_SPDLOG)
  find_package(spdlog REQUIRED)
  add_library(logger_spdlog STATIC spdlog_logger.cpp)
  target_link_libraries(logger_spdlog PUBLIC spdlog::spdlog)
  set(DEPENDENCIES logger_spdlog)
endif()


target_link_libraries(logger PUBLIC ${DEPENDENCIES})