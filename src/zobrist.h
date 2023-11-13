#ifndef ZOBRIST_H
#define ZOBRIST_H

#include "typedefs.h"

/* Zobrist hash tables for incremental position hashing
 * These random bitstrings are XORed together to create unique position hashes
 */
extern Bitboard PIECES[12][64];        /* Hash values for [piece_type][square] */
extern Bitboard EN_PASSANT[64];        /* Hash values for en passant squares */
extern Bitboard CASTLING[16];          /* Hash values for castling rights 0-15 */
extern Bitboard WHITE_TO_MOVE;         /* Hash toggle for side to move */

/* Initialize Zobrist hash tables with pseudo-random 64-bit values */
void initZobrist(void);

/* Compute full Zobrist hash for a board position
 * Returns a 64-bit hash representing the complete board state
 */
Bitboard hash(const Board board);

#endif