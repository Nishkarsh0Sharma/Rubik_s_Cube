//
// Created by Nishkarsh Sharma on 10-01-2026.
//

#include "GenericRubicksCube.h"


/*
cube[FACE][ROW][COL]
cube[6][3][3]

Face Indexing :
                0 → UP
                1 → LEFT
                2 → FRONT
                3 → RIGHT
                4 → BACK
                5 → DOWN
 */
class RubiksCube3dArray : public GenericRubicksCube {
private:
    /*
        It rotates only the 3×3 face itself clockwise.
        Copy face : temp_arr[i][j] = cube[ind][i][j]; Because: You cannot rotate in-place safely. You’ll overwrite needed values.

        (0,0) ← (2,0)
        (0,1) ← (1,0)
        (0,2) ← (0,0)
        ...
        is the standard 90° clockwise rotation.
        a b c        g d a
        d e f   →    h e b
        g h i        i f c
     */
    void rotateFace(int ind) {
        char temp_arr[3][3] = {};
        for (int i=0; i<3; i++) {
            for (int j=0; j<3; j++) {
                temp_arr[i][j] = cube[ind][i][j];
            }
        }

        /*
            top row :
                Left column( [0][i] ) → top row ( [2-i][0] )
                (0,0) ← (2,0)
                (0,1) ← (1,0)
                (0,2) ← (0,0)
         */
        for (int i=0; i<3; i++) cube[ind][0][i] = temp_arr[2-i][0];

        /*
            right col :
                cube[ind][i][2] = temp_arr[0][i];
                (0,2) ← (0,0)
                (1,2) ← (0,1)
                (2,2) ← (0,2)
                ✔ Top row → right column
         */
        for (int i=0; i<3; i++) cube[ind][i][2] = temp_arr[0][i];

        /*
            bottom row :
                cube[ind][2][2-i] = temp_arr[i][2];
                (2,0) ← (2,2)
                (2,1) ← (1,2)
                (2,2) ← (0,2)
                ✔ right col → bottom row
         */
        for (int i=0; i<3; i++) cube[ind][2][2-i] = temp_arr[i][2];

        /*
            left col :
                cube[ind][2-i][0] = temp_arr[2][2-i];
                (2,0) ← (2,2)
                (1,0) ← (2,1)
                (0,0) ← (2,0)
                ✔ bottom row → left col
         */
        for (int i=0; i<3; i++) cube[ind][2-i][0] = temp_arr[2][2-i];
    }

public:
    char cube[6][3][3];

    RubiksCube3dArray() {
        for (int i=0; i<6; i++) { // faces
            for (int j=0; j<3; j++) { // row
                for (int k=0; k<3; k++) { // col
                    cube[i][j][k] = getColorLetter(COLOR(i));
                }
            }
        }
    }

    COLOR getColor(FACE face, unsigned row, unsigned col) const override {
        char color = cube[int(face)][row][col];
        switch (color) {
            case 'B':
                return COLOR::BLUE;
            case 'R':
                return COLOR::RED;
            case 'G':
                return COLOR::GREEN;
            case 'O':
                return COLOR::ORANGE;
            case 'Y':
                return COLOR::YELLOW;
            default:
                return COLOR::WHITE;
        }
    }

    void setColor(FACE face, unsigned row, unsigned col, COLOR color) override {
        cube[int(face)][row][col] = getColorLetter(color);
    }

    // Solved cube = each face is uniform
    bool isSolved() const override {
        for (int i=0; i<6; i++) { // faces
            for (int j=0; j<3; j++) { // row
                for (int k=0; k<3; k++) { // col
                    if ( this->cube[i][j][k] == getColorLetter(COLOR(i)) ) continue;
                    return false;
                }
            }
        }
    }


    /*
        "MOVE IMPLEMENTATIONS"
        Each move has two parts:
                1. Rotate the face itself
                2. Rotate the adjacent edge strips
     */


