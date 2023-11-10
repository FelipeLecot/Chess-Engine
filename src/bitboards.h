#ifndef BITBOARDS_H
#define BITBOARDS_H

#include "typedefs.h"

/* Pre-computed bitboard constants - each contains a single set bit */
extern Bitboard SQUARE_BITBOARDS[64];   /* Individual square bitboards (0-63) */

/* Rank bitboard masks for pawn move generation */
extern Bitboard RANK_1;   /* First rank (white back rank) */
extern Bitboard RANK_7;   /* Seventh rank (black pawn start rank) */

/* Initialize all bitboard lookup tables and constants
 * Must be called before using move generation
 */
void initBitboards(void);

#endif