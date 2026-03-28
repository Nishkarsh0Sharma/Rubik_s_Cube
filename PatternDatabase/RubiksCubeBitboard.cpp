//
// Created by Nishkarsh Sharma on 10-01-2026.
//

#include "GenericRubicksCube.h"

//
// Bitboard-based Rubik’s Cube implementation.
// Each face is stored as a 64-bit integer.
// Each sticker occupies exactly 8 bits (1 byte).
//
class RubikksCubeBitboard : public GenericRubicksCube {
private:
    uint64_t solved_side_config[6]{}; // 8 sticker(exclude center) * 8 bit
    /*
            8 stickers × 8 bits = 64 bits
            Each sticker stores:
                        00000001 → White
                        00000010 → Green
                        00000100 → Red
                        00001000 → Blue
                        00010000 → Orange
                        00100000 → Yellow
            Exactly one bit set → color identity.
     */

    /*
        Rotating a face = circular shift by 16 bits
        Clockwise rotation moves each sticker 2 positions forward.
     */
    int arr[3][3] = { { 0 , 1 , 2 },
                      { 7 , 8 , 3 },
                      { 6 , 5 , 4 }
                    };
    uint64_t one_8 = ( 1 << 8 ) - 1 , one_24 = ( 1 << 24 ) - 1;

    /*
        Rotating a face clockwise moves each sticker 2 positions forward
        Move by 2 stickers → 2 × 8 = 16 bits
     */
    /*
        Rotate a face CLOCKWISE.

        Face is stored as:
        [ S0 | S1 | S2 | S3 | S4 | S5 | S6 | S7 ]

        Clockwise rotation means:
        each sticker moves forward by 2 positions in the ring.

        That equals a LEFT ROTATION by 16 bits (2 stickers × 8 bits).
     */
    void rotateFace(int ind) {
        uint64_t side = bitboard[ind]; // Copy face : side = [ S0 | S1 | S2 | S3 | S4 | S5 | S6 | S7 ]

        /* 64 - 48 = 16
            Shifts right by 48 bits: side = [ S6 | S7 ]
            These two will wrap around after rotation
         */
        side = side >> (8*6); // Extract last 2 sticker

        /*
        Rotate left by 16 bits
        [S0 S1 S2 S3 S4 S5 S6 S7] << 16
        → [S2 S3 S4 S5 S6 S7  0  0]

        OR with side:
        → [S2 S3 S4 S5 S6 S7 S0 S1]
         */
        bitboard[ind] = (bitboard[ind] << 16) | side;
    }

    // EDGE STRIP TRANSFER
    /*
        rotateSide is the CORE helper for all moves.

        It copies 3 stickers from one face (s2)
        into 3 positions of another face (s1).

        This function DOES NOT rotate.
        It only moves sticker values safely.
     */
    void rotateSide(int s1, int s1_1, int s1_2, int s1_3, int s2, int s2_1, int s2_2, int s2_3) {
        // Extract 3 stickers from source face
        /*
        For each sticker:
                    one_8 << (8 * s2_k)
                    creates a mask for sticker s2_k
                    bitboard[s2] & mask
                    isolates that sticker’s 8 bits
                    >> (8 * s2_k)
                    shifts it down to bits [0..7]
         */

        /*
            void rotateSideArray(
                char face1[8], int a1, int a2, int a3,
                char face2[8], int b1, int b2, int b3
            ) {
                char t1 = face2[b1];
                char t2 = face2[b2];
                char t3 = face2[b3];

                face1[a1] = t1;
                face1[a2] = t2;
                face1[a3] = t3;
            }
         */
        uint64_t clr1 = (bitboard[s2] & (one_8 << (8 * s2_1))) >> (8 * s2_1);
        uint64_t clr2 = (bitboard[s2] & (one_8 << (8 * s2_2))) >> (8 * s2_2);
        uint64_t clr3 = (bitboard[s2] & (one_8 << (8 * s2_3))) >> (8 * s2_3);

        bitboard[s1] = (bitboard[s1] & ~(one_8 << (8 * s1_1))) | (clr1 << (8 * s1_1));
        bitboard[s1] = (bitboard[s1] & ~(one_8 << (8 * s1_2))) | (clr2 << (8 * s1_2));
        bitboard[s1] = (bitboard[s1] & ~(one_8 << (8 * s1_3))) | (clr3 << (8 * s1_3));
    }

