#include "game.h"

int game::win_detect(bitboard::Position &board) {
	for (int i = 0; i < 6; i++) {
		for (int j = 0; j <= 3; j++) {
			bool ok = true;
			for (int k = 1; k < 4; k++) {
				if (board.board[i][j + k] == -1 || board.board[i][j + k] != board.board[i][j]) {
					ok = false;
					break;
				}
			}
			if (ok)
				return board.board[i][j];
		}
	}

	for (int i = 0; i < 7; i++) {
		for (int j = 0; j <= 2; j++) {
			bool ok = true;
			for (int k = 1; k < 4; k++) {
				if (board.board[j + k][i] == -1 || board.board[j + k][i] != board.board[j][i]) {
					ok = false;
					break;
				}
			}
			if (ok)
				return board.board[j][i];
		}
	}
	
	for (int i = 0; i <= 3; i++) {
		for (int j = 0; j <= 2; j++) {
			bool ok = true;
			for (int k = 1; k < 4; k++) {
				if (board.board[i + k][j + k] == -1 || board.board[i + k][j + k] != board.board[i][j]) {
					ok = false;
					break;
				}
			}
			if (ok)
				return board.board[i][j];
		}
	}
	
	for (int i = 3; i < 7; i++) {
		for (int j = 0; j <= 2; j++) {
			bool ok = true;
			for (int k = 1; k < 4; k++) {
				if (board.board[j + k][i - k] == -1 || board.board[j + k][i - k] != board.board[j][i]) {
					ok = false;
					break;
				}
			}
			if (ok)
				return board.board[j][i];
		}
	}

	return -1;
}