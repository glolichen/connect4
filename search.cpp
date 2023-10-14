#include <chrono>
#include <cstring>
#include <iostream>
#include <limits.h>
#include <unordered_map>
#include <tuple>
#include <queue>
#include <vector>
#include <future>
#include <thread>

#include "bitboard.h"
#include "game.h"
#include "move.h"
#include "hash.h"
#include "search.h"

#include "ctpl_stl.h"

/*
add soon:
 * transposition table on quiscence
 * play test games
 * std map to unordered map
*/

#define DELTA_CUTOFF 800
#define QUISCENCE_DEPTH 5
#define TT_KEEP_MOVE_COUNT 5
#define NODES_PER_TIME_CHECK 8192
#define THREAD_COUNT 8
#define INT_MIN_PLUS_1 (INT_MIN + 1)

struct TTResult {
	int eval, move, depth, moveNum;
};

ctpl::thread_pool tp(THREAD_COUNT);

bool topMoveNull;
int bestMove;
int secondBestEval;

int moveNum = 0;

std::mutex mx;
std::unordered_map<ull, TTResult> transposition;

void table_insert(ull hash, TTResult result) {
	mx.lock();
	transposition.insert({ hash, result });
	mx.unlock();
}

void search::table_clear() {
	moveNum = 0;
	transposition.clear();
}
void search::table_clear_move() {
	moveNum++;
	// https://stackoverflow.com/a/8234813
	for (auto it = transposition.begin(); it != transposition.end();) {
		if (it->second.moveNum >= moveNum - TT_KEEP_MOVE_COUNT)
			transposition.erase(it++);
		else
			it++;
	}
}

void search::pvs(int &result, bitboard::Position &board, int depth, int alpha, int beta, int depth_from_start, bool use_threads) {
	std::vector<int> moves;
	move::movegen(board, moves);

	// if (depth_from_start == 0 && !topMoveNull) {
	// 	for (size_t i = 0; i < moves.size(); i++) {
	// 		if (moves[i] == bestMove) {
	// 			moves.erase(moves.begin() + i);
	// 			break;
	// 		}
	// 	}
	// 	moves.insert(moves.begin(), bestMove);
	// }

	int win = game::win_detect(board);
	if (win >= 0) {
		if (board.turn != win)
			result = INT_MIN_PLUS_1 + depth_from_start;
		else
			result = INT_MAX - depth_from_start;
		return;
	}

	if (depth == 0) {		
		// this is where the ai stuff goes
		result = 0;
		return;
	}

	ull hash = hash::get_hash(board);
	auto it = transposition.find(hash);
	if (it != transposition.end()) {
		// bool contains = false;
		// for (size_t i = 0; i < moves.size(); i++) {
		// 	if (moves[i] == it->second.move) {
		// 		moves.erase(moves.begin() + i);
		// 		moves.insert(moves.begin(), it->second.move);
		// 		contains = true;
		// 		break;
		// 	}
		// }
		if (it->second.depth > depth && depth_from_start) {
			result = it->second.eval * (board.turn ? -1 : 1);
			return;
		}
	}
	ull mirrorHash = hash::get_mirror_hash(board);
	it = transposition.find(mirrorHash);
	if (it != transposition.end()) {
		if (it->second.depth > depth && depth_from_start) {
			result = it->second.eval * (board.turn ? -1 : 1);
			return;
		}
	}

	int *data;
	std::vector<std::future<void>> results;
	if (use_threads)
		data = new int[moves.size()];

	int topMove = moves[0];
	for (size_t i = 0; i < moves.size(); i++) {
		move::make_move(board, moves[i]);
		
		int curEval;
		if (i == 0) {
			search::pvs(curEval, board, depth - 1, -beta, -alpha, depth_from_start + 1, use_threads);
			curEval *= -1;
		}
		else {
			if (use_threads) {
				results.push_back(tp.push([&data, i, &board, depth, alpha, beta, depth_from_start](int) {
					search::pvs(data[i], board, depth - 1, -alpha - 1, -alpha, depth_from_start + 1, false);
					data[i] *= -1;
					if (data[i] > alpha) {
						search::pvs(data[i], board, depth - 1, -beta, -data[i], depth_from_start + 1, false);
						data[i] *= -1;
					}
				}));
				move::unmake_move(board, moves[i]);
				continue;
			}
			else {
				search::pvs(curEval, board, depth - 1, -alpha - 1, -alpha, depth_from_start + 1, false);
				curEval *= -1;
				if (curEval > alpha) {
					search::pvs(curEval, board, depth - 1, -beta, -curEval, depth_from_start + 1, false);
					curEval *= -1;
				}
			}
		}

		move::unmake_move(board, moves[i]);
		if (curEval > alpha) {
			alpha = curEval;
			topMove = moves[i];
		}
		if (alpha >= beta)
			break;
	}

	if (use_threads) {
		for (size_t i = 0; i < results.size(); i++)
			results[i].get();
		for (size_t i = 1; i < moves.size(); i++) {
			if (data[i] > alpha) {
				if (depth_from_start == 0) {
					secondBestEval = alpha;
				}
				alpha = data[i];
				topMove = moves[i];
			}
			if (alpha >= beta)
				break;
		}
	}

	it = transposition.find(hash);
	if (it == transposition.end())
		table_insert(hash, { alpha * (board.turn ? -1 : 1), topMove, depth, moveNum });
	else {
		if (depth > it->second.depth) {
			mx.lock();
			transposition[hash] = { alpha * (board.turn ? -1 : 1), topMove, depth, moveNum };
			mx.unlock();
		}
	}

	if (depth_from_start == 0)
		bestMove = topMove;

	result = alpha;

	if (use_threads)
		delete[] data;
}

search::SearchResult search::search_by_depth(bitboard::Position &board, int depth) {	
	int eval = 0;
	search::pvs(eval, board, depth, INT_MIN_PLUS_1 + 2, INT_MAX - 2, 0, false);

	std::cout << "info depth " << std::to_string(depth) << " move " << move::to_string(bestMove) << " score ";

	if (eval >= INT_MAX - 250) {
		std::cout << "mate " << (INT_MAX - eval) << "\n";
	}
	else if (eval <= INT_MIN_PLUS_1 + 250) {
		std::cout << "mate " << (INT_MIN_PLUS_1 - eval) << "\n";
	}
	else
		std::cout << "ai guess " << eval << "\n";

	return { bestMove, depth, eval };
}

search::SearchResult search::search_unlimited(bitboard::Position &board) {
	topMoveNull = true;
	
	int depth = 3, eval = 0;
	while (true) {
		auto result = search::search_by_depth(board, depth);
		if (eval_is_mate(result.eval))
			break;
		depth++;
	}

	return { bestMove, depth, eval };
}

int search::eval_is_mate(int eval) {
	return (eval > INT_MAX - 250) || (eval < INT_MIN_PLUS_1 + 250);
}