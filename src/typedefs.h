#ifndef TYPEDEFS_H
#define TYPEDEFS_H

/* Player color definitions for chess logic */
#define COLOR_BLACK 0
#define COLOR_WHITE 1

/* Legacy compatibility - use either old or new names */
#define BLACK COLOR_BLACK
#define WHITE COLOR_WHITE

#include <stdint.h>
#include <stdbool.h>

/* Core chess bitboard type - represents 64 squares as bits */
typedef uint64_t Bitboard;

/* Main board representation structure
 * Uses separate bitboards for each piece type and color to enable
 * efficient move generation and board queries
 */
typedef struct {
    /* White piece bitboards - each bit represents presence on that square */
    Bitboard pawn_w;      /* White pawns bitboard */
    Bitboard knight_w;    /* White knights bitboard */
    Bitboard bishop_w;    /* White bishops bitboard */
    Bitboard rook_w;      /* White rooks bitboard */
    Bitboard queen_w;     /* White queen bitboard */
    Bitboard king_w;      /* White king bitboard */
    
    /* Black piece bitboards */
    Bitboard pawn_b;      /* Black pawns bitboard */
    Bitboard knight_b;    /* Black knights bitboard */
    Bitboard bishop_b;    /* Black bishops bitboard */
    Bitboard rook_b;      /* Black rooks bitboard */
    Bitboard queen_b;     /* Black queen bitboard */
    Bitboard king_b;      /* Black king bitboard */
    
    /* Game state tracking */
    int turn;             /* Current player to move (WHITE=1 or BLACK=0) */
    int castling;         /* Bitfield encoding castling rights (KQkq) */
    int epSquare;         /* En passant target square index, -1 if none */
    int halfmoves;        /* Half-moves since last pawn move or capture (50-move rule) */
    int fullmoves;        /* Full move number (incremented after black's move) */
    int whiteKingSq;      /* Cached square index of white king (0-63) */
    int blackKingSq;      /* Cached square index of black king (0-63) */
    
    /* Derived occupancy bitboards - computed from piece bitboards */
    Bitboard occupancy;       /* Combined bitboard of all pieces */
    Bitboard occupancyWhite;  /* Combined bitboard of white pieces */
    Bitboard occupancyBlack;  /* Combined bitboard of black pieces */
    
    /* Zobrist position hash for transposition table lookups */
    Bitboard hash;
    
    /* Attack bitboard of opponent pieces (for check detection) */
    Bitboard attacks;
} Board;

/* Chess move representation structure
 * Encodes all information needed to execute and undo a move
 */
typedef struct {
    int fromSquare;        /* Origin square index (0-63) */
    int toSquare;          /* Destination square index (0-63) */
    int promotion;         /* Promoted piece type (for pawn promotion), -1 if none */
    int castle;            /* Castling move indicator (K, Q, k, q), 0 if not castling */
    int validation;        /* Move legality flag (NOT_VALIDATED, LEGAL, ILLEGAL) */
    int pieceType;         /* Type of piece being moved (PAWN_W, KNIGHT_W, etc.) */
    int score;             /* Move ordering score for search optimization */
    bool exhausted;        /* Flag indicating if this move has been searched */
} Move;

/* Chess piece type enumeration
 * Separate constants for white and black pieces
 */
enum PIECES {
    PAWN_W, KNIGHT_W, BISHOP_W, ROOK_W, QUEEN_W, KING_W,
    PAWN_B, KNIGHT_B, BISHOP_B, ROOK_B, QUEEN_B, KING_B
};

enum SQUARES {
    H1, G1, F1, E1, D1, C1, B1, A1,
    H2, G2, F2, E2, D2, C2, B2, A2,
    H3, G3, F3, E3, D3, C3, B3, A3,
    H4, G4, F4, E4, D4, C4, B4, A4,
    H5, G5, F5, E5, D5, C5, B5, A5,
    H6, G6, F6, E6, D6, C6, B6, A6,
    H7, G7, F7, E7, D7, C7, B7, A7,
    H8, G8, F8, E8, D8, C8, B8, A8
};

/* Move validation status flags */
enum MOVE_VALIDAITON {
    NOT_VALIDATED,  /* Move hasn't been checked for legality yet */
    LEGAL,          /* Move is confirmed legal */
    ILLEGAL         /* Move is illegal (leaves king in check, etc.) */
};

/* Game outcome enumeration */
enum GAME_RESULT {
    UN_DETERMINED,  /* Game is still in progress */
    WHITE_WIN,      /* White has won */
    BLACK_WIN,      /* Black has won */
    DRAW            /* Game ended in draw */
};

/* Castling rights bit flags
 * K = White kingside, Q = White queenside
 * k = Black kingside, q = Black queenside
 */
enum CASTLING {
    K=1,  /* White can castle kingside */
    Q=2,  /* White can castle queenside */
    k=4,  /* Black can castle kingside */
    q=8   /* Black can castle queenside */
};

/* Castling rights bit masks for efficient manipulation
 * Used to set, clear, and check castling availability
 */
#define CASTLING_WHITE_KING  0b0001  /* White kingside castling */
#define CASTLING_WHITE_QUEEN 0b0010  /* White queenside castling */
#define CASTLING_BLACK_KING  0b0100  /* Black kingside castling */
#define CASTLING_BLACK_QUEEN 0b1000  /* Black queenside castling */
#define CASTLING_WHITE_BOTH  0b0011  /* Both white castling rights */
#define CASTLING_BLACK_BOTH  0b1100  /* Both black castling rights */
#define CASTLING_REMOVE_WK   0b1110  /* Mask to remove white kingside */
#define CASTLING_REMOVE_WQ   0b1101  /* Mask to remove white queenside */
#define CASTLING_REMOVE_BK   0b1011  /* Mask to remove black kingside */
#define CASTLING_REMOVE_BQ   0b0111  /* Mask to remove black queenside */

/* File enumeration (chess board columns) */
enum FILES {H, G, F, E, D, C, B, A};

/* Square name strings for notation */
extern char SQUARES[64][3];

#endif