    GenericRubicksCube &u() override {
        // Step 1: Rotate UP face
        this->rotateFace(0);

        // Step 2: Rotate surrounding faces
        // Affected faces: BACK → LEFT → FRONT → RIGHT → BACK
        char temp_arr[3] = {};

        /*
            BACK face (4)
            Top row (row = 0)
            Read right → left
            Why reverse ? Back's left-right direction FRONT's Opposite
         */
        // back copy
        for (int i = 0; i < 3; i++) temp_arr[i] = cube[4][0][2 - i];

        // BACK → LEFT → FRONT → RIGHT → BACK
        for (int i = 0; i < 3; i++) cube[4][0][2 - i] = cube[1][0][2 - i]; // back = left
        for (int i = 0; i < 3; i++) cube[1][0][2 - i] = cube[2][0][2 - i]; // left = front
        for (int i = 0; i < 3; i++) cube[2][0][2 - i] = cube[3][0][2 - i]; // front = right
        for (int i = 0; i < 3; i++) cube[3][0][2 - i] = temp_arr[i]; // right = back

        return *this;
    }

    GenericRubicksCube &uPrime() override {
        this->u();
        this->u();
        this->u();

        return *this;
    }

    GenericRubicksCube &u2() override {
        this->u();
        this->u();

        return *this;
    }

    // MOVE: L (Left face clockwise)
    GenericRubicksCube &l() override {
        this->rotateFace(1);
        /*
            UP → BACK → DOWN → FRONT → UP
         */
        char temp_arr[3] = {};

        for (int i = 0; i < 3; i++) temp_arr[i] = cube[0][i][0]; // copy up's 1st col

        for (int i = 0; i < 3; i++) cube[0][i][0] = cube[4][2 - i][2]; // in up : back's 2nd col in reversed
        for (int i = 0; i < 3; i++) cube[4][2 - i][2] = cube[5][i][0]; // in back : down's 1st col
        for (int i = 0; i < 3; i++) cube[5][i][0] = cube[2][i][0]; // in down : front's 1st col
        for (int i = 0; i < 3; i++) cube[2][i][0] = temp_arr[i]; // in front : up's 1st col

        return *this;
    }

    GenericRubicksCube &lPrime() override {
        this->l();
        this->l();
        this->l();

        return *this;
    }

    GenericRubicksCube &l2() override {
        this->l();
        this->l();

        return *this;
    }

    // MOVE: F (Front face clockwise)
    GenericRubicksCube &f() override {
        this->rotateFace(2);

        char temp_arr[3] = {};
        for (int i = 0; i < 3; i++) temp_arr[i] = cube[0][2][i];
        // UP → LEFT → DOWN → RIGHT → UP
        for (int i = 0; i < 3; i++) cube[0][2][i] = cube[1][2 - i][2];
        for (int i = 0; i < 3; i++) cube[1][2 - i][2] = cube[5][0][2 - i];
        for (int i = 0; i < 3; i++) cube[5][0][2 - i] = cube[3][i][0];
        for (int i = 0; i < 3; i++) cube[3][i][0] = temp_arr[i];

        return *this;
    }

    GenericRubicksCube &fPrime() override {
        this->f();
        this->f();
        this->f();

        return *this;
    }

    GenericRubicksCube &f2() override {
        this->f();
        this->f();

        return *this;
    }

    // MOVE: R (Right face clockwise)
    GenericRubicksCube &r() override {
        this->rotateFace(3);

        char temp_arr[3] = {};
        for (int i = 0; i < 3; i++) temp_arr[i] = cube[0][2 - i][2];
        // UP → FRONT → DOWN → BACK → UP
        for (int i = 0; i < 3; i++) cube[0][2 - i][2] = cube[2][2 - i][2];
        for (int i = 0; i < 3; i++) cube[2][2 - i][2] = cube[5][2 - i][2];
        for (int i = 0; i < 3; i++) cube[5][2 - i][2] = cube[4][i][0];
        for (int i = 0; i < 3; i++) cube[4][i][0] = temp_arr[i];

        return *this;
    }

    GenericRubicksCube &rPrime() override {
        this->r();
        this->r();
        this->r();

        return *this;
    }

    GenericRubicksCube &r2() override {
        this->r();
        this->r();

        return *this;
    }

