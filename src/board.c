#include <stdbool.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include "board.h"
#include "movegen.h"
#include "zobrist.h"
#include "utils.h"

#define PAWN_DOUBLE_MOVE_DISTANCE 16

/* Square name strings for algebraic notation */
char SQUARES[64][3] = {
    "h1", "g1", "f1", "e1", "d1", "c1", "b1", "a1",
    "h2", "g2", "f2", "e2", "d2", "c2", "b2", "a2",
    "h3", "g3", "f3", "e3", "d3", "c3", "b3", "a3",
    "h4", "g4", "f4", "e4", "d4", "c4", "b4", "a4",
    "h5", "g5", "f5", "e5", "d5", "c5", "b5", "a5",
    "h6", "g6", "f6", "e6", "d6", "c6", "b6", "a6",
    "h7", "g7", "f7", "e7", "d7", "c7", "b7", "a7",
    "h8", "g8", "f8", "e8", "d8", "c8", "b8", "a8"
};

/* Castling rights notation strings */
char CASTLING_RIGHTS[4][2] = { "K", "Q", "k", "q" };

/* Check if board position has insufficient material for checkmate
 * Returns true for positions like: King vs King, King+Knight vs King, etc.
 */
bool hasInsufficientMatingMaterial(Board board) {
    /* If any pawns, rooks, or queens exist, mate is possible */
    if (board.pawn_w || board.pawn_b || board.rook_w || board.rook_b || 
        board.queen_w || board.queen_b) {
        return false;
    }

    /* Count minor pieces on the board */
    int knightCount = 0;
    int bishopCount = 0;
    Bitboard knights = board.knight_w | board.knight_b;
    Bitboard bishops = board.bishop_w | board.bishop_b;
    
    while (knights) {
        knights &= knights - 1;
        knightCount++;
    }
    while (bishops) {
        bishops &= bishops - 1;
        bishopCount++;
    }

    /* Insufficient material combinations */
    if (!bishopCount && knightCount) return true;       /* KN or KNN vs K */
    if (!knightCount && bishopCount == 1) return true;  /* KB vs K */

    return false;
}

/* Determine the game outcome based on position and available moves
 * Detects checkmate, stalemate, and insufficient material
 */
int result(const Board board, const Move legal[], int length) {
    if (hasInsufficientMatingMaterial(board)) {
        return DRAW;
    }

    bool hasNoLegalMoves = length == 0;

    if (hasNoLegalMoves) {
        /* Check if king is under attack */
        int kingSquare = board.turn ? board.whiteKingSq : board.blackKingSq;
        Bitboard kingUnderAttack = SQUARE_BITBOARDS[kingSquare] & board.attacks;
        
        if (kingUnderAttack) {
            /* Checkmate - side to move loses */
            return board.turn ? BLACK_WIN : WHITE_WIN;
        }

        /* Stalemate - no legal moves but not in check */
        return DRAW;
    }

    /* Game continues */
    return UN_DETERMINED;
}

/* Update derived occupancy bitboards
 * Should be called after any piece movement
 */
void computeOccupancyMasks(Board* board) {
    board->occupancyWhite = board->pawn_w | board->knight_w | board->bishop_w | 
                            board->rook_w | board->queen_w | board->king_w;
    board->occupancyBlack = board->pawn_b | board->knight_b | board->bishop_b | 
                            board->rook_b | board->queen_b | board->king_b;
    board->occupancy = board->occupancyBlack | board->occupancyWhite;
}

/* Execute an en passant capture move
 * Special handling required for pawn capture and hash updates
 */
void executeEnPassantCapture(Board* board, const Move move) {
    int capturedPawnSquare = board->epSquare + (board->turn ? -8 : 8);

    /* Update position hash for en passant */
    board->hash ^= EN_PASSANT[board->epSquare];
    board->hash ^= PIECES[move.pieceType][move.fromSquare];
    board->hash ^= PIECES[move.pieceType][move.toSquare];
    board->hash ^= PIECES[board->turn ? PAWN_B : PAWN_W][capturedPawnSquare];
    board->hash ^= WHITE_TO_MOVE;

    /* Get pointers to relevant pawn bitboards */
    Bitboard* activePawns = board->turn ? &(board->pawn_w) : &(board->pawn_b);
    Bitboard* opponentPawns = board->turn ? &(board->pawn_b) : &(board->pawn_w);

    /* Remove captured pawn from opponent's bitboard */
    *opponentPawns = toggleBit(*opponentPawns, capturedPawnSquare);

    /* Move the capturing pawn to en passant square */
    *activePawns = toggleBit(*activePawns, move.fromSquare);
    *activePawns = setBit(*activePawns, board->epSquare);

    board->epSquare = -1;
    board->turn = board->turn ? 0 : 1;
    computeOccupancyMasks(board);
}

