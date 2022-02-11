cmake_minimum_required(VERSION 3.12)

project(PokeGuesserCinematicUniverse VERSION 0.0.1 DESCRIPTION "A game")

set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${CMAKE_CURRENT_SOURCE_DIR}/cmake")

find_package(Threads REQUIRED)
find_package(nlohmann_json REQUIRED)
find_package(OpenSSL REQUIRED)

include(CPM)
# I use CPM and not vcpkg for this one because VCPKG version is not up to date
CPMAddPackage("gh:yhirose/cpp-httplib@0.10.1")

add_executable(poke_guesser
	${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/App.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/PokeAPI.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/Random.cpp
	)

target_compile_features(poke_guesser PRIVATE cxx_std_20)
target_include_directories(poke_guesser PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src ${cpp-httplib_SOURCE_DIR}) 
target_link_libraries(poke_guesser PRIVATE Threads::Threads nlohmann_json::nlohmann_json OpenSSL::SSL OpenSSL::Crypto)
