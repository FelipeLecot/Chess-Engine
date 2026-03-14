# Recommended Read
[Technical documentation on how chess engines work](https://www.felipelecot.com/docs/chess-engine)

# Chess Engine

## Overview
Chess engine written in C with UCI protocol support. Features bitboard-based move generation, alpha-beta search with transposition tables, and piece-square table evaluation. Can interface with any UCI-compatible chess GUI.

## Core Features
- **Move Generation**: Bitboard-based with magic numbers for sliding pieces (bishops and rooks)
- **Position Evaluation**: Piece-square table evaluation with material counting
- **Search Algorithm**: Configurable depth alpha-beta search with move ordering
- **Game State Handling**: Full support for castling, en passant, and stalemate detection
- **No Opening Book**: Pure tactical play from position evaluation

## Technical Implementation
- **Bitboard Representation**: 64-bit integers represent board state for efficient operations
- **Magic Bitboards**: Pre-computed magic numbers enable fast sliding piece attack generation
- **Zobrist Hashing**: Incremental hash updates for transposition table lookups
- **Alpha-Beta Pruning**: Minimax search with alpha-beta cutoffs and move ordering
- **UCI Protocol**: Standard chess interface for GUI compatibility

## Attributions
Heaviliy based of the following documentation and code:
- https://www.chessprogramming.org/
- https://github.com/bartekspitza/sophia
- https://github.com/Sopel97/FatTitz
- https://github.com/abhigyan7/chess
- https://github.com/kz04px/Baislicka
