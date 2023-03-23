include(FetchContent)

FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        release-1.11.0
)
FetchContent_MakeAvailable(googletest)
add_library(GTest::GTest INTERFACE IMPORTED)
target_link_libraries(GTest::GTest INTERFACE gtest_main)

add_executable(TrianglesIntersection_test test.cpp)

target_link_libraries(TrianglesIntersection_test
  PRIVATE
  GTest::GTest
  Primitives
  compiler_flags)

add_test(TrianglesIntersection_gtest TrianglesIntersection_test)