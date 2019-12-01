cmake_minimum_required(VERSION 3.15.0)

project(EDcircle)

set(CMAKE_VERBOSE_MAKEFILE on)
set(CMAKE_CXX_STANDARD 11)

set(TARGET_ED EdgeDrawing)
set(TARGET_EDCIRCLE EDCircle)

add_library(${TARGET_ED} SHARED)
add_library(${TARGET_EDCIRCLE} SHARED)

target_sources(${TARGET_ED}
        PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/src/edge_drawing.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/edge_drawing.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/edge_drawing_parameter_free.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/edge_drawing_parameter_free.h"
        )

target_sources(${TARGET_EDCIRCLE}
        PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/src/ed_circle.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/ed_circle.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/ed_line.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/ed_line.h"
        )

add_subdirectory(main)

