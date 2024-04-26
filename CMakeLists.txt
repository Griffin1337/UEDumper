cmake_minimum_required(VERSION 3.24)
project(UEDumper)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)
add_compile_options(-utf-8 -w)

# configure output directories
# Set output directories for all build types (Release, Debug, etc.)
set(BUILD_OUTPUTS ${CMAKE_CURRENT_SOURCE_DIR}/build/${CMAKE_HOST_SYSTEM_NAME}/${CMAKE_SYSTEM_PROCESSOR})

add_subdirectory(./MemStream)
add_subdirectory(./UEDumper)