    // helper to getCorners()
    int get5bitCorner(string corner) {
        int ret = 0;
        string actual_str;

        // Encode White / Yellow
        for (auto c: corner) {
            if (c != 'W' && c != 'Y') continue;
            actual_str.push_back(c);
            if (c == 'Y') {
                ret |= (1 << 2);
            }
        }

        // Encode Red / Orange
        for (auto c: corner) {
            if (c != 'R' && c != 'O') continue;
            if (c == 'O') {
                ret |= (1 << 1);
            }
        }

        // Encode Blue / Green
        for (auto c: corner) {
            if (c != 'B' && c != 'G') continue;
            if (c == 'G') {
                ret |= (1 << 0);
            }
        }

        // Orientation bits
        if (corner[1] == actual_str[0]) {
            ret |= (1 << 3);
        } else if (corner[2] == actual_str[0]) {
            ret |= (1 << 4);
        }
        return ret;
    }

//    This function was used for testing / printing

//    void print5bitbin(int a){
//        for(int i=4; i>=0; i--){
//            if(a & (1 << i)) cout << 1;
//            else cout << 0;
//        }
//    }

public:
    /*
        bitboard[face] stores the 8 stickers of that face.
     */
    uint64_t bitboard[6]{};

    /*
        Constructor initializes a SOLVED cube.

        For each face:
          - choose color bit (1 << face)
          - fill all 8 stickers with same color
          - store solved configuration
     */

    RubikksCubeBitboard() {
        for (int side = 0; side < 6; side++) {
            uint64_t clr = 1 << side;
            bitboard[side] = 0;
            for (int faceIdx = 0; faceIdx < 8; faceIdx++) {
                bitboard[side] |= clr << (8 * faceIdx);
            }
            solved_side_config[side] = bitboard[side];
        }
    }

    /*
    Returns color at (face, row, col).

    If index == 8 → center sticker → face color.
    Else:
    - extract sticker byte
    - find which bit is set
    */
    COLOR getColor(FACE face, unsigned row, unsigned col) const override {
        int idx = arr[row][col];
        if (idx == 8) return (COLOR)((int) face);

        uint64_t side = bitboard[(int) face];
        uint64_t color = (side >> (8 * idx)) & one_8;

        int bit_pos = 0;
        while (color != 0) {
            color = color >> 1;
            bit_pos++;
        }
        return (COLOR)(bit_pos - 1);
    }

    void setColor(FACE face, unsigned row, unsigned col, COLOR color) override {
        int idx = arr[row][col];
        if (idx == 8) {
            return;
        }

        uint64_t encoded_color = 1ULL << static_cast<int>(color);
        bitboard[static_cast<int>(face)] =
                (bitboard[static_cast<int>(face)] & ~(one_8 << (8 * idx))) |
                (encoded_color << (8 * idx));
    }

    bool isSolved() const override {
        for (int i = 0; i < 6; i++) {
            if (solved_side_config[i] != bitboard[i]) return false;
        }
        return true;
    }

    /*
        Move implementations follow this pattern:

        1. rotateFace(face)
        2. rotate side strips using rotateSide()
        3. manually insert saved stickers where needed
     */
    GenericRubicksCube &u() override {
        this->rotateFace(0);
        uint64_t temp = bitboard[2] & one_24;
        bitboard[2] = (bitboard[2] & ~one_24) | (bitboard[3] & one_24);
        bitboard[3] = (bitboard[3] & ~one_24) | (bitboard[4] & one_24);
        bitboard[4] = (bitboard[4] & ~one_24) | (bitboard[1] & one_24);
        bitboard[1] = (bitboard[1] & ~one_24) | temp;

        return *this;
    }

