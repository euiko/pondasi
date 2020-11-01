set(SOURCES
  json.cpp
)


find_package(RapidJSON CONFIG REQUIRED)

add_library(encoding ${SOURCES})
target_link_libraries(encoding PUBLIC meta)
add_compile_definitions(RAPIDJSON_HAS_STDSTRING)
target_compile_definitions(encoding PUBLIC )
target_include_directories(meta PUBLIC ${RAPIDJSON_INCLUDE_DIRS})


add_executable(encoding_json_test json_test.cpp)
target_link_libraries(encoding_json_test PRIVATE encoding)
