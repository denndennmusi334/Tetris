cmake_minimum_required(VERSION 3.10)
cmake_minimum_required(VERSION 3.10)

project(Tetris)

set(CMAKE_CXX_STANDARD 17)

file(GLOB SRC
    *.cpp
)

add_executable(Tetris ${SRC})

target_include_directories(Tetris PRIVATE
    "C:/DxLib"
)