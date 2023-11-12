#ifndef MOVEGEN_H
#define MOVEGEN_H

#include <stdbool.h>
#include "typedefs.h"
#include "magics.h"

/* Pre-computed attack and movement lookup tables
 * Generated once at startup for efficient move generation
 */
extern Bitboard PAWN_START_WHITE;          /* Starting rank for white pawns */
extern Bitboard PAWN_START_BLACK;          /* Starting rank for black pawns */
extern Bitboard PAWN_W_ATTACKS_EAST[64];   /* White pawn east capture attacks */
extern Bitboard PAWN_W_ATTACKS_WEST[64];   /* White pawn west capture attacks */
extern Bitboard PAWN_B_ATTACKS_EAST[64];   /* Black pawn east capture attacks */
extern Bitboard PAWN_B_ATTACKS_WEST[64];   /* Black pawn west capture attacks */
extern Bitboard KNIGHT_MOVEMENT[64];        /* Knight movement from each square */
extern Bitboard BISHOP_MOVEMENT[64];        /* Bishop movement masks */
extern Bitboard ROOK_MOVEMENT[64];          /* Rook movement masks */
extern Bitboard KING_MOVEMENT[64];          /* King movement from each square */

/* Initialize all move generation lookup tables
 * Must be called once at engine startup
 */
void initMoveGeneration(void);

/* Generate all legal moves in current position
 * Filters pseudo-legal moves to exclude those leaving king in check
 * moveList: output array to store generated moves
 * Returns: number of legal moves generated
 */
int legalMoves(Board* board, Move moves[]);

/* Extract lowest set bit from bitboard (destructive)
 * Used for iterating through piece positions
 * bitboard: pointer to bitboard (modified - lowest bit cleared)
 * Returns: index (0-63) of the lowest set bit
 */
int bitScanForward(Bitboard* bb);

/* Check if a square is under attack by opponent
 * squareIndex: square to check (0-63)
 * Returns: true if square is attacked by opponent pieces
 */
bool isSquareAttacked(const Board board, int square);

/* Calculate rook attack bitboard using magic bitboards
 * squareIndex: rook position (0-63)
 * occupancyBitboard: occupied squares on board
 * Returns: bitboard of all squares rook can attack
 */
Bitboard getRookAttacks(int square, Bitboard occupancy);

/* Calculate bishop attack bitboard using magic bitboards
 * squareIndex: bishop position (0-63)
 * occupancyBitboard: occupied squares on board
 * Returns: bitboard of all squares bishop can attack
 */
Bitboard getBishopAttacks(int square, Bitboard occupancy);

#endif