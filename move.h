#ifndef MOVE_H
#define MOVE_H

#include <vector>
#include "bitboard.h"

#define NEW_MOVE(file, color) ((file << 1) | (color & 1))
#define FILE(move) (((move) >> 1) & 0b111)
#define COLOR(move) ((move) & 0b1)

namespace move {
	void make_move(bitboard::Position &board, int move);
	void unmake_move(bitboard::Position &board, int move);
	void movegen(bitboard::Position &board, std::vector<int> &moves);
	std::string to_string(int move);
}

#endif