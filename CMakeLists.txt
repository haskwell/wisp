cmake_minimum_required(VERSION 3.15)
project(wisp LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Default to Debug if you don't pick a build type
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug)
endif()

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "src/*.cpp")
add_executable(wisp ${SOURCES})

target_include_directories(wisp PRIVATE src)
target_compile_options(wisp PRIVATE -Wall -Wextra)