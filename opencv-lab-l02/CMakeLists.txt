cmake_minimum_required(VERSION 3.10)
project(L02_convolution)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS} include)

add_executable(main
    src/main.cpp
    src/myfilter2D.cpp
    src/utils.cpp
)

target_link_libraries(main ${OpenCV_LIBS})

