# 3.28 recomended by SFML tutorial
cmake_minimum_required(VERSION 3.28)


set(MY_LIB_PATH_MODE "Debug" CACHE STRING "Select which library build to link against")
# Optional: Provide a dropdown menu in the GUI
set_property(CACHE MY_LIB_PATH_MODE PROPERTY STRINGS "Debug" "Release")

if(MY_LIB_PATH_MODE STREQUAL "Debug")
    set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "" FORCE)
    set(SFML_DIR "${CMAKE_SOURCE_DIR}/../SFML_DBG/lib/cmake/SFML")
    set(TGUI_DIR "${CMAKE_SOURCE_DIR}/../TGUI_DBG/lib/cmake/TGUI")
else()
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "" FORCE)
    set(SFML_DIR "${CMAKE_SOURCE_DIR}/../SFML_RLS/lib/cmake/SFML")
    set(TGUI_DIR "${CMAKE_SOURCE_DIR}/../TGUI_RLS/lib/cmake/TGUI")
endif()

if(DEFINED GCC) # add -DGCC to use g++ compiler
    set(CMAKE_C_COMPILER "gcc")
    set(CMAKE_CXX_COMPILER "g++")
elseif(DEFINED CLANG) # add -DCLANG to use clang++ compiler
    set(CMAKE_C_COMPILER "clang")
    set(CMAKE_CXX_COMPILER "clang++")
endif()

project(DasWerdeSpiel LANGUAGES CXX)
set(SRC_DIR "${PROJECT_SOURCE_DIR}/src")

# Get all .cpp and .h files in the src directory
file(GLOB_RECURSE SOURCES "${SRC_DIR}/*.cpp" "${SRC_DIR}/*.h")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

find_package(SFML 3.0.2 COMPONENTS Graphics Audio System REQUIRED) # Graphics, Window, System, Audio, Network
find_package(TGUI 1.12.0 REQUIRED)

# boost::QVM
include_directories(${PROJECT_SOURCE_DIR}/../BoostQVM/include)

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

target_link_libraries(DWS PRIVATE TGUI::TGUI SFML::Graphics SFML::Audio SFML::System)