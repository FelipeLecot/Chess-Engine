#include "evaluation.h"
#include "board.h"

// https://www.chessprogramming.org/Piece-Square_Tables

int PIECE_HEAD_VALUES[] = {
    100,
    320,
    330,
    500,
    900,
    2000,
    -100,
    -320,
    -330,
    -500,
    -900,
    -2000};

int PAWN_W_POS[] = {
    0, 0, 0, 0, 0, 0, 0, 0,
    5, 10, 10, -20, -20, 10, 10, 5,
    5, -5, -10, 0, 0, -10, -5, 5,
    0, 0, 0, 20, 20, 0, 0, 0,
    5, 5, 10, 25, 25, 10, 5, 5,
    10, 10, 20, 30, 30, 20, 10, 10,
    50, 50, 50, 50, 50, 50, 50, 50,
    0, 0, 0, 0, 0, 0, 0, 0};
int KNIGHT_W_POS[] = {
    -50, -40, -30, -30, -30, -30, -40, -50,
    -40, -20, 0, 5, 5, 0, -20, -40,
    -30, 5, 10, 15, 15, 10, 5, -30,
    -30, 0, 15, 20, 20, 15, 0, -30,
    -30, 5, 15, 20, 20, 15, 5, -30,
    -30, 0, 10, 15, 15, 10, 0, -30,
    -40, -20, 0, 0, 0, 0, -20, -40,
    -50, -40, -30, -30, -30, -30, -40, -50};
int BISHOP_W_POS[] = {

    -20, -10, -10, -10, -10, -10, -10, -20,
    -10, 5, 0, 0, 0, 0, 5, -10,
    -10, 10, 10, 10, 10, 10, 10, -10,
    -10, 0, 10, 10, 10, 10, 0, -10,
    -10, 5, 5, 10, 10, 5, 5, -10,
    -10, 0, 5, 10, 10, 5, 0, -10,
    -10, 0, 0, 0, 0, 0, 0, -10,
    -20, -10, -10, -10, -10, -10, -10, -20};
int ROOK_W_POS[] = {
    0,
    0,
    5,
    10,
    10,
    5,
    0,
    0,
    -5,
    0,
    0,
    0,
    0,
    0,
    0,
    -5,
    -5,
    0,
    0,
    0,
    0,
    0,
    0,
    -5,
    -5,
    0,
    0,
    0,
    0,
    0,
    0,
    -5,
    -5,
    0,
    0,
    0,
    0,
    0,
    0,
    -5,
    -5,
    0,
    0,
    0,
    0,
    0,
    0,
    -5,
    5,
    10,
    10,
    10,
    10,
    10,
    10,
    5,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
};
int QUEEN_W_POS[] = {
    -20,
    -10,
    -10,
    -5,
    -5,
    -10,
    -10,
    -20 - 10,
    0,
    5,
    0,
    0,
    0,
    0,
    -10,
    -10,
    5,
    5,
    5,
    5,
    5,
    0,
    -10,
    0,
    0,
    5,
    5,
    5,
    5,
    0,
    -5,
    -5,
    0,
    5,
    5,
    5,
    5,
    0,
    -5,
    -10,
    0,
    5,
    5,
    5,
    5,
    0,
    -10,
    -10,
    0,
    0,
    0,
    0,
    0,
    0,
    -10,
    -20,
    -10,
    -10,
    -5,
    -5,
    -10,
    -10,
    -20,
};
int KING_W_POS[] = {
    20,
    30,
    10,
    0,
    0,
    10,
    30,
    20,
    20,
    20,
    0,
    0,
    0,
    0,
    20,
    20,
    -10,
    -20,
    -20,
    -20,
    -20,
    -20,
    -20,
    -10,
    -20,
    -30,
    -30,
    -40,
    -40,
    -30,
    -30,
    -20,
    -30,
    -40,
    -40,
    -50,
    -50,
    -40,
    -40,
    -30,
    -30,
    -40,
    -40,
    -50,
    -50,
    -40,
    -40,
    -30,
    -30,
    -40,
    -40,
    -50,
    -50,
    -40,
    -40,
    -30,
    -30,
    -40,
    -40,
    -50,
    -50,
    -40,
    -40,
    -30,
};

int KNIGHT_B_POS[64];
int PAWN_B_POS[64];
int BISHOP_B_POS[64];
int ROOK_B_POS[64];
int QUEEN_B_POS[64];
int KING_B_POS[64];

