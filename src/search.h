#ifndef SEARCH_H
#define SEARCH_H

#include "typedefs.h"

/* Search context structure to track search statistics and results
 * Passed through recursive search calls to accumulate information
 */
typedef struct {
    int nodesSearched;     /* Total number of nodes evaluated during search */
    Move bestMove;         /* Best move found at root of search tree */
    int maxDepth;          /* Maximum depth limit for current search iteration */
} SearchContext;

/* Perform alpha-beta search to find best move
 * Returns evaluation score for the position from active player's perspective
 */
int performAlphaBetaSearch(Board board, int depthLimit, SearchContext* searchContext);

/* Legacy function name for compatibility */
#define search performAlphaBetaSearch

#endif