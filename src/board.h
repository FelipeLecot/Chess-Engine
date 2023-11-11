#ifndef BOARD_H
#define BOARD_H

#include "typedefs.h"
#include "bitboards.h"

/* Bitboard manipulation macros for efficient square operations
 * All macros work on individual squares (0-63) within a bitboard
 */
#define getBit(bitboard, square) (bitboard & SQUARE_BITBOARDS[square])      /* Test if bit is set */
#define popBit(bitboard, square) (bitboard & ~SQUARE_BITBOARDS[square])     /* Clear a bit */
#define setBit(bitboard, square) bitboard | SQUARE_BITBOARDS[square]        /* Set a bit */
#define toggleBit(bitboard, square) (bitboard ^ SQUARE_BITBOARDS[square])   /* Flip a bit */

/* Determine game outcome (checkmate, stalemate, draw, ongoing)
 * legalMoveList: array of all legal moves in current position
 * numMoves: number of legal moves available
 * Returns: UN_DETERMINED, WHITE_WIN, BLACK_WIN, or DRAW
 */
int result(const Board board, const Move pseudoLegal[], int length);

/* Update occupancy bitboards from individual piece bitboards
 * Should be called after any piece movement
 */
void computeOccupancyMasks(Board* board);

/* Display board in ASCII format for debugging
 * Shows piece placement, turn, castling rights, and en passant
 */
void printBoard(const Board board);

/* Execute a move on the board
 * Updates piece positions, game state, and incremental hash
 * move: the move to execute (must be legal)
 */
void pushMove(Board* board, const Move move);

#endif