int MIN_EVAL = -1000000;
int MAX_EVAL = 1000000;

void initEvaluation(void)
{
    for (int i = 0; i < 64; i++)
    { // Black evaluation is reverse of white
        PAWN_B_POS[i] = PAWN_W_POS[63 - i];
        KNIGHT_B_POS[i] = KNIGHT_W_POS[63 - i];
        BISHOP_B_POS[i] = BISHOP_W_POS[63 - i];
        ROOK_B_POS[i] = ROOK_W_POS[63 - i];
        QUEEN_B_POS[i] = QUEEN_W_POS[63 - i];
        KING_B_POS[i] = KING_W_POS[63 - i];
    }
}

int evaluate(const Board board, int result)
{
    if (result == DRAW)
        return 0;
    else if (result == WHITE_WIN)
        return MAX_EVAL;
    else if (result == BLACK_WIN)
        return MIN_EVAL;

    int eval = 0;

    /* Create local copies of bitboards for iteration (destructive pop operations) */
    Bitboard whitePawns = board.pawn_w;
    Bitboard whiteKnights = board.knight_w;
    Bitboard whiteBishops = board.bishop_w;
    Bitboard whiteRooks = board.rook_w;
    Bitboard whiteQueens = board.queen_w;
    Bitboard blackPawns = board.pawn_b;
    Bitboard blackKnights = board.knight_b;
    Bitboard blackBishops = board.bishop_b;
    Bitboard blackRooks = board.rook_b;
    Bitboard blackQueens = board.queen_b;

    /* Evaluate white pieces */
    while (whitePawns)
    {
        int sq = __builtin_ctzll(whitePawns);
        eval += PAWN_W_POS[sq];
        eval += PIECE_HEAD_VALUES[PAWN_W];
        whitePawns &= whitePawns - 1;
    }
    while (whiteKnights)
    {
        int sq = __builtin_ctzll(whiteKnights);
        eval += KNIGHT_W_POS[sq];
        eval += PIECE_HEAD_VALUES[KNIGHT_W];
        whiteKnights &= whiteKnights - 1;
    }
    while (whiteBishops)
    {
        int sq = __builtin_ctzll(whiteBishops);
        eval += BISHOP_W_POS[sq];
        eval += PIECE_HEAD_VALUES[BISHOP_W];
        whiteBishops &= whiteBishops - 1;
    }
    while (whiteRooks)
    {
        int sq = __builtin_ctzll(whiteRooks);
        eval += ROOK_W_POS[sq];
        eval += PIECE_HEAD_VALUES[ROOK_W];
        whiteRooks &= whiteRooks - 1;
    }
    while (whiteQueens)
    {
        int sq = __builtin_ctzll(whiteQueens);
        eval += QUEEN_W_POS[sq];
        eval += PIECE_HEAD_VALUES[QUEEN_W];
        whiteQueens &= whiteQueens - 1;
    }
    
    /* Evaluate black pieces */
    while (blackPawns)
    {
        int sq = __builtin_ctzll(blackPawns);
        eval -= PAWN_B_POS[sq];
        eval += PIECE_HEAD_VALUES[PAWN_B];
        blackPawns &= blackPawns - 1;
    }
    while (blackKnights)
    {
        int sq = __builtin_ctzll(blackKnights);
        eval -= KNIGHT_B_POS[sq];
        eval += PIECE_HEAD_VALUES[KNIGHT_B];
        blackKnights &= blackKnights - 1;
    }
    while (blackBishops)
    {
        int sq = __builtin_ctzll(blackBishops);
        eval -= BISHOP_B_POS[sq];
        eval += PIECE_HEAD_VALUES[BISHOP_B];
        blackBishops &= blackBishops - 1;
    }
    while (blackRooks)
    {
        int sq = __builtin_ctzll(blackRooks);
        eval -= ROOK_B_POS[sq];
        eval += PIECE_HEAD_VALUES[ROOK_B];
        blackRooks &= blackRooks - 1;
    }
    while (blackQueens)
    {
        int sq = __builtin_ctzll(blackQueens);
        eval -= QUEEN_B_POS[sq];
        eval += PIECE_HEAD_VALUES[QUEEN_B];
        blackQueens &= blackQueens - 1;
    }

    // King square bonuses
    eval += KING_W_POS[board.whiteKingSq];
    eval -= KING_B_POS[board.blackKingSq];

    return eval;
}