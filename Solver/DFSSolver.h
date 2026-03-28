//
// Created by Nishkarsh Sharma on 12-01-2026.
//

#include "../bits/stdc++.h"
#include "../PatternDatabase/GenericRubicksCube.h"

#ifndef RUBIK_S_CUBE_DFSSOLVER_H
#define RUBIK_S_CUBE_DFSSOLVER_H

// Typename T: RubiksCube Representation used (3d, 1d, Bitboard)
// Typename H: Corresponding Hash function

template<typename T, typename H>
class DFSSolver {
private:
    vector<GenericRubicksCube::MOVE> moves;
    int max_search_depth;

    // DFS code to find the solution ( helper function )
    bool dfs(int dep) {
        if ( rubiksCube.isSolved() ) return true;
        if ( dep > max_search_depth ) return false;
        for (int i=0; i<18; i++) {
            rubiksCube.move( GenericRubicksCube::MOVE(i) );
            moves.push_back( GenericRubicksCube::MOVE(i) );
            if ( dfs(dep+1) ) return true;
            moves.pop_back();
            rubiksCube.invert(GenericRubicksCube::MOVE(i));
        }
        return false;
    }

public:
    T rubiksCube;

    DFSSolver(T _rubiksCube, int _max_search_depth = 0) {
        rubiksCube = _rubiksCube;
        max_search_depth = _max_search_depth;
    }

    vector<GenericRubicksCube::MOVE> solve() {
        moves.clear();
        dfs(0);
        return moves;
    }
};


#endif //RUBIK_S_CUBE_DFSSOLVER_H
