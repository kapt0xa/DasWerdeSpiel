# 3.28 recomended by SFML tutorial
cmake_minimum_required(VERSION 3.28)

# uncomment one of blocks to use prefered c/c++ compiler

# set(CMAKE_C_COMPILER "gcc")
# set(CMAKE_CXX_COMPILER "g++")

# set(CMAKE_C_COMPILER "clang")
# set(CMAKE_CXX_COMPILER "clang++")

project(DasWerdeSpiel LANGUAGES CXX)
set(SRC_DIR "${PROJECT_SOURCE_DIR}/src")

# Get all .cpp and .h files in the src directory
file(GLOB_RECURSE SOURCES "${SRC_DIR}/*.cpp" "${SRC_DIR}/*.h")

#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include(FetchContent)
FetchContent_Declare(SFML
#    GIT_REPOSITORY https://github.com/SFML/SFML.git # original command
#    to let it work localy U need to clone the repository "git clone https://github.com/SFML/SFML.git ../../SFML" (if surrent folder is build folder)
    GIT_REPOSITORY "${PROJECT_SOURCE_DIR}/../SFML" # local cloned lib that does not require internet connection
    GIT_TAG 3.0.2
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(SFML)

add_executable(DWS ${SOURCES})

# Set the C++ standard to C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS OFF) # Disable compiler-specific extensions

# Stricter warning settings
if(MSVC)  # For Microsoft Visual Studio
    target_compile_options(DWS PRIVATE /W3 /WX)  # Warning level 3, treat warnings as errors
else()  # For other compilers like GCC or Clang
    target_compile_options(DWS PRIVATE -Wall -Werror)  # Treat all warnings as errors
endif()

target_link_libraries(DWS PRIVATE SFML::Graphics SFML::Audio)