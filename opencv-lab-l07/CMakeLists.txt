cmake_minimum_required(VERSION 3.5)
project(lab07_stereo)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenCV REQUIRED)

add_executable(stereo simple.cpp)
target_link_libraries(stereo PRIVATE ${OpenCV_LIBS})

install(TARGETS stereo RUNTIME DESTINATION bin)


