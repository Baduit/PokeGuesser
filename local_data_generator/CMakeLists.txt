cmake_minimum_required(VERSION 3.24)

project(local_data_generator VERSION 0.0.1 DESCRIPTION "Generate all the needed data for Pokeguesser from the data of github repository of pokeapi to avoid calling the API everyday")

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(strong_type CONFIG REQUIRED)

set(SRCS src/main.cpp)

add_executable(local_data_generator ${SRCS})
target_compile_options(local_data_generator
					   PRIVATE
					   $<$<CXX_COMPILER_ID:MSVC>:/W3 /permissive- /TP>
					   $<$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>:-Wextra -Wall -Wsign-conversion -Wfloat-equal -pedantic -Wredundant-decls -Wshadow -Wpointer-arith -O3>)
target_link_libraries(local_data_generator PRIVATE strong_type::strong_type)					   
 