    GenericRubicksCube &uPrime() override {
        this->u();
        this->u();
        this->u();

        return *this;
    };

    GenericRubicksCube &u2() override {
        this->u();
        this->u();

        return *this;
    };

    GenericRubicksCube &l() override {
        this->rotateFace(1);
        uint64_t clr1 = (bitboard[2] & (one_8 << (8 * 0))) >> (8 * 0);
        uint64_t clr2 = (bitboard[2] & (one_8 << (8 * 6))) >> (8 * 6);
        uint64_t clr3 = (bitboard[2] & (one_8 << (8 * 7))) >> (8 * 7);

        this->rotateSide(2, 0, 7, 6, 0, 0, 7, 6);
        this->rotateSide(0, 0, 7, 6, 4, 4, 3, 2);
        this->rotateSide(4, 4, 3, 2, 5, 0, 7, 6);

        bitboard[5] = (bitboard[5] & ~(one_8 << (8 * 0))) | (clr1 << (8 * 0));
        bitboard[5] = (bitboard[5] & ~(one_8 << (8 * 6))) | (clr2 << (8 * 6));
        bitboard[5] = (bitboard[5] & ~(one_8 << (8 * 7))) | (clr3 << (8 * 7));

        return *this;

    };

    GenericRubicksCube &lPrime() override {
        this->l();
        this->l();
        this->l();

        return *this;
    };

    GenericRubicksCube &l2() override {
        this->l();
        this->l();

        return *this;
    };

    GenericRubicksCube &f() override {
        this->rotateFace(2);

        uint64_t clr1 = (bitboard[0] & (one_8 << (8 * 4))) >> (8 * 4);
        uint64_t clr2 = (bitboard[0] & (one_8 << (8 * 5))) >> (8 * 5);
        uint64_t clr3 = (bitboard[0] & (one_8 << (8 * 6))) >> (8 * 6);

        this->rotateSide(0, 4, 5, 6, 1, 2, 3, 4);
        this->rotateSide(1, 2, 3, 4, 5, 0, 1, 2);
        this->rotateSide(5, 0, 1, 2, 3, 6, 7, 0);

        bitboard[3] = (bitboard[3] & ~(one_8 << (8 * 6))) | (clr1 << (8 * 6));
        bitboard[3] = (bitboard[3] & ~(one_8 << (8 * 7))) | (clr2 << (8 * 7));
        bitboard[3] = (bitboard[3] & ~(one_8 << (8 * 0))) | (clr3 << (8 * 0));

        return *this;
    };

    GenericRubicksCube &fPrime() override {
        this->f();
        this->f();
        this->f();
        return *this;
    };

    GenericRubicksCube &f2() override {
        this->f();
        this->f();

        return *this;
    };

    GenericRubicksCube &r() override {
        this->rotateFace(3);
        uint64_t clr1 = (bitboard[0] & (one_8 << (8 * 2))) >> (8 * 2);
        uint64_t clr2 = (bitboard[0] & (one_8 << (8 * 3))) >> (8 * 3);
        uint64_t clr3 = (bitboard[0] & (one_8 << (8 * 4))) >> (8 * 4);

        this->rotateSide(0, 2, 3, 4, 2, 2, 3, 4);
        this->rotateSide(2, 2, 3, 4, 5, 2, 3, 4);
        this->rotateSide(5, 2, 3, 4, 4, 7, 6, 0);

        bitboard[4] = (bitboard[4] & ~(one_8 << (8 * 7))) | (clr1 << (8 * 7));
        bitboard[4] = (bitboard[4] & ~(one_8 << (8 * 6))) | (clr2 << (8 * 6));
        bitboard[4] = (bitboard[4] & ~(one_8 << (8 * 0))) | (clr3 << (8 * 0));

        return *this;
    };

