#ifndef GAME_H
#define GAME_H

#include "bitboard.h"

namespace game {
	int win_detect(bitboard::Position &board);
}

#endif