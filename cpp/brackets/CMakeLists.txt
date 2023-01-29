# Set the minimum required version of CMake to be 3.10
cmake_minimum_required(VERSION 3.10)

# TODO 2: Create a project named Brackets
project(Brackets  VERSION 1.1)

# Set the variable CMAKE_CXX_STANDARD to 11
# and the variable CMAKE_CXX_STANDARD_REQUIRED to True
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# download the Catch2 libary from git 
Include(FetchContent)
FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v3.3.0 # or a later release
)
FetchContent_MakeAvailable(Catch2)
list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)

# alternative with Catch2 in a subdirectory
#add_subdirectory(lib/Catch2)

# alternatively in CMake is installed system wide
#find_package(Catch2 3 REQUIRED)

# adds an executable named `tests`
add_executable(tests deque.test.cpp stack.test.cpp brackets.tests.cpp bracket_matcher.cpp)

# These tests can use the Catch2-provided main
target_link_libraries(tests PRIVATE Catch2::Catch2WithMain)

include(CTest)
include(Catch)
catch_discover_tests(tests)
