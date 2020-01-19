cmake_minimum_required(VERSION 3.10)
project(shapeDetection)
find_package(OpenCV REQUIRED)

file(GLOB_RECURSE includes ./include/*.h)
file(GLOB_RECURSE sources ./src/*.cu ./src/*.cpp)
include_directories("${PROJECT_SOURCE_DIR}/include" ${OpenCV_INCLUDE_DIRS})
add_executable(shapeDetection src/main.cpp)
target_link_libraries(shapeDetection ${OpenCV_LIBS})

