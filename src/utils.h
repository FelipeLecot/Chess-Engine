#ifndef UTILS_H
#define UTILS_H

#include "typedefs.h"

/* Display a list of moves in human-readable format */
void displayMoveList(const Move moveArray[], int moveCount);

/* Display a bitboard in visual 8x8 grid format */
void displayBitboardGrid(const Bitboard bitboard);

/* Display a bitboard as binary bit representation */
void displayBitboardBinary(const Bitboard bitboard);

/* Legacy function names for compatibility */
#define printMoves displayMoveList
#define printBitboard displayBitboardGrid
#define printBits displayBitboardBinary

#endif