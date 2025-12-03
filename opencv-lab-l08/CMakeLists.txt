cmake_minimum_required(VERSION 3.5)
project(opencv-lab-l08-fitting)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenCV REQUIRED)

add_executable(fitting
    src/fitting.cpp
)

target_link_libraries(fitting
    ${OpenCV_LIBS}
)

