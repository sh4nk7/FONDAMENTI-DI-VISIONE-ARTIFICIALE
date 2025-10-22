
cmake_minimum_required(VERSION 3.10)
project(L03_background VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(OpenCV REQUIRED)

add_library(bg_ops
    src/background.cpp
    src/io.cpp
)
target_include_directories(bg_ops PUBLIC include)
target_link_libraries(bg_ops PUBLIC ${OpenCV_LIBS})

add_executable(l03_bg_app src/main.cpp)
target_link_libraries(l03_bg_app PRIVATE bg_ops)

if(MSVC)
    target_compile_options(bg_ops PRIVATE /W4 /permissive-)
    target_compile_options(l03_bg_app PRIVATE /W4 /permissive-)
else()
    target_compile_options(bg_ops PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_options(l03_bg_app PRIVATE -Wall -Wextra -Wpedantic)
endif()