/* Execute a castling move (kingside or queenside for either color)
 * Moves both king and rook to their castled positions
 */
void executeCastlingMove(Board* board, const Move move) {
    /* Handle each castling type separately */
    if (move.castle == K) {  /* White kingside */
        board->hash ^= PIECES[KING_W][E1];
        board->hash ^= PIECES[KING_W][G1];
        board->hash ^= PIECES[ROOK_W][H1];
        board->hash ^= PIECES[ROOK_W][F1];
        board->king_w = board->king_w >> 2;
        board->rook_w = toggleBit(board->rook_w, H1);
        board->rook_w = setBit(board->rook_w, F1);
        board->whiteKingSq = G1;
    } else if (move.castle == Q) {  /* White queenside */
        board->hash ^= PIECES[KING_W][E1];
        board->hash ^= PIECES[KING_W][C1];
        board->hash ^= PIECES[ROOK_W][A1];
        board->hash ^= PIECES[ROOK_W][D1];
        board->king_w = board->king_w << 2;
        board->rook_w = toggleBit(board->rook_w, A1);
        board->rook_w = setBit(board->rook_w, D1);
        board->whiteKingSq = C1;
    } else if (move.castle == k) {  /* Black kingside */
        board->hash ^= PIECES[KING_B][E8];
        board->hash ^= PIECES[KING_B][G8];
        board->hash ^= PIECES[ROOK_B][H8];
        board->hash ^= PIECES[ROOK_B][F8];
        board->king_b = board->king_b >> 2;
        board->rook_b = toggleBit(board->rook_b, H8);
        board->rook_b = setBit(board->rook_b, F8);
        board->blackKingSq = G8;
    } else if (move.castle == q) {  /* Black queenside */
        board->hash ^= PIECES[KING_B][E8];
        board->hash ^= PIECES[KING_B][C8];
        board->hash ^= PIECES[ROOK_B][A8];
        board->hash ^= PIECES[ROOK_B][D8];
        board->king_b = board->king_b << 2;
        board->rook_b = toggleBit(board->rook_b, A8);
        board->rook_b = setBit(board->rook_b, D8);
        board->blackKingSq = C8;
    }

    /* Update hash for en passant and turn change */
    if (board->epSquare != -1) {
        board->hash ^= EN_PASSANT[board->epSquare];
    }

    board->hash ^= WHITE_TO_MOVE;
    board->hash ^= CASTLING[board->castling];

    /* Remove castling rights for the side that just castled */
    board->castling &= board->turn ? CASTLING_BLACK_BOTH : CASTLING_WHITE_BOTH;
    board->hash ^= CASTLING[board->castling];

    computeOccupancyMasks(board);
    board->epSquare = -1;
    board->turn = board->turn ? 0 : 1;
}

/* Execute a chess move and update all board state
 * Handles regular moves, captures, promotions, en passant, and castling
 */
