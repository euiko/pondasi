set(SOURCES
  reflect.cpp
)

add_library(meta 
  ${SOURCES}
)
add_compile_definitions(RAPIDJSON_HAS_STDSTRING)

find_package(RapidJSON CONFIG REQUIRED)
target_include_directories(meta PUBLIC ${RAPIDJSON_INCLUDE_DIRS})


add_executable(meta_metadata_test metadata_test.cpp)
target_link_libraries(meta_metadata_test PRIVATE meta)

add_executable(meta_nameof_test nameof_test.cpp)
target_link_libraries(meta_nameof_test PRIVATE meta)

add_executable(meta_typeid_test typeid_test.cpp)
target_link_libraries(meta_typeid_test PRIVATE meta)