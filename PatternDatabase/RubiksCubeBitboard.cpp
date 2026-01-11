//
// Created by Nishkarsh Sharma on 10-01-2026.
//

#include "GenericRubicksCube.h"

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
        uint64_t clr1 = (bitboard[s2] & (one_8 << (8 * s2_1))) >> (8 * s2_1);
        uint64_t clr2 = (bitboard[s2] & (one_8 << (8 * s2_2))) >> (8 * s2_2);
        uint64_t clr3 = (bitboard[s2] & (one_8 << (8 * s2_3))) >> (8 * s2_3);

        bitboard[s1] = (bitboard[s1] & ~(one_8 << (8 * s1_1))) | (clr1 << (8 * s1_1));
        bitboard[s1] = (bitboard[s1] & ~(one_8 << (8 * s1_2))) | (clr2 << (8 * s1_2));
        bitboard[s1] = (bitboard[s1] & ~(one_8 << (8 * s1_3))) | (clr3 << (8 * s1_3));
    }

    // helper to getCorners()
    itn get5bitCorner(string corner) {
        int ret = 0;
        string actual_str;
        for (auto c: corner) {
            if ( c != 'W' &&  c != 'Y' ) continue;
            actual_str.push_back(c);
            if ( c == 'Y' ) {
                ret |= (1<<2);
            }
        }
    }
};