void pushMove(Board* board, const Move move) {
    /* Handle special move types */
    bool isEnPassantCapture = move.toSquare == board->epSquare && 
                             (move.pieceType == PAWN_W || move.pieceType == PAWN_B);
    if (isEnPassantCapture) {
        executeEnPassantCapture(board, move);
        return;
    } 
    
    if (move.castle) {
        executeCastlingMove(board, move); 
        return;
    }

    /* Update position hash - remove old state */
    board->hash ^= PIECES[move.pieceType][move.fromSquare];
    board->hash ^= WHITE_TO_MOVE;
    board->hash ^= CASTLING[board->castling];
    if (board->epSquare != -1) {
        board->hash ^= EN_PASSANT[board->epSquare];
    }

    /* Determine if this move creates an en passant opportunity */
    board->epSquare = -1;
    bool isPawnStartingRankMove = (move.pieceType == PAWN_W && (move.fromSquare > A1 && move.fromSquare < H3)) ||
                                  (move.pieceType == PAWN_B && (move.fromSquare > A6 && move.fromSquare < H8));
    if (isPawnStartingRankMove) {
        int squaresTraveled = abs(move.fromSquare - move.toSquare);
        if (squaresTraveled == PAWN_DOUBLE_MOVE_DISTANCE) {
            board->epSquare = board->turn ? move.fromSquare + 8 : move.fromSquare - 8;
        }
    }

    /* Update hash with new en passant target square if any */
    if (board->epSquare != -1) {
        board->hash ^= EN_PASSANT[board->epSquare];
    }

    /* Move the piece on its bitboard */
    Bitboard activePlayerRooks = board->turn ? board->rook_w : board->rook_b;
    Bitboard* movingPieceBitboard = (&board->pawn_w + move.pieceType);

    /* Update castling rights if king or rook moves */
    if (move.pieceType == KING_W || move.pieceType == KING_B) {
        board->castling &= board->turn ? CASTLING_BLACK_BOTH : CASTLING_WHITE_BOTH;
    } else if (*movingPieceBitboard == activePlayerRooks) {
        if (board->turn && move.fromSquare == A1) board->castling &= CASTLING_REMOVE_WQ;
        if (board->turn && move.fromSquare == H1) board->castling &= CASTLING_REMOVE_WK;
        if (!board->turn && move.fromSquare == A8) board->castling &= CASTLING_REMOVE_BQ;
        if (!board->turn && move.fromSquare == H8) board->castling &= CASTLING_REMOVE_BK;
    }

    /* Remove piece from origin square */
    *movingPieceBitboard = toggleBit(*movingPieceBitboard, move.fromSquare);

    /* Place piece on destination square (handle promotions) */
    if (move.promotion <= 0) {
        /* Regular move - place same piece type */
        *movingPieceBitboard = setBit(*movingPieceBitboard, move.toSquare);

        /* Update king position cache if king moved */
        if (*movingPieceBitboard == board->king_w) {
            board->whiteKingSq = move.toSquare;
        } else if (*movingPieceBitboard == board->king_b) {
            board->blackKingSq = move.toSquare;
        }

        board->hash ^= PIECES[move.pieceType][move.toSquare];
    } else {
        /* Pawn promotion - place promoted piece instead */
        Bitboard* promotedPieceBitboard = &(board->pawn_w) + move.promotion;
        *promotedPieceBitboard = setBit(*promotedPieceBitboard, move.toSquare);
        board->hash ^= PIECES[move.promotion][move.toSquare];
    }

    /* Check for captures and remove captured pieces */
    Bitboard* opponentPieceBitboard = board->turn ? &(board->pawn_b) : &(board->pawn_w);
    Bitboard opponentRooks = board->turn ? board->rook_b : board->rook_w;
    
    for (int pieceTypeOffset = 0; pieceTypeOffset < 5; pieceTypeOffset++) {

        if (getBit(*opponentPieceBitboard, move.toSquare)) {

            /* Update hash to reflect captured piece */
            board->hash ^= PIECES[(board->turn ? 6 : 0) + pieceTypeOffset][move.toSquare];

            /* Update castling rights if a rook was captured */
            if (*opponentPieceBitboard == opponentRooks) {
                if (board->turn && move.toSquare == H8) board->castling &= CASTLING_REMOVE_BK;
                if (board->turn && move.toSquare == A8) board->castling &= CASTLING_REMOVE_BQ;
                if (!board->turn && move.toSquare == A1) board->castling &= CASTLING_REMOVE_WQ;
                if (!board->turn && move.toSquare == H1) board->castling &= CASTLING_REMOVE_WK;
            }

            /* Remove captured piece from its bitboard */
            *opponentPieceBitboard = toggleBit(*opponentPieceBitboard, move.toSquare);
        }
        ++opponentPieceBitboard;
    }

    /* Update hash with new castling rights */
    board->hash ^= CASTLING[board->castling];

    /* Switch active player */
    board->turn = board->turn ? 0 : 1;
    computeOccupancyMasks(board);
}

/* Display the current board position in ASCII art format
 * Shows pieces, active player, castling rights, and en passant square
 */
void printBoard(Board board) {
    char pieceSymbols[] = "PNBRQKpnbrqk";
    printf("\n");
    
    /* Display board from white's perspective (rank 8 to rank 1) */
    for (int rank = 0; rank < 8; rank++) {
        for (int file = 0; file < 8; file++) {

            int squareIndex = 63 - ((rank * 8) + file);
            bool pieceFound = false;

            /* Check each piece type to see if it occupies this square */
            for (int pieceType = 0; pieceType < 12; pieceType++) {
                Bitboard* pieceBitboard = ((Bitboard*) &board) + pieceType;                    

                if (getBit(*pieceBitboard, squareIndex)) {
                    printf("%c", pieceSymbols[pieceType]);
                    pieceFound = true;
                    break;
                }
            }

            /* Display empty square if no piece found */
            if (!pieceFound) putchar('.');
            printf(" ");
        }
        printf("\n");
    }

    /* Display game state information */
    printf("Turn: %s\n", board.turn ? "White" : "Black");
    printf("Castling: ");
    for (int i = 0; i < 4; i++) {
        if ((board.castling & (1 << i)) >> i) {
            putchar(CASTLING_RIGHTS[i][0]);
        }
    }
    printf("\nEn Passant square: %s\n", 
           board.epSquare == -1 ? "None" : &SQUARES[board.epSquare][0]);
    printf("\n");
}
