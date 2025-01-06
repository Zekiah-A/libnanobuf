cmake_minimum_required(VERSION 3.31)
project(libnanobuf VERSION 1.0)

# Set C standard
set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED True)

# Include directories for the header files
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# List of source files for the library
set(SOURCE_FILES
    nanobuf.c
    buf_reader.c
    buf_writer.c
)

# Create both static and shared libraries
add_library(nanobuf STATIC ${SOURCE_FILES})
add_library(nanobuf_shared SHARED ${SOURCE_FILES})

# Specify the include directories
target_include_directories(nanobuf PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(nanobuf_shared PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Installation instructions
install(TARGETS nanobuf DESTINATION lib)
install(TARGETS nanobuf_shared DESTINATION lib)
install(FILES
    nanobuf_defs.h
    nanobuf.h
    buf_reader.h
    buf_writer.h
    DESTINATION include
)

# Enable testing
enable_testing()

# Add tests directory
add_subdirectory(tests)
