cmake_minimum_required(VERSION 3.24)
project(UEDumper_main)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)
add_compile_options(-utf-8 -w)

# bring in dependencies
link_directories(../MemStream/build/Windows/AMD64)
include_directories(../MemStream/include)

#file(GLOB main ./*.cpp)
file(GLOB_RECURSE Engine ./Engine/*.cpp)
file(GLOB_RECURSE Frontend ./Frontend/*.cpp)
file(GLOB_RECURSE Memory ./Memory/*.cpp)
file(GLOB_RECURSE Resources ./Resources/*.cpp)
file(GLOB_RECURSE Settings ./Settings/*.cpp)

# Recursively search for header files in all subdirectories
file(GLOB_RECURSE HEADER_FILES "./*.h" "./*.hpp")

# Add all directories containing header files to the include path
foreach(header ${HEADER_FILES})
    get_filename_component(header_path "${header}" PATH)
    list(APPEND HEADER_DIRS ${header_path})
endforeach()
list(REMOVE_DUPLICATES HEADER_DIRS)

include_directories(${HEADER_DIRS})

add_executable(${PROJECT_NAME} UEDumper.cpp ${Engine} ${Frontend} ${Memory} ${Resources} ${Settings})
target_compile_definitions(${PROJECT_NAME} PUBLIC UNICODE _UNICODE)

target_link_libraries(${PROJECT_NAME} D3D11 windowscodecs)
target_include_directories(${PROJECT_NAME} PRIVATE ./ ./Frontend/ImGui  ./Resources/AES ./Resources/Json ./Resources/Dumpspace)

set_target_properties(${PROJECT_NAME} PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY ${BUILD_OUTPUTS}/${PROJECT_NAME}
        LIBRARY_OUTPUT_DIRECTORY ${BUILD_OUTPUTS}/${PROJECT_NAME}
        RUNTIME_OUTPUT_DIRECTORY ${BUILD_OUTPUTS}/${PROJECT_NAME})