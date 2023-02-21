cmake_minimum_required(VERSION 3.5)
project(rm LANGUAGES CXX)
find_package(OpenCV 4 REQUIRED)

include_directories( ${OpenCV_INCLUDE_DIRS} )
add_executable(rm main.cpp)
target_link_libraries( rm ${OpenCV_LIBS} )

