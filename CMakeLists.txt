cmake_minimum_required(VERSION 3.14)
project(chess)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(RAYLIB_VERSION 5.0)

include(FetchContent)

FetchContent_Declare(
  raylib
  DOWNLOAD_EXTRACT_TIMESTAMP OFF
  URL https://github.com/raysan5/raylib/archive/refs/tags/${RAYLIB_VERSION}.tar.gz
)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest)
FetchContent_MakeAvailable(raylib)

set(SOURCES 
    src/main.cpp
    src/movement.cpp)

set(HEADERS
    include/movement.h)

#for raylib
add_executable(
  chess
  ${SOURCES}
  ${HEADERS}
)
target_include_directories(chess PRIVATE include)
target_link_libraries(
  chess
  raylib
)

enable_testing()

add_executable(
  testing
  tests/test.cpp
)
target_link_libraries(
  testing
  GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(testing)