    GenericRubicksCube &rPrime() override {
        this->r();
        this->r();
        this->r();

        return *this;
    };

    GenericRubicksCube &r2() override {
        this->r();
        this->r();

        return *this;
    };

    GenericRubicksCube &b() override {
        this->rotateFace(4);

        uint64_t clr1 = (bitboard[0] & (one_8 << (8 * 0))) >> (8 * 0);
        uint64_t clr2 = (bitboard[0] & (one_8 << (8 * 1))) >> (8 * 1);
        uint64_t clr3 = (bitboard[0] & (one_8 << (8 * 2))) >> (8 * 2);

        this->rotateSide(0, 0, 1, 2, 3, 2, 3, 4);
        this->rotateSide(3, 2, 3, 4, 5, 4, 5, 6);
        this->rotateSide(5, 4, 5, 6, 1, 6, 7, 0);

        bitboard[1] = (bitboard[1] & ~(one_8 << (8 * 6))) | (clr1 << (8 * 6));
        bitboard[1] = (bitboard[1] & ~(one_8 << (8 * 7))) | (clr2 << (8 * 7));
        bitboard[1] = (bitboard[1] & ~(one_8 << (8 * 0))) | (clr3 << (8 * 0));

        return *this;
    };

    GenericRubicksCube &bPrime() override {
        this->b();
        this->b();
        this->b();

        return *this;
    };

    GenericRubicksCube &b2() override {
        this->b();
        this->b();

        return *this;
    };

    GenericRubicksCube &d() override {
        this->rotateFace(5);

        uint64_t clr1 = (bitboard[2] & (one_8 << (8 * 4))) >> (8 * 4);
        uint64_t clr2 = (bitboard[2] & (one_8 << (8 * 5))) >> (8 * 5);
        uint64_t clr3 = (bitboard[2] & (one_8 << (8 * 6))) >> (8 * 6);

        this->rotateSide(2, 4, 5, 6, 1, 4, 5, 6);
        this->rotateSide(1, 4, 5, 6, 4, 4, 5, 6);
        this->rotateSide(4, 4, 5, 6, 3, 4, 5, 6);

        bitboard[3] = (bitboard[3] & ~(one_8 << (8 * 4))) | (clr1 << (8 * 4));
        bitboard[3] = (bitboard[3] & ~(one_8 << (8 * 5))) | (clr2 << (8 * 5));
        bitboard[3] = (bitboard[3] & ~(one_8 << (8 * 6))) | (clr3 << (8 * 6));

        return *this;
    };

    GenericRubicksCube &dPrime() override {
        this->d();
        this->d();
        this->d();

        return *this;
    };

    GenericRubicksCube &d2() override {
        this->d();
        this->d();

        return *this;
    }

    /*
        Equality operator: compares all face bitboards.
     */
    bool operator==(const RubikksCubeBitboard &r1) const {
        for (int i = 0; i < 6; i++) {
            if (bitboard[i] != r1.bitboard[i]) return false;
        }
        return true;
    }

    /*
        Assignment operator
     */
    RubikksCubeBitboard &operator=(const RubikksCubeBitboard &r1) {
        for (int i = 0; i < 6; i++) {
            bitboard[i] = r1.bitboard[i];
        }
        return *this;
    }

