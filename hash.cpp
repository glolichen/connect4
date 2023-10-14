#include <tuple>
#include <random>

#include "bitboard.h"
#include "hash.h"

// square, piece occupying the spot
ull zhColors[42][2];

void hash::init() {
	std::random_device rd;
	std::mt19937_64 random(rd());
	std::uniform_int_distribution<ull> distribution;
	for (int i = 0; i < 42; i++) {
		for (int j = 0; j < 2; j++)
			zhColors[i][j] = distribution(random);
	}
}

ull hash::get_hash(const bitboard::Position &board) {
	ull hash = 0;
	for (int i = 0; i < 6; i++) {
		for (int j = 0; j < 7; j++) {
			if (board.board[i][j] == -1)
				continue;
			hash ^= zhColors[i * 7 + j][board.board[i][j]];
		}
	}
	return hash;
}
