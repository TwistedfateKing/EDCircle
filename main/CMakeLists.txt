cmake_minimum_required(VERSION 3.15.0)

set(CMAKE_CXX_STANDARD 11)

set(TARGET_MAIN_CPP main)

add_executable(main main.cpp)

target_include_directories(${TARGET_MAIN_CPP} PRIVATE "${OPENCV_DIR}/include")
target_link_libraries(${TARGET_MAIN_CPP} PUBLIC opencv_core opencv_videoio opencv_highgui opencv_imgproc opencv_imgcodecs)