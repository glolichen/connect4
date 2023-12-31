#ifndef SEARCH_H
#define SEARCH_H

#include <string>
#include "bitboard.h"
#include "move.h"

namespace search {
	struct SearchResult {
		int move, depth, eval;
	};

	void table_clear();
	void table_clear_move();

	void pvs(int &result, bitboard::Position &board, int depth, int alpha, int beta, int depth_from_start, bool use_threads);

	SearchResult search_by_depth(bitboard::Position &board, int depth);
	SearchResult search_unlimited(bitboard::Position &board);

	int eval_is_mate(int eval);
}

#endif
