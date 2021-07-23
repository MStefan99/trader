cmake_minimum_required(VERSION 3.16)
project(trader)

set(CMAKE_CXX_STANDARD 20)

#add_subdirectory(lib/eigen-3.3.9)
add_subdirectory(nn)

add_executable(trader main.cpp)
target_link_libraries(trader NeuralNetwork)
