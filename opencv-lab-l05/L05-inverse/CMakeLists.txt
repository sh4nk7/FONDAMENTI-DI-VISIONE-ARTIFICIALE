cmake_minimum_required(VERSION 2.8)
project(lab5_2)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})
find_package(Eigen3 REQUIRED)
include_directories(${EIGEN3_INCLUDE_DIR})

include_directories(include)

add_definitions(-std=c++11)

add_executable(lab5_2 src/main.cpp)
target_link_libraries(lab5_2 ${OpenCV_LIBS})
install(TARGETS lab5_2 RUNTIME DESTINATION bin)

