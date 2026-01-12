cmake_minimum_required(VERSION 4.0)
project(Rubik_s_Cube)

set(CMAKE_CXX_STANDARD 20)


include_directories(PatternDatabase)


add_executable(Rubik_s_Cube
        main.cpp
        PatternDatabase/GenericRubicksCube.cpp
        PatternDatabase/GenericRubicksCube.h
        bits/stdc++.h
        PatternDatabase/RubiksCube1dArray.cpp
        PatternDatabase/RubiksCubeBitboard.cpp
        Solver/DFSSolver.h
        Solver/BFSSolver.h
        Solver/IDDFSSolver.h
        PatternDatabase/PatternDatabase.h
        Solver/IDAstarSolver.h
        PatternDatabase/Patterns/NibbleArray.cpp
        PatternDatabase/Patterns/NibbleArray.h
        PatternDatabase/Patterns/PatternDatabase.cpp
        PatternDatabase/Patterns/PatternDatabase.h
        PatternDatabase/Patterns/PermutationIndexer.h
        PatternDatabase/Patterns/CornerPatternDatabase.cpp
        PatternDatabase/Patterns/CornerPatternDatabase.h
        PatternDatabase/Patterns/CornerDBMaker.cpp
        PatternDatabase/Patterns/CornerDBMaker.h
        PatternDatabase/Patterns/math.cpp
        PatternDatabase/Patterns/math.h
)

target_link_libraries(Rubik_s_Cube ${OpenCV_LIBS})
