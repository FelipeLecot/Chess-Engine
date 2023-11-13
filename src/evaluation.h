#ifndef EVALUATION_H
#define EVALUATION_H

#include "typedefs.h"

/* Bounds for alpha-beta search - represent winning/losing scores */
extern int MAX_EVAL;   /* Maximum evaluation (white winning) */
extern int MIN_EVAL;   /* Minimum evaluation (black winning) */

/* Initialize position evaluation tables (piece-square tables)
 * Must be called once at engine startup
 */
void initEvaluation(void);

/* Evaluate board position from white's perspective
 * Positive scores favor white, negative scores favor black
 * gameResult: current game status (UN_DETERMINED, DRAW, etc.)
 * Returns: evaluation in centipawns
 */
int evaluate(const Board board, int result);

#endif