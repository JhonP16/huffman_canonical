cmake_minimum_required(VERSION 3.30)
project(huffman_canonical)

set(CMAKE_CXX_STANDARD 20)

add_executable(huffman_canonical src/main.cpp
        src/huffman.cpp)
