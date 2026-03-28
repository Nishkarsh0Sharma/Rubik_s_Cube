#include "bits/stdc++.h"
#include "PatternDatabase/RubiksCubeBitboard.h"
#include "PatternDatabase/RubiksCube1dArray.h"
#include "PatternDatabase/RubiksCube3dArray.h"
#include "Solver/DFSSolver.h"
#include "Solver/BFSSolver.h"
#include "Solver/IDDFSSolver.h"
#include "Solver/IDAstarSolver.h"
#include "PatternDatabase/Patterns/CornerDBMaker.h"

using namespace std;

namespace {

void printMoves(const vector<GenericRubicksCube::MOVE> &moves) {
    for (const auto move: moves) {
        cout << GenericRubicksCube::getMove(move) << ' ';
    }
    cout << '\n';
}

template <typename Cube>
void printScramble(const string &label, const Cube &cube, const vector<GenericRubicksCube::MOVE> &moves) {
    cout << label << " scramble: ";
    printMoves(moves);
    cube.print();
}

template <typename Cube>
void applyMoves(Cube &cube, const vector<GenericRubicksCube::MOVE> &moves) {
    for (const auto move: moves) {
        cube.move(move);
    }
}

} // namespace

int main() {
    const string corner_db_path = "Databases/cornerDepth5V1.txt";

    // I kept these scrambles fixed on purpose. It makes debugging calmer because every run tells the same story.
    RubiksCubeBitboard bfs_cube;
    const vector<GenericRubicksCube::MOVE> bfs_scramble = {
            GenericRubicksCube::MOVE::D,
            GenericRubicksCube::MOVE::BPRIME,
            GenericRubicksCube::MOVE::F2
    };
    applyMoves(bfs_cube, bfs_scramble);
    printScramble("BFS", bfs_cube, bfs_scramble);
    BFSSolver<RubiksCubeBitboard, HashBitboard> bfs_solver(bfs_cube);
    auto bfs_solution = bfs_solver.solve();
    cout << "BFS solution (" << bfs_solution.size() << " moves): ";
    printMoves(bfs_solution);
    cout << "BFS solved? " << (bfs_solver.rubiksCube.isSolved() ? "yes" : "no") << "\n\n";

    RubiksCube3dArray dfs_cube;
    const vector<GenericRubicksCube::MOVE> dfs_scramble = {
            GenericRubicksCube::MOVE::B,
            GenericRubicksCube::MOVE::UPRIME,
            GenericRubicksCube::MOVE::RPRIME,
            GenericRubicksCube::MOVE::F2
    };
    applyMoves(dfs_cube, dfs_scramble);
    printScramble("DFS", dfs_cube, dfs_scramble);
    DFSSolver<RubiksCube3dArray, Hash3d> dfs_solver(dfs_cube, 7);
    auto dfs_solution = dfs_solver.solve();
    cout << "DFS solution (" << dfs_solution.size() << " moves): ";
    printMoves(dfs_solution);
    cout << "DFS solved? " << (dfs_solver.rubiksCube.isSolved() ? "yes" : "no") << "\n\n";

    RubiksCubeBitboard iddfs_cube;
    const vector<GenericRubicksCube::MOVE> iddfs_scramble = {
            GenericRubicksCube::MOVE::UPRIME,
            GenericRubicksCube::MOVE::F2,
            GenericRubicksCube::MOVE::F,
            GenericRubicksCube::MOVE::DPRIME
    };
    applyMoves(iddfs_cube, iddfs_scramble);
    printScramble("IDDFS", iddfs_cube, iddfs_scramble);
    IDDFSSolver<RubiksCubeBitboard, HashBitboard> iddfs_solver(iddfs_cube, 8);
    auto iddfs_solution = iddfs_solver.solve();
    cout << "IDDFS solution (" << iddfs_solution.size() << " moves): ";
    printMoves(iddfs_solution);
    cout << "IDDFS solved? " << (iddfs_solver.rubiksCube.isSolved() ? "yes" : "no") << "\n\n";

    RubiksCubeBitboard ida_cube;
    const vector<GenericRubicksCube::MOVE> ida_scramble = {
            GenericRubicksCube::MOVE::RPRIME,
            GenericRubicksCube::MOVE::DPRIME,
            GenericRubicksCube::MOVE::BPRIME
    };
    applyMoves(ida_cube, ida_scramble);
    printScramble("IDA*", ida_cube, ida_scramble);
    IDAstarSolver<RubiksCubeBitboard, HashBitboard> ida_solver(ida_cube, corner_db_path);
    auto ida_solution = ida_solver.solve();
    cout << "IDA* solution (" << ida_solution.size() << " moves): ";
    printMoves(ida_solution);
    cout << "IDA* solved? " << (ida_solver.rubiksCube.isSolved() ? "yes" : "no") << "\n";

    return 0;
}
