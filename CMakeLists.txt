cmake_minimum_required(VERSION 3.13)

project(EX3 VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set (CMAKE_CXX_STANDARD_REQUIRED YES)
set_property(GLOBAL PROPERTY CXX_STANDARD 20)


#linking boost
find_package(Boost 1.70 REQUIRED COMPONENTS system)

include_directories(${Boost_INCLUDE_DIRS})


# Collect all .cpp files from the Algo_209120054_207759085_Deterministic directory
file(GLOB ALGO_SOURCES "Algo_209120054_207759085_Deterministic/*.cpp")

# Collect all .cpp files from the Algo_209120054_207759085_Random directory
file(GLOB ALGO2_SOURCES "Algo_209120054_207759085_Random/*.cpp")


# Collect all .cpp files from the common_algo directory
file(GLOB COMMON_ALGO_SOURCES "common_algo/*.cpp")


# Add the Algo_209120054_207759085_Deterministic with all collected sources
add_library(Algo_209120054_207759085_Deterministic MODULE ${ALGO_SOURCES} ${COMMON_ALGO_SOURCES})


# Add the Algo_209120054_207759085_Random library with all collected sources
add_library(Algo_209120054_207759085_Random MODULE ${ALGO2_SOURCES} ${COMMON_ALGO_SOURCES})

  
target_include_directories(Algo_209120054_207759085_Deterministic PUBLIC ${CMAKE_SOURCE_DIR})

target_include_directories(Algo_209120054_207759085_Random PUBLIC ${CMAKE_SOURCE_DIR})

add_executable(myrobot simulator/myrobot.cpp simulator/AlgorithmRegistrar.cpp simulator/MySimulator.cpp simulator/robot_state.cpp simulator/house.cpp simulator/mywallsensor.cpp simulator/mybatterymeter.cpp simulator/mydirtsensor.cpp)

target_link_options(myrobot PUBLIC "-rdynamic")

target_include_directories(myrobot PUBLIC ${CMAKE_SOURCE_DIR})

target_link_libraries(myrobot PUBLIC dl ${Boost_LIBRARIES})



