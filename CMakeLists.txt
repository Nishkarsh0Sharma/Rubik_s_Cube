cmake_minimum_required(VERSION 4.0)
project(Rubik_s_Cube)

set(CMAKE_CXX_STANDARD 20)

add_executable(Rubik_s_Cube main.cpp
        PatternDatabase/GenericRubicksCube.cpp
        PatternDatabase/GenericRubicksCube.h
        PatternDatabase/RubiksCube3dArray.cpp
        PatternDatabase/RubiksCube1dArray.cpp
        PatternDatabase/RubiksCubeBitboard.cpp
)
