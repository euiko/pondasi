
set(HEADERS
  app.hpp
  modular_app.hpp
  simple_app.hpp
)

set(SOURCES
  app.cpp
  modular_app.cpp
  simple_app.cpp
)
add_library(app ${SOURCES})