    /*
        getCorners encodes all 8 corners into a single 40-bit integer
        (8 corners × 5 bits each).

        Used for hashing / heuristic evaluation in solvers.
     */
    uint64_t getCorners() {
        // Each corner is read via getColor(), converted to 5-bit code,
        // then packed into ret.
        // (Corner construction logic omitted here for brevity)
        uint64_t ret = 0;
        string top_front_right = "";
        top_front_right += getColorLetter(getColor(FACE::UP, 2, 2));
        top_front_right += getColorLetter(getColor(FACE::FRONT, 0, 2));
        top_front_right += getColorLetter(getColor(FACE::RIGHT, 0, 0));

        string top_front_left = "";
        top_front_left += getColorLetter(getColor(FACE::UP, 2, 0));
        top_front_left += getColorLetter(getColor(FACE::FRONT, 0, 0));
        top_front_left += getColorLetter(getColor(FACE::LEFT, 0, 2));

        string top_back_left = "";
        top_back_left += getColorLetter(getColor(FACE::UP, 0, 0));
        top_back_left += getColorLetter(getColor(FACE::BACK, 0, 2));
        top_back_left += getColorLetter(getColor(FACE::LEFT, 0, 0));

        string top_back_right = "";
        top_back_right += getColorLetter(getColor(FACE::UP, 0, 2));
        top_back_right += getColorLetter(getColor(FACE::BACK, 0, 0));
        top_back_right += getColorLetter(getColor(FACE::RIGHT, 0, 2));

        string bottom_front_right = "";
        bottom_front_right += getColorLetter(getColor(FACE::DOWN, 0, 2));
        bottom_front_right += getColorLetter(getColor(FACE::FRONT, 2, 2));
        bottom_front_right += getColorLetter(getColor(FACE::RIGHT, 2, 0));

        string bottom_front_left = "";
        bottom_front_left += getColorLetter(getColor(FACE::DOWN, 0, 0));
        bottom_front_left += getColorLetter(getColor(FACE::FRONT, 2, 0));
        bottom_front_left += getColorLetter(getColor(FACE::LEFT, 2, 2));

        string bottom_back_right = "";
        bottom_back_right += getColorLetter(getColor(FACE::DOWN, 2, 2));
        bottom_back_right += getColorLetter(getColor(FACE::BACK, 2, 0));
        bottom_back_right += getColorLetter(getColor(FACE::RIGHT, 2, 2));

        string bottom_back_left = "";
        bottom_back_left += getColorLetter(getColor(FACE::DOWN, 2, 0));
        bottom_back_left += getColorLetter(getColor(FACE::BACK, 2, 2));
        bottom_back_left += getColorLetter(getColor(FACE::LEFT, 2, 0));

        ret |= get5bitCorner(top_front_right);
        ret = ret << 5;

        ret |= get5bitCorner(top_front_left);
        ret = ret << 5;

        ret |= get5bitCorner(top_back_right);
        ret = ret << 5;

        ret |= get5bitCorner(top_back_left);
        ret = ret << 5;

        ret |= get5bitCorner(bottom_front_right);
        ret = ret << 5;

        ret |= get5bitCorner(bottom_front_left);
        ret = ret << 5;

        ret |= get5bitCorner(bottom_back_right);
        ret = ret << 5;

        ret |= get5bitCorner(bottom_back_left);
        ret = ret << 5;

//        Following was used for Testing / Printing

//        cout << top_front_right << " "; print5bitbin(get5bitCorner(top_front_right )); cout  << "\n";
//        cout << top_front_left << " "; print5bitbin(get5bitCorner(top_front_left )); cout << "\n";
//        cout << top_back_right << " "; print5bitbin(get5bitCorner(top_back_right )); cout << "\n";
//        cout << top_back_left  << " "; print5bitbin(get5bitCorner(top_back_left  )); cout << "\n";
//        cout << bottom_front_right  << " "; print5bitbin(get5bitCorner(bottom_front_right  )); cout << "\n";
//        cout << bottom_front_left << " "; print5bitbin(get5bitCorner(bottom_front_left )); cout << "\n";
//        cout << bottom_back_right << " "; print5bitbin(get5bitCorner(bottom_back_right )); cout << "\n";
//        cout << bottom_back_left << " "; print5bitbin(get5bitCorner(bottom_back_left )); cout << "\n";

        return ret;
    }
};

/*
    Custom hash for unordered containers.
    XOR of all face bitboards.
*/
struct HashBitboard {
    size_t operator()(const RubikksCubeBitboard &r1) const {
        uint64_t final_hash = r1.bitboard[0];
        for (int i = 1; i < 6; i++) final_hash ^= r1.bitboard[i];
        return (size_t) final_hash;
    }
};
