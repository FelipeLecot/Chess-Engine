#ifndef FEN_H
#define FEN_H

/* Standard chess starting position in Forsyth-Edwards Notation (FEN) */
#define START_FEN "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"

#include "typedefs.h"

/* Parse FEN string and set up board position
 * fenNotation: FEN string describing position
 * Updates board state, piece placement, and game flags
 */
void setFen(Board* board, const char* fen);

/* Calculate all squares attacked by opponent pieces
 * Used for check detection and move legality
 * Returns: bitboard with all attacked squares set
 */
Bitboard computeAttacks(const Board board);

#endif