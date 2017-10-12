cmake_minimum_required(VERSION 2.6 FATAL_ERROR)

set(CMAKE_CXX_STANDARD 11)

project(trc2root)

list(APPEND CMAKE_PREFIX_PATH $/home/ide/PROGS/root-6.10.06-install/)

find_package(ROOT REQUIRED)

include_directories(${ROOT_INCLUDE_DIRS})

include_directories(${PROJECT_SOURCE_DIR}/include)
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cpp)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.h)

add_executable(trc2root trc2root.cpp ${sources} ${headers})
target_link_libraries(trc2root ${ROOT_LIBRARIES})
