
cmake_minimum_required(VERSION 3.10)
project(L01_rewrite VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(OpenCV REQUIRED)

add_library(l01_ops
    src/ops.cpp
    src/io.cpp
)
target_include_directories(l01_ops PUBLIC include)
target_link_libraries(l01_ops PUBLIC ${OpenCV_LIBS})

add_executable(l01_app src/main.cpp)
target_link_libraries(l01_app PRIVATE l01_ops)

# Nice warnings
if(MSVC)
    target_compile_options(l01_ops PRIVATE /W4 /permissive-)
    target_compile_options(l01_app PRIVATE /W4 /permissive-)
else()
    target_compile_options(l01_ops PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_options(l01_app PRIVATE -Wall -Wextra -Wpedantic)
endif()

install(TARGETS l01_app RUNTIME DESTINATION bin)
