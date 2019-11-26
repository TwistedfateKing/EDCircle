cmake_minimum_required(VERSION 3.15.0)

project(EDcircle)

set(CMAKE_VERBOSE_MAKEFILE on)
set(CMAKE_CXX_STANDARD 11)

set(TARGET_ED EdgeDrawing)

add_library(${TARGET_ED} SHARED)

target_sources(${TARGET_ED}
        PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/src/edge_drawing.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/edge_drawing.h"
        )

add_subdirectory(main)

