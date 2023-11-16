CC = gcc
CFLAGS = -O3 -Wall -Wextra -Wno-unused-parameter -std=c11
SOURCES = uci.c bitboards.c evaluatemove.c evaluation.c san.c tt.c zobrist.c search.c board.c fen.c utils.c movegen.c magics.c
TEST_SOURCES = test.c bitboards.c evaluation.c san.c zobrist.c board.c fen.c utils.c movegen.c magics.c
TARGET = chessengine

all: $(TARGET)

$(TARGET): $(SOURCES)
	@$(CC) $(CFLAGS) $(SOURCES) -o $(TARGET)

test: $(TEST_SOURCES)
	@$(CC) $(CFLAGS) $(TEST_SOURCES) -o test -L../libs/unity -lunity
	@./test
	@rm test

clean:
	@rm -f $(TARGET) test

.PHONY: all test clean
