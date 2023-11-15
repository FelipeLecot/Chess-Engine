#ifndef MOVEORDERER_H
#define MOVEORDERER_H

#include "typedefs.h"
#include "tt.h"

/* Assign heuristic scores to moves for better move ordering
 * Good move ordering improves alpha-beta pruning efficiency
 */
void assignMoveOrderingScores(const Board board, const TTEntry ttEntry, 
                               Move moveList[], int moveCount);

/* Select next best move from list based on scores (selection sort)
 * Returns index of selected move, or -1 if all moves exhausted
 */
int selectNextBestMove(Move moveList[], int moveCount);

/* Legacy function names for compatibility */
#define score_moves assignMoveOrderingScores
#define select_move selectNextBestMove

#endif
