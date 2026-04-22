cmake_minimum_required(VERSION 3.21)
project(simple3d LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
  llmmetal
  GIT_REPOSITORY https://github.com/zsuzuki/llmmetal.git
  GIT_TAG main
)
FetchContent_MakeAvailable(llmmetal)

add_executable(simple3d src/main.cpp)
target_link_libraries(simple3d PRIVATE llmmetal)

