cmake_minimum_required(VERSION 3.12.0)

if(DEFINED ENV{VCPKG_ROOT} AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
  set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake"
      CACHE STRING "")
endif()

project(pondasi VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(DEFINED CMAKE_TOOLCHAIN_FILE)
  if(EXISTS CACHE{VCPKG_MANIFEST_FILE})
    vcpkg_acquire_dependencies()
  endif()
endif()

include(CTest)
enable_testing()

include_directories(${CMAKE_SOURCE_DIR})
add_compile_options(-fno-exceptions)

add_subdirectory(pondasi)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
