//
// Created by Nishkarsh Sharma on 10-01-2026.
//

#include "GenericRubicksCube.h"

/*
    A Rubik’s Cube has:
        6 faces × 3 rows × 3 cols = 54 stickers
        Instead of: cube[6][3][3]
        we store: cube[54]

    But then we must answer: ❓ How do I map (face, row, col) → index?
    First-principle reasoning
        Each face has 9 stickers
        Each row has 3 stickers
            So:
            Face offset  = ind × 9
            Row offset   = row × 3
            Column offset = col
            static inline int getIndex(int ind, int row, int col) {
                return (ind * 9) + (row * 3) + col;
            }

 */

class RubiksCube1dArray : public GenericRubicksCube {
private:

    static inline int getIndex(int ind, int row, int col) {
        return (ind * 9) + (row * 3) + col;
    }

    void rotateFace(int ind) {
        char temp_arr[9] = {};

        for (int i=0; i<3; i++) {
            for (int j=0; j<3; j++) {
                temp_arr[i*3+j] = cube[getIndex(ind,i,j)];
            }
        }

        /*
            TOP row  ← LEFT column (bottom → top)
                    Mapping :
                        Copy code
                        (0,0) ← (2,0)
                        (0,1) ← (1,0)
                        (0,2) ← (0,0)
         */
        for (int i=0; i<3; i++) cube[getIndex(ind,0,i)] = temp_arr[getIndex(0,2-i,0)];

        /*
            RIGHT column ← TOP row
                    Mapping
                        Copy code
                        (0,2) ← (0,0)
                        (1,2) ← (0,1)
                        (2,2) ← (0,2)
         */
        for (int i=0; i<3; i++) cube[getIndex(ind,i,2)] = temp_arr[getIndex(0,0,i)];


        /*
            BOTTOM row ← RIGHT column (top → bottom)
                    Mapping
                        Copy code
                        (2,2) ← (0,2)
                        (2,1) ← (1,2)
                        (2,0) ← (2,2)
         */
        for (int i=0; i<3; i++) cube[getIndex(ind,2,2-i)] = temp_arr[getIndex(0,i,2)];

        /*
            LEFT column ← BOTTOM row (right → left)
                    Mapping
                        Copy code
                        (2,0) ← (2,2)
                        (1,0) ← (2,1)
                        (0,0) ← (2,0)
         */
        for (int i=0; i<3; i++) cube[getIndex(ind,2-i,0)] = temp_arr[getIndex(0,2,2-i)];
    }

public:
    char cube[54] = {};

    RubiksCube1dArray() {
        for (int i=0; i<6; i++) {
            for (int j=0; j<3; j++) {
                for (int k=0; k<3; k++) {
                    cube[ i*9 + j*3 + k ] = getColorLetter(COLOR(i));
                }
            }
        }
    }

    COLOR getColor(FACE face, unsigned row, unsigned col) const override {
        char color = cube[getIndex((int)face, (int)row, (int)col)];
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
        cube[getIndex((int)face, (int)row, (int)col)] = getColorLetter(color);
    }

    bool isSolved() const override {
        for (int i = 0; i < 6; i++) {
            for (int j = 0; j < 3; j++) {
                for (int k = 0; k < 3; k++) {
                    if (this->cube[getIndex(i, j, k)] == getColorLetter(COLOR(i))) continue; // == face_color
                    return false;
                }
            }
        }
        return true;
    }

