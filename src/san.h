#ifndef SAN_H
#define SAN_H

#include "typedefs.h"

/* Convert a move structure to Standard Algebraic Notation string */
void convertMoveToSAN(const Move move, char sanNotation[]);

/* Parse a SAN string and populate a move structure */
void parseSANToMove(const Board board, Move* move, const char* sanNotation);

/* Execute a move specified in SAN format */
void executeSANMove(Board* board, char* sanNotation);

/* Legacy function names for compatibility */
#define moveToSan convertMoveToSAN
#define sanToMove parseSANToMove
#define pushSan executeSANMove

#endif