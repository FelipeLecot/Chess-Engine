#ifndef MAGICS_H
#define MAGICS_H

#include "typedefs.h"

/* Magic numbers for the Magic Bitboard sliding-piece move generation technique
 * These pre-computed values enable efficient attack generation for bishops and rooks
 * using minimal perfect hashing of occupancy patterns
 */

extern Bitboard ROOK_MAGICS[64];    /* Magic multipliers for rook attacks */
extern Bitboard BISHOP_MAGICS[64];  /* Magic multipliers for bishop attacks */

#endif