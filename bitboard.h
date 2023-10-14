#ifndef BITBOARD_H
#define BITBOARD_H

#include <string>
#include <vector>
#include <iostream>
#include <map>
#include <unordered_set>

using ull = unsigned long long;
enum Color { RED, YELLOW };
enum Squares {
	G1, F1, E1, D1, C1, B1, A1,
	G2, F2, E2, D2, C2, B2, A2,
	G3, F3, E3, D3, C3, B3, A3,
	G4, F4, E4, D4, C4, B4, A4,
	G5, F5, E5, D5, C5, B5, A5,
	G6, F6, E6, D6, C6, B6, A6,
};

namespace bitboard {
	struct Position {
		int turn;
		int board[6][7];
	};

	extern Position board;
	
	extern std::string squares[];
	extern std::string pieces;

	std::vector<std::string> split(std::string str, char splitOn);

	void decode(std::string position);
	std::string encode(const Position &board);

	void print_board(const Position &board);
}

#endif
