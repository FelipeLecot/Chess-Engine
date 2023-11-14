#ifndef TT_H
#define TT_H

#include "typedefs.h"

/* Transposition table entry for storing evaluated positions
 * Enables reuse of search results for positions reached via different move orders
 */
typedef struct {
    Bitboard zobrist;      /* Zobrist hash key for position verification */
    int eval;              /* Position evaluation score in centipawns */
    int nodeType;          /* Node type classification (EXACT, LOWER, UPPER bound) */
    int depth;             /* Search depth at which evaluation was computed */
    Move move;             /* Best move found from this position */
} TTEntry;

/* Node type enumeration for alpha-beta bounds */
enum NODE_TYPE {
    EXACT,  /* Exact evaluation (PV node) */
    LOWER,  /* Lower bound (beta cutoff) */
    UPPER   /* Upper bound (alpha failed) */
};

/* Transposition table and size */
extern TTEntry TT_TABLE[];
extern const int TT_SIZE;

/* Retrieve entry from transposition table */
TTEntry retrieveTranspositionEntry(Bitboard hashKey);

/* Store position evaluation in transposition table */
void storeTranspositionEntry(Board board, int evaluation, Move move, 
                             int depthSearched, int beta, int alpha);

/* Legacy function names for compatibility */
#define getTTEntry retrieveTranspositionEntry
#define addTTEntry storeTranspositionEntry

#endif