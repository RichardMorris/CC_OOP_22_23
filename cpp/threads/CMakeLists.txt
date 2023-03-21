# Set the minimum required version of CMake to be 3.10
cmake_minimum_required(VERSION 3.10)

# TODO 2: Create a project named Brackets
project(Brackets  VERSION 1.1)

# Set the variable CMAKE_CXX_STANDARD to 14
# and the variable CMAKE_CXX_STANDARD_REQUIRED to True
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# alternative with Catch2 in a subdirectory
#add_subdirectory(lib/Catch2)

# alternatively in CMake is installed system wide
#find_package(Catch2 3 REQUIRED)


set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

# adds an executable named `tests`
add_executable(threadex threads.cpp)
add_executable(messages send.cpp)
add_executable(gpt gpt.cpp)

#target_link_libraries(gpt PRIVATE Threads::Threads)
target_link_libraries(gpt pthread)
