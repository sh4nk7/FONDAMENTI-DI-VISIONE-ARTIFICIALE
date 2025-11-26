cmake_minimum_required(VERSION 3.5)
project(lab04_morphology)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenCV REQUIRED)

add_executable(l04 src/exprunning.cpp)
target_link_libraries(l04 PRIVATE ${OpenCV_LIBS})

install(TARGETS l04 RUNTIME DESTINATION bin)

