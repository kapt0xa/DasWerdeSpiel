#include <iostream>
#include <iomanip>
#include <sstream>
#include <exception>
#include <array>
#include <cmath>
#include <random>
#include <vector>
#include <memory>
#include <string>
#include <chrono>

#include <SFML/Graphics.hpp>

#include "GameLoop.h"
#include "Game.h"
#include "Puzzle15Game.h"

using namespace spiel;

void compilerValidation();

constexpr float pi = 3.14159265358979323846f;

int main()
{
    
}


void compilerValidation()
{
#ifdef __GNUC__
#if __GNUC__ == 7 && __GNUC_MINOR__ >= 1 || __GNUC__ > 7
        std::cout << "Compiled with g++ version >= 7.1" << std::endl;
#else
        std::cout << "Compiled with g++ version < 7.1" << std::endl;
#endif
#elif defined __clang__
    std::cout << "Compiled with clang compiler." << std::endl;
#else
    std::cout << "Not using g++/clang compiler." << std::endl;
#endif

    // Check for C++17
#if __cplusplus >= 201703L
    std::cout << "Compiled with C++17 or later." << std::endl;
#else
    std::cout << "Not compiled with C++17." << std::endl;
#endif
}
