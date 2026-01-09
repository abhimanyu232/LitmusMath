set(PROJECT_NAME LitmusMath)

### Google Tests block
include(FetchContent)
FetchContent_Declare(
googletest
# Specify the commit you depend on and update it regularly.
URL https://github.com/google/googletest/archive/5376968f6948923e2411081fd9372e71a59d8e77.zip
DOWNLOAD_EXTRACT_TIMESTAMP 1
SYSTEM
)
## For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
#   include(CTest)
# endif()
### OR 
# enable_testing()

file(GLOB_RECURSE sources_test CONFIGURE_DEPENDS *.cpp *.cc)

set(TEST_NAME ${PROJECT_NAME}_tests)
add_executable(${TEST_NAME} ${sources_test})
target_link_libraries(${TEST_NAME} gtest gtest_main)
## Extra linking to this project.
target_link_libraries(${TEST_NAME} lm_matlib lm_stats lm_timer)

# gtest_discover_tests(${TEST_NAME})
add_test(UnitTests ${TEST_NAME})

# add_executable(MATRIX_TEST tests/matrix_tests.cpp)
# target_link_libraries(MATRIX_TEST gtest gtest_main)
# target_link_libraries(MATRIX_TEST lm_matlib)
# add_test(matrix_tests MATRIX_TEST)

# add_executable(STATS_TEST tests/statistics_test.cc)
# target_link_libraries(STATS_TEST gtest gtest_main)
# target_link_libraries(MATRIX_TEST lm_matlib)
# add_test(stats_test STATS_TEST)
## Extra linking to this project.

### IDE clean-up
set_target_properties(gtest PROPERTIES FOLDER extern)
set_target_properties(gtest_main PROPERTIES FOLDER extern)
set_target_properties(gmock PROPERTIES FOLDER extern)
set_target_properties(gmock_main PROPERTIES FOLDER extern) 

### clang-tidy to ignore google-test files
set_target_properties(gtest PROPERTIES CXX_CLANG_TIDY "")
set_target_properties(gtest_main PROPERTIES CXX_CLANG_TIDY "")
set_target_properties(gmock PROPERTIES CXX_CLANG_TIDY "")
set_target_properties(gmock_main PROPERTIES CXX_CLANG_TIDY "")


set_target_properties(${TEST_NAME}  PROPERTIES CXX_CLANG_TIDY "")
set_target_properties(${TEST_NAME}  PROPERTIES CXX_CLANG_TIDY "")
set_target_properties(${TEST_NAME}  PROPERTIES CXX_CLANG_TIDY "")
set_target_properties(${TEST_NAME}  PROPERTIES CXX_CLANG_TIDY "")
