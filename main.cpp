#include <string>

#include "bitboard.h"
#include "move.h"
#include "game.h"
#include "hash.h"
#include "search.h"

int main() {
	hash::init();

	// at the end of each row, add a /
	// on consecutive blank spaces, write the number of spaces as a number
	// if there is a piece, write the color - r for red and y for yellow
	// after this (there should be 6 rows and 5 slashes), add the next turn - same rule as before
	// basically, forsyth edwards notation in chess adapted for connect 4
	// https://en.wikipedia.org/wiki/Forsyth%E2%80%93Edwards_Notation#Definition

	// theres stuff in search.cpp for mutlithreaded search
	// i haven't figured that out yet but thats from chess engine
	bitboard::decode("7/7/1yy4/1rr4/1rr1y1/1ryyyr1 r");
	bitboard::print_board(bitboard::board);
	auto result = search::search_unlimited(bitboard::board);

	return 0;
}