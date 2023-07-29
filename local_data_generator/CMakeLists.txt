cmake_minimum_required(VERSION 3.24)

project(local_data_generator VERSION 0.0.1 DESCRIPTION "Generate all the needed data for Pokeguesser from the data of github repository of pokeapi to avoid calling the API everyday")

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(SRCS src/main.cpp)

add_executable(local_data_generator ${SRCS})
