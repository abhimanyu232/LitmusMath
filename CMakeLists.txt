cmake_minimum_required(VERSION 3.28)

set(PROJECT_NAME LitmusMath)
# project(${PROJECT_NAME})
project( 
  ${PROJECT_NAME}
  LANGUAGES CXX
  )

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS "-march=native")

set(default_build_type "RelWithDebInfo")
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "Setting build type to '${default_build_type}' as none was specified.")
  set(CMAKE_BUILD_TYPE "${default_build_type}" CACHE
      STRING "Choose the type of build." FORCE)
  # Set the possible values of build type for cmake-gui
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS
    "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()
# "Debug: {-g}" "Release: {-O3 -DNDEBUG} " "MinSizeRel: {-Os}" "RelWithDebInfo: { -O2 -g -DNDEBUG}")


########################################
## Define include directories
set(COMMON_INCLUDES ${PROJECT_SOURCE_DIR}/include)
include_directories(${COMMON_INCLUDES})

#### file globbing ##############################################################
### These instructions search the directory tree when cmake is
### invoked and put all files that match the pattern in the variables 
### `sources` and `data`.
file(GLOB_RECURSE sources CONFIGURE_DEPENDS src/*.cpp)

### Compile header files into a library
add_library(lm_matlib INTERFACE)
target_include_directories(lm_matlib INTERFACE ${COMMON_INCLUDES})
### IF I needed Boost say
# target_link_libraries(lm_matlib INTERFACE Boost::Boost)

### Main as a library client)
add_executable(${PROJECT_NAME} ${sources})
target_link_libraries(${PROJECT_NAME} lm_matlib)
target_compile_options(${PROJECT_NAME} PUBLIC -std=c++23)

###### dependencies ##
### This defines the variables Boost_LIBRARIES that contains all library names
### that we need to link into the program.
### find_package(Boost 1.36.0 COMPONENTS filesystem system REQUIRED)
# target_link_libraries(example PUBLIC
#   ${Boost_LIBRARIES}
#   # here you can add any library dependencies
# )


set(TESTING OFF CACHE BOOL "True if tests shall be built")
if (TESTING)
  ### Google Tests 
  include(FetchContent)
  FetchContent_Declare(
    googletest
    # Specify the commit you depend on and update it regularly.
    URL https://github.com/google/googletest/archive/5376968f6948923e2411081fd9372e71a59d8e77.zip
    DOWNLOAD_EXTRACT_TIMESTAMP 1
  )
  ### For Windows: Prevent overriding the parent project's compiler/linker settings
  # set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
  FetchContent_MakeAvailable(googletest)

  # if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
  #   include(CTest)
  # endif()
  ### OR 
  enable_testing()

  file(GLOB_RECURSE sources_test CONFIGURE_DEPENDS tests/*.cpp)

  set(TEST_NAME ${PROJECT_NAME}_tests)
  add_executable(${TEST_NAME} ${sources_test})
  target_link_libraries(${TEST_NAME} gtest gtest_main)
  ## Extra linking to this project.
  target_link_libraries(${TEST_NAME} lm_matlib)

  # target_compile_options(${TEST_NAME} PUBLIC -O2 -Wall -Werror=return-type -Wconversion -Wimplicit-fallthrough -Werror=missing-declarations -Werror=redundant-decls )

  # gtest_discover_tests(${TEST_NAME})
  add_test(UnitTests ${TEST_NAME})


  ### IDE clean-up
  set_target_properties(gtest PROPERTIES FOLDER extern)
  set_target_properties(gtest_main PROPERTIES FOLDER extern)
  set_target_properties(gmock PROPERTIES FOLDER extern)
  set_target_properties(gmock_main PROPERTIES FOLDER extern) 


endif()