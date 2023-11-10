#include <stdio.h>
#include <string.h>
#include "utils.h"
#include "san.h"

void printBitboard(const Bitboard bb) {
    printf("\n");
    for (int y = 0; y < 8; y++) {
        for (int x = 0; x < 8; x++) {

            int loc = 63 - ((y*8) + x);
            Bitboard bit = bb & (1LL << loc);

            if (bit) {
                putchar('+');
            } else {
                putchar('-');
            }

            printf(" ");
        }
        printf("\n");
    }
    printf("\n");
}

void printBits(const Bitboard bb) {
    for (int i = 0; i < 64; i++) {
        Bitboard bit = bb & (1LL << (63 - i));
        printf(bit ? "1" : "0");
    }
    printf("\n");
}

void printMoves(const Move moves[], int length) {
    printf("Moves: %d\n", length);
    for(int i = 0;i<length;i++) {
        char san[6];
        moveToSan(moves[i], san);
        printf("%s\n", san);
        memset(san, 0, sizeof(san));
    }
}