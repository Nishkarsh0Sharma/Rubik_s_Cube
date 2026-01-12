//
// Created by Nishkarsh Sharma on 12-01-2026.
//
#ifndef RUBIKS_CUBE_SOLVER_CORNERPATTERNDATABASE_H
#define RUBIKS_CUBE_SOLVER_CORNERPATTERNDATABASE_H

#include "../GenericRubicksCube.h"
#include "PatternDatabase.h"
#include "PermutationIndexer.h"
using namespace std;

class CornerPatternDatabase : public PatternDatabase {

    typedef GenericRubicksCube::FACE F;

    PermutationIndexer<8> permIndexer;

public:
    CornerPatternDatabase();
    CornerPatternDatabase(uint8_t init_val);
    uint32_t getDatabaseIndex(const GenericRubicksCube& cube) const;

};


#endif //RUBIKS_CUBE_SOLVER_CORNERPATTERNDATABASE_H