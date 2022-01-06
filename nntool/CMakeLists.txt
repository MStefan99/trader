cmake_minimum_required(VERSION 3.16)
project(trader)

set(CMAKE_CXX_STANDARD 20)

if(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS_DEBUG "-Og")
set(CMAKE_CXX_FLAGS_RELEASE "-Ofast")

if (${CMAKE_CXX_COMPILER} MATCHES "clang-cl")
	add_compile_options(/EHs)
else ()
	add_link_options(-pthread)
endif ()

add_subdirectory(NeuralNetwork)

add_executable(trader main.cpp)
target_link_libraries(trader NeuralNetwork)