    GenericRubicksCube &u() override {
        this->rotateFace(0);

        char temp_arr[3] = {};
        for (int i = 0; i < 3; i++) temp_arr[i] = cube[getIndex(4, 0, 2 - i)];

        // BACK → LEFT → FRONT → RIGHT → BACK
        for (int i = 0; i < 3; i++) cube[getIndex(4, 0, 2 - i)] = cube[getIndex(1, 0, 2 - i)];
        for (int i = 0; i < 3; i++) cube[getIndex(1, 0, 2 - i)] = cube[getIndex(2, 0, 2 - i)];
        for (int i = 0; i < 3; i++) cube[getIndex(2, 0, 2 - i)] = cube[getIndex(3, 0, 2 - i)];
        for (int i = 0; i < 3; i++) cube[getIndex(3, 0, 2 - i)] = temp_arr[i];

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

    GenericRubicksCube &l() override {
        this->rotateFace(1);

        char temp_arr[3] = {};
        for (int i = 0; i < 3; i++) temp_arr[i] = cube[getIndex(0, i, 0)];

        // UP → BACK → DOWN → FRONT → UP
        for (int i = 0; i < 3; i++) cube[getIndex(0, i, 0)] = cube[getIndex(4, 2 - i, 2)];
        for (int i = 0; i < 3; i++) cube[getIndex(4, 2 - i, 2)] = cube[getIndex(5, i, 0)];
        for (int i = 0; i < 3; i++) cube[getIndex(5, i, 0)] = cube[getIndex(2, i, 0)];
        for (int i = 0; i < 3; i++) cube[getIndex(2, i, 0)] = temp_arr[i];

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

    GenericRubicksCube &f() override {
        this->rotateFace(2);

        char temp_arr[3] = {};
        for (int i = 0; i < 3; i++) temp_arr[i] = cube[getIndex(0, 2, i)];

        // UP → LEFT → DOWN → RIGHT → UP
        for (int i = 0; i < 3; i++) cube[getIndex(0, 2, i)] = cube[getIndex(1, 2 - i, 2)];
        for (int i = 0; i < 3; i++) cube[getIndex(1, 2 - i, 2)] = cube[getIndex(5, 0, 2 - i)];
        for (int i = 0; i < 3; i++) cube[getIndex(5, 0, 2 - i)] = cube[getIndex(3, i, 0)];
        for (int i = 0; i < 3; i++) cube[getIndex(3, i, 0)] = temp_arr[i];

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

    GenericRubicksCube &r() override {
        this->rotateFace(3);

        char temp_arr[3] = {};
        for (int i = 0; i < 3; i++) temp_arr[i] = cube[getIndex(0, 2 - i, 2)];
        for (int i = 0; i < 3; i++) cube[getIndex(0, 2 - i, 2)] = cube[getIndex(2, 2 - i, 2)];

        // UP → FRONT → DOWN → BACK → UP
        for (int i = 0; i < 3; i++) cube[getIndex(2, 2 - i, 2)] = cube[getIndex(5, 2 - i, 2)];
        for (int i = 0; i < 3; i++) cube[getIndex(5, 2 - i, 2)] = cube[getIndex(4, i, 0)];
        for (int i = 0; i < 3; i++) cube[getIndex(4, i, 0)] = temp_arr[i];

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

    GenericRubicksCube &b() override {
        this->rotateFace(4);

        char temp_arr[3] = {};
        for (int i = 0; i < 3; i++) temp_arr[i] = cube[getIndex(0, 0, 2 - i)];

        // UP → RIGHT → DOWN → LEFT → UP
        for (int i = 0; i < 3; i++) cube[getIndex(0, 0, 2 - i)] = cube[getIndex(3, 2 - i, 2)];
        for (int i = 0; i < 3; i++) cube[getIndex(3, 2 - i, 2)] = cube[getIndex(5, 2, i)];
        for (int i = 0; i < 3; i++) cube[getIndex(5, 2, i)] = cube[getIndex(1, i, 0)];
        for (int i = 0; i < 3; i++) cube[getIndex(1, i, 0)] = temp_arr[i];

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

    GenericRubicksCube &d() override {
        this->rotateFace(5);

        char temp_arr[3] = {};
        for (int i = 0; i < 3; i++) temp_arr[i] = cube[getIndex(2, 2, i)];
        for (int i = 0; i < 3; i++) cube[getIndex(2, 2, i)] = cube[getIndex(1, 2, i)];

        // FRONT → LEFT → BACK → RIGHT → FRONT
        for (int i = 0; i < 3; i++) cube[getIndex(1, 2, i)] = cube[getIndex(4, 2, i)];
        for (int i = 0; i < 3; i++) cube[getIndex(4, 2, i)] = cube[getIndex(3, 2, i)];
        for (int i = 0; i < 3; i++) cube[getIndex(3, 2, i)] = temp_arr[i];

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

    // PART 1: operator== (Equality)
    bool operator==(const RubiksCube1dArray &r1) const {
        for (int i = 0; i < 54; i++) {
            if (cube[i] != r1.cube[i]) return false;
        }
        return true;
    }

    // PART 2: operator= (Assignment) Deep Copy
    RubiksCube1dArray &operator=(const RubiksCube1dArray &r1) {
        for (int i = 0; i < 54; i++) {
            cube[i] = r1.cube[i];
        }
        return *this;
    }
};

struct Hash1d {
    size_t operator()(const RubiksCube1dArray &r1) const {
        string str = "";
        for (int i = 0; i < 54; i++) str += r1.cube[i];
        return hash<string>()(str);
    }
};
