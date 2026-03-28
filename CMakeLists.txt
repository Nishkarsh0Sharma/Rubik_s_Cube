cmake_minimum_required(VERSION 3.16)
project(Rubik_s_Cube)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(. PatternDatabase)


add_executable(Rubik_s_Cube
        main.cpp
        PatternDatabase/GenericRubicksCube.cpp
        PatternDatabase/Patterns/NibbleArray.cpp
        PatternDatabase/Patterns/PatternDatabase.cpp
        PatternDatabase/Patterns/CornerPatternDatabase.cpp
        PatternDatabase/Patterns/CornerDBMaker.cpp
        PatternDatabase/Patterns/math.cpp
)
