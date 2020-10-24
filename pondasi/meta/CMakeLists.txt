set(SOURCES
  reflect.cpp
)

add_library(meta ${SOURCES})

add_executable(meta_typeid_test typeid_test.cpp)