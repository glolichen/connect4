#include <iostream>
#include <sstream>
#include <string>

#include "bitboard.h"
#include "move.h"

void move::make_move(bitboard::Position &board, int move) {
	for (int i = 5; i >= 0; i--) {
		if (board.board[i][FILE(move)] == -1) {
			if (COLOR(move) == RED)
				board.board[i][FILE(move)] = RED;
			else
				board.board[i][FILE(move)] = YELLOW;
			break;
		}
	}
	board.turn = !board.turn;
}

void move::unmake_move(bitboard::Position &board, int move) {
	for (int i = 0; i < 6; i++) {
		if (board.board[i][FILE(move)] >= 0) {
			board.board[i][FILE(move)] = -1;
			break;
		}
	}
	board.turn = !board.turn;
}

void move::movegen(bitboard::Position &board, std::vector<int> &moves) {
	moves.clear();
	for (int i = 0; i < 7; i++) {
		if (board.board[0][i] == -1)
			moves.push_back(NEW_MOVE(i, board.turn));
	}
}

std::string move::to_string(int move) {
	return (COLOR(move) == 1 ? "Y" : "R") + std::to_string(FILE(move) + 1);
}