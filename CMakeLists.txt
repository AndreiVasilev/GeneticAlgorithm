cmake_minimum_required(VERSION 3.6)
project(TextGenerator_GeneticAlgorithm)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES src/main.cpp src/Phrase.cpp include/Phrase.h src/Population.cpp include/Population.h)
add_executable(TextGenerator_GeneticAlgorithm ${SOURCE_FILES})
