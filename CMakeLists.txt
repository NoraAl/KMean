cmake_minimum_required(VERSION 3.9)
project(cluster)

find_package( OpenCV 3.3.0 REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )

set(CMAKE_CXX_STANDARD 11)

add_executable(cluster main.cpp helpers.cpp)

target_link_libraries( cluster ${OpenCV_LIBS})