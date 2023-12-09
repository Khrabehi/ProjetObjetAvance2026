
cmake_policy(SET CMP0135 NEW)

# Copy-paste from http://google.github.io/googletest/quickstart-cmake.html

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(
  hello_test
  hello_test.cpp # Only modification: cc -> cpp
)
target_link_libraries(
  hello_test
  PRIVATE
    base
    GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(hello_test)
