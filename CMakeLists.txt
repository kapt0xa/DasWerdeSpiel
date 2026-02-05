# 3.28 recomended by SFML tutorial
cmake_minimum_required(VERSION 3.28)

# uncomment one of blocks to use prefered c/c++ compiler

# set(CMAKE_C_COMPILER "gcc")
# set(CMAKE_CXX_COMPILER "g++")

# set(CMAKE_C_COMPILER "clang")
# set(CMAKE_CXX_COMPILER "clang++")

project(DasWerdeSpiel LANGUAGES CXX)
set(SRC_DIR "${PROJECT_SOURCE_DIR}/src")
set(BOOST_QVM_INCLUDE "${CMAKE_BINARY_DIR}/_deps/boostqvm-src/include")

# add -DONLINE=ON to cmake command to use online repositories
# else local predownloaded repositories will be used
option(ONLINE "Use online repositories instead of local ones" OFF)
if(ONLINE)
    message("\nusing online repositories\n")
    set(SFML_REPOS "https://github.com/SFML/SFML.git")
    set(TGUI_REPOS "https://github.com/texus/TGUI/")
    set(QVM_REPOS "https://github.com/boostorg/qvm.git")
else()
    message("\nusing local repositories")
    message("add -DONLINE=ON to cmake command to use online repositories\n")
    set(SFML_REPOS "${PROJECT_SOURCE_DIR}/../SFML")
    set(TGUI_REPOS "${PROJECT_SOURCE_DIR}/../TGUI")
    set(QVM_REPOS "${PROJECT_SOURCE_DIR}/../BoostQVM")
endif()

# Get all .cpp and .h files in the src directory
file(GLOB_RECURSE SOURCES "${SRC_DIR}/*.cpp" "${SRC_DIR}/*.h")

#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY "${SFML_REPOS}"
    GIT_TAG 3.0.2
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(SFML)

set(TGUI_BACKEND SFML_GRAPHICS CACHE STRING "TGUI Backend")
FetchContent_Declare(TGUI
    GIT_REPOSITORY "${TGUI_REPOS}"
    GIT_TAG v1.12.0)
FetchContent_MakeAvailable(TGUI)

FetchContent_Declare(BoostQVM
    GIT_REPOSITORY "${QVM_REPOS}"
    GIT_TAG boost-1.90.0
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(BoostQVM)
include_directories("${BOOST_QVM_INCLUDE}")

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

target_link_libraries(DWS PRIVATE 
    SFML::Graphics SFML::Audio
    TGUI::TGUI)