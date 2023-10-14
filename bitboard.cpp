#include <iostream>
#include <string>
#include <vector>
#include <cstring>

#include "bitboard.h"

std::string bitboard::squares[42] = {
	"g1", "f1", "e1", "d1", "c1", "b1", "a1",
	"g2", "f2", "e2", "d2", "c2", "b2", "a2",
	"g3", "f3", "e3", "d3", "c3", "b3", "a3",
	"g4", "f4", "e4", "d4", "c4", "b4", "a4",
	"g5", "f5", "e5", "d5", "c5", "b5", "a5",
	"g6", "f6", "e6", "d6", "c6", "b6", "a6",
};
bitboard::Position bitboard::board;

std::vector<std::string> bitboard::split(std::string str, char split_on) {
	std::vector<std::string> result;

	int left = 0;
	for (size_t i = 0; i < str.length(); i++) {
		if (str[i] == split_on) {
			result.push_back(str.substr(left, i - left));
			left = i + 1;
		}
	}

	result.push_back(str.substr(left, str.length() - left));
	return result;
}

void bitboard::decode(std::string position) {
	for (int i = 0; i < 6; i++) {
		for (int j = 0; j < 7; j++)
			bitboard::board.board[i][j] = -1;
	}

	std::vector<std::string> split1 = bitboard::split(position, ' ');
	std::vector<std::string> splitted = bitboard::split(split1[0], '/');

	for (int i = 0; i < 6; i++) {
		int cur = 0;
		std::string cur_rank = splitted[i];
		for (size_t j = 0; j < cur_rank.size(); j++) {
			if (isdigit(cur_rank[j])) {
				cur += cur_rank[j] - '0';
				continue;
			}

			if (cur_rank[j] == 'r')
				bitboard::board.board[i][cur] = RED;
			if (cur_rank[j] == 'y')
				bitboard::board.board[i][cur] = YELLOW;

			cur++;
		}
	}

	bitboard::board.turn = (split1[1] == "y");
}
std::string bitboard::encode(const bitboard::Position &board) {
	std::string position = "";
	for (int i = 0; i < 6; i++) {
		int empty = 0;
		for (int j = 0; j < 7; j++) {
			if (board.board[i][j] >= 0) {
				if (empty != 0)
					position += empty + '0';
				empty = 0;

				if (board.board[i][j] == RED)
					position += "r";
				if (board.board[i][j] == YELLOW)
					position += "y";
			}
			else
				empty++;
		}

		if (empty != 0)
			position += empty + '0';

		if (i < 7)
			position += "/";
	}
	position += " ";
	position += board.turn ? 'y' : 'r';
	return position;
}

void bitboard::print_board(const bitboard::Position &board) {
	std::cout << "+---+---+---+---+---+---+---+" << "\n";

	for (int i = 0; i < 6; i++) {
		for (int j = 0; j < 7; j++) {
			if (bitboard::board.board[i][j] == RED)
				std::cout << "| r ";
			else if (bitboard::board.board[i][j] == YELLOW)
				std::cout << "| y ";
			else
				std::cout << "|   ";
		}

		std::cout << "|";

		if (i < 7)
			std::cout << "\n+---+---+---+---+---+---+---+\n";
		else
			std::cout << "\n+---+---+---+---+---+---+---+\n";
	}
	std::cout << bitboard::encode(board) << "\n";
}
