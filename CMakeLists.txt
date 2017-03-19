# ¯\_(ツ)_/¯ vim: et ts=2 sw=2

cmake_minimum_required(VERSION 3.6)
project(cli VERSION 0.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include_directories(
  ext/rxcpp/Rx/v2/src/
  )

set(SOURCE_FILES src/main.cpp)
add_executable(cli ${SOURCE_FILES})
target_link_libraries(cli ${CAF_LIBRARIES})
