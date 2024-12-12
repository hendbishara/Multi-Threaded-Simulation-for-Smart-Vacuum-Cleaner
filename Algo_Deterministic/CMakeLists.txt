cmake_minimum_required(VERSION 3.13)

project(EX3 VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set_property(GLOBAL PROPERTY CXX_STANDARD 20)

# Collect all .cpp files from the algo_209120054 directory
file(GLOB ALGO_SOURCES "Algo_209120054_207759085_Deterministic/*.cpp")

# Collect all .cpp files from the common_algo directory
file(GLOB COMMON_ALGO_SOURCES "common_algo/*.cpp")

# Add the Algo_209120054 library with all collected sources
add_library(Algo_209120054_207759085_Deterministic SHARED Algo_209120054_207759085_Deterministic.cpp ${COMMON_ALGO_SOURCES})

target_include_directories(Algo_209120054_207759085_Deterministic PRIVATE include)
