# ¯\_(ツ)_/¯ vim: et ts=2 sw=2

cmake_minimum_required(VERSION 3.6)
project(cli VERSION 0.0.0 LANGUAGES CXX)

list( APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR} )
message( "cmake module path: ${CMAKE_MODULE_PATH}" )
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include_directories(
  rxcpp/Rx/v2/src/
  )

# add_subdirectory(caf)
find_package(CAF REQUIRED MODULE)

set(SOURCE_FILES src/main.cpp)
add_executable(cli ${SOURCE_FILES})