    // MOVE: B (Back face clockwise)
    GenericRubicksCube &b() override {
        this->rotateFace(4);

        char temp_arr[3] = {};
        for (int i = 0; i < 3; i++) temp_arr[i] = cube[0][0][2 - i];

        // UP → RIGHT → DOWN → LEFT → UP
        for (int i = 0; i < 3; i++) cube[0][0][2 - i] = cube[3][2 - i][2];
        for (int i = 0; i < 3; i++) cube[3][2 - i][2] = cube[5][2][i];
        for (int i = 0; i < 3; i++) cube[5][2][i] = cube[1][i][0];
        for (int i = 0; i < 3; i++) cube[1][i][0] = temp_arr[i];

        return *this;
    }

    GenericRubicksCube &bPrime() override {
        this->b();
        this->b();
        this->b();

        return *this;
    }

    GenericRubicksCube &b2() override {
        this->b();
        this->b();

        return *this;
    }

    // MOVE: D (Down face clockwise)
    GenericRubicksCube &d() override {
        this->rotateFace(5);

        char temp_arr[3] = {};
        for (int i = 0; i < 3; i++) temp_arr[i] = cube[2][2][i];

        // FRONT → LEFT → BACK → RIGHT → FRONT
        for (int i = 0; i < 3; i++) cube[2][2][i] = cube[1][2][i];
        for (int i = 0; i < 3; i++) cube[1][2][i] = cube[4][2][i];
        for (int i = 0; i < 3; i++) cube[4][2][i] = cube[3][2][i];
        for (int i = 0; i < 3; i++) cube[3][2][i] = temp_arr[i];

        return *this;
    }

    GenericRubicksCube &dPrime() override {
        this->d();
        this->d();
        this->d();

        return *this;
    }

    GenericRubicksCube &d2() override {
        this->d();
        this->d();

        return *this;
    }

    /*
                       Question	                          Why
            Are these two states the same?	         Avoid revisiting
            Can I copy a state safely?	                Branching
            Can I store states in a hash table?	       Performance

            These three questions map exactly to:
                                                    operator==
                                                    operator=
                                                    Hash3d


        PART 1: operator== (Equality)
                    bool operator==(const RubiksCube3dArray &r1) const
                    🔹 First principle

                        Two cube states are equal if and only if every sticker in the same position has the same color.

                    There is no shortcut.
                    You must compare all 54 stickers.

     */

    bool operator==(const RubiksCube3dArray &r1) const {
        for (int i=0; i<6; i++) {
            for (int j=0; j<3; j++) {
                for (int k=0; k<3; k++) {
                    if ( r1.cube[i][j][k] != cube[i][j][k] ) return false;
                }
            }
        }
        return true;
    }


    /*
    PART 2: operator= (Assignment) Deep Copy
                    RubiksCube3dArray &operator=(const RubiksCube3dArray &r1)
                    🔹 First principle

                    When exploring a graph, you constantly copy states.
                    Example in DFS:
                        RubiksCube3dArray next = current;
                        next.move(F);

                    If copying is wrong → solver corrupts states.
     */
    RubiksCube3dArray &operator=(const RubiksCube3dArray &r1) {
        for (int i=0; i<6; i++) {
            for (int j=0; j<3; j++) {
                for (int k=0; k<3; k++) {
                    cube[i][j][k] = r1.cube[i][j][k];
                }
            }
        }
    }

};

/*
    PART 3: Hash3d (Hash Function)
                        struct Hash3d {
                            size_t operator()(const RubiksCube3dArray &r1) const

                        🔹 First principle
                        Hashing allows O(1) average lookup in visited sets.

                        In BFS / DFS:
                        unordered_map<Cube, bool>

                        Without hashing:
                        You must linearly scan all visited states
                        That’s impossible for millions of states
 */

struct Hash3d {
    size_t operator()(const RubiksCube3dArray &r1) const {
        string str = "";

        for (int i=0; i<6; i++) {
            for (int j=0; j<3; j++) {
                for (int k=0; k<3; k++) {
                    str += r1.cube[i][j][k];
                }
            }
        }

        return hash<string>()(str);
        // C++ provides: High-quality string hashing. Well-distributed hash values.
    }
};