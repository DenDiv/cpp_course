include(FetchContent)

FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        release-1.11.0
)
FetchContent_MakeAvailable(googletest)
add_library(GTest::GTest INTERFACE IMPORTED)
target_link_libraries(GTest::GTest INTERFACE gtest_main)

add_executable(Cache_test test.cpp)
target_include_directories(Cache_test PUBLIC
                           "${PROJECT_BINARY_DIR}"
                           )

target_link_libraries(Cache_test
  PRIVATE
  GTest::GTest
  Caches
  compiler_flags)

add_test(Cache_gtest Cache_test)