#include <string.h>
#include <stdbool.h>
#include "search.h"
#include "evaluation.h"
#include "board.h"
#include "movegen.h"
#include "tt.h"
#include "zobrist.h"
#include "evaluatemove.h"

#define min(a, b) (a < b) ? a : b;
#define max(a, b) (a > b) ? a : b;

static int alphabeta(Board board, int depth, int alpha, int beta, SearchContext* ctx);

int search(Board board, int depth, SearchContext* ctx) {
    memset(&ctx->bestMove, 0, sizeof(Move));
    ctx->nodesSearched = 0;
    ctx->maxDepth = depth;

    int eval = alphabeta(board, depth, MIN_EVAL, MAX_EVAL, ctx);
    return eval;
}

static int alphabeta(Board board, int depth, int alpha, int beta, SearchContext* ctx) {
    ctx->nodesSearched++;
    int origAlpha = alpha;

    TTEntry entry = getTTEntry(board.hash);
    if (board.hash == entry.zobrist && entry.depth >= depth) {

        if (entry.nodeType == EXACT) {

            if (depth == ctx->maxDepth)
                ctx->bestMove = entry.move;

            return entry.eval;
        } else if (entry.nodeType == LOWER) {
            alpha = max(alpha, entry.eval);
        } else if (entry.nodeType == UPPER) {
            beta = min(beta, entry.eval);
        }

        if (alpha >= beta) {
            if (depth == ctx->maxDepth)
                ctx->bestMove = entry.move;

            return entry.eval;
        }
    }

    Move moves[256];
    int cmoves = legalMoves(&board, moves);

    int res = result(board, moves, cmoves);
    if (res) {
        int eval = evaluate(board, res);

        if (res != DRAW)
            eval += (1 * ctx->maxDepth - depth) * (board.turn ? 1 : -1);

        return eval * (board.turn ? 1 : -1);
    } else if (depth == 0) {
        return evaluate(board, res) * (board.turn ? 1 : -1);
    }

    int nextMove = 0;
    int eval = MIN_EVAL;
    Move best_move;
    memset(&best_move, 0, sizeof(Move));
    score_moves(board, entry, moves, cmoves);

    while (nextMove = select_move(moves, cmoves), nextMove != -1) {
        if (moves[nextMove].validation == LEGAL) {
            Board child = board;
            pushMove(&child, moves[nextMove]);
            int childEval = -alphabeta(child, depth-1, -beta, -alpha, ctx);

            if (childEval > eval) {
                eval = childEval;
                best_move = moves[nextMove];

                if (depth == ctx->maxDepth) 
                    ctx->bestMove = best_move; 
            }

            alpha = max(alpha, childEval);

            if (alpha >= beta)
                break;
        }
    }

    addTTEntry(board, eval, best_move, depth, beta, origAlpha);

    return eval;
}
