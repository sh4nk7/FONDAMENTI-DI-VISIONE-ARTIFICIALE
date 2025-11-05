
cmake_minimum_required(VERSION 3.10)
project(L05_edges LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(OpenCV REQUIRED)

add_library(l05_filters
    src/args.cpp
    src/utils.cpp
    src/convolution.cpp
    src/filters.cpp
)
target_include_directories(l05_filters PUBLIC include)
target_link_libraries(l05_filters PUBLIC ${OpenCV_LIBS})

add_executable(l05_edges_app src/main.cpp)
target_link_libraries(l05_edges_app PRIVATE l05_filters)

if(MSVC)
    target_compile_options(l05_filters PRIVATE /W4 /permissive-)
    target_compile_options(l05_edges_app PRIVATE /W4 /permissive-)
else()
    target_compile_options(l05_filters PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_options(l05_edges_app PRIVATE -Wall -Wextra -Wpedantic)
endif()
