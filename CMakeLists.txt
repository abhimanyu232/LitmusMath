cmake_minimum_required(VERSION 3.20)

set(PROJECT_NAME LitmusMath)
project(${PROJECT_NAME} VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_CXX_FLAGS "-march=native -mtune=native")

### Clang Tidy
find_program(CLANG_TIDY NAMES "clang-tidy-20")

if(CLANG_TIDY)
  message(STATUS "Clang-tidy found: ${CLANG_TIDY}")
  set(CMAKE_CXX_CLANG_TIDY
        ${CLANG_TIDY}
        "-config-file=${CMAKE_CURRENT_SOURCE_DIR}/.clang-tidy"
        "--extra-arg=-Wno-unknown-warning-option"
        "--extra-arg=-stdlib=libc++" # ensures that clang-tidy uses the right stdlib, else shows errors with gcc. does not affect compilation
        "--extra-arg=-std=c++23" 
    ) 
else() ##-Wno-unknown-warning-option,-stdlib=libc++,-std=c++23
  message(WARNING "Clang-tidy not found. Static analysis disabled.")
endif()

### SET BUILD TYPE
## "Debug: {-g}" 
## "Release: {-O3 -DNDEBUG} " 
## "MinSizeRel: {-Os}" 
## "RelWithDebInfo: { -O2 -g -DNDEBUG}") [DEFAULT]
set(default_build_type "RelWithDebInfo")
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "Setting build type to '${default_build_type}' as none was specified.")
  set(CMAKE_BUILD_TYPE "${default_build_type}" CACHE
      STRING "Choose the type of build." FORCE)
  # Set the possible values of build type for cmake-gui
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS
    "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()


### Define include directories
set(COMMON_INCLUDES ${PROJECT_SOURCE_DIR}/include)
include_directories(${COMMON_INCLUDES})

### Compile header files into a library
## local matrix library     !!! not needed, doesnt do anything
add_library(lm_matlib INTERFACE ${PROJECT_SOURCE_DIR}/include/matrix.h) 
set_target_properties(lm_matlib PROPERTIES CXX_CLANG_TIDY "${CMAKE_CXX_CLANG_TIDY}")

## local statistics library !!! not needed, doesnt do anything
add_library(lm_stats INTERFACE ${PROJECT_SOURCE_DIR}/include/statistics.h)
set_target_properties(lm_stats PROPERTIES CXX_CLANG_TIDY "${CMAKE_CXX_CLANG_TIDY}")

## local time library       !!! not needed, doesnt do anything
add_library(lm_timer INTERFACE ${PROJECT_SOURCE_DIR}/include/timer.h)
set_target_properties(lm_timer PROPERTIES CXX_CLANG_TIDY "${CMAKE_CXX_CLANG_TIDY}")

## IF I needed an external library, say header only Boost 
# target_link_libraries(lm_matlib INTERFACE Boost::Boost)

### file globbing !!! not used ##################################################
# file(GLOB_RECURSE sources CONFIGURE_DEPENDS src/*.cpp src/*.cc)

### Define Targets


## TARGET: Main as a library client
add_executable(${PROJECT_NAME})
target_sources(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/src/main.cpp)
target_compile_options(${PROJECT_NAME} PUBLIC -std=c++23)
# This can enable Clang-Tidy for the test executable too
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_CLANG_TIDY "${CMAKE_CXX_CLANG_TIDY}")
## no need to link header only libraries, simple #include directive is good enough. 
# target_link_libraries(${PROJECT_NAME} lm_matlib lm_stats lm_timer) 

## TARGET: Benchmark
set(TargetBenchMM run_bench_mm)
add_executable(${TargetBenchMM})
target_sources(${TargetBenchMM} PUBLIC ${PROJECT_SOURCE_DIR}/src/bench_mm.cc)
target_compile_options(${TargetBenchMM} PUBLIC -std=c++23)
set_target_properties(${TargetBenchMM} PROPERTIES CXX_CLANG_TIDY "${CMAKE_CXX_CLANG_TIDY}")

###### hypothetical dependencies ######
# find_package(Boost 1.36.0 COMPONENTS filesystem system REQUIRED)
# target_link_libraries(example PUBLIC
#   ${Boost_LIBRARIES}
#   # here you can add any library dependencies
# )

set(TESTING OFF CACHE BOOL "True if tests shall be built")
if (TESTING)
  enable_testing()
  add_subdirectory(tests)
endif() 