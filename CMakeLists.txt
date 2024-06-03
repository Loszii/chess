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
    src/helper.cpp
    src/movement.cpp
    src/engine.cpp)

set(HEADERS
    include/helper.h
    include/movement.h
    include/engine.h)


#to statically link
set(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
set(BUILD_SHARED_LIBS OFF)
set(CMAKE_EXE_LINKER_FLAGS "-static")

#for main chess
add_executable(
  chess
  src/main.cpp
  ${SOURCES}
  ${HEADERS}
)
target_include_directories(chess PRIVATE include)
target_link_libraries(
  chess
  raylib
)

#for testing
enable_testing()

add_executable(
  testing
  tests/helper_test.cpp
  tests/movement_test.cpp
  tests/engine_test.cpp
  ${SOURCES}
  ${HEADERS}
)
target_include_directories(testing PRIVATE include)
target_link_libraries(
  testing
  GTest::gtest_main
  raylib
)

include(GoogleTest)
gtest_discover_tests(testing)

#for installer
install(TARGETS chess RUNTIME DESTINATION bin)
install(DIRECTORY res DESTINATION .)
set(CPACK_PACKAGE_NAME "jaden chess engine")
set(CPACK_PACKAGE_VERSION "1.0")
include(CPack)
