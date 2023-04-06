#include "Tetromino.hpp"

using namespace std;

TPosition getInitialPos(type_tetromino type) {
	//position of each of the 4 cells of each tetromino type, the first cell is the rotation center except for the I as it has a different rotation center
	switch (type) {
	case I:
		return { {0 + CENTERING, 0}, {1 + CENTERING, 0}, {2 + CENTERING, 0}, {3 + CENTERING, 0} };
	case O:
		return { {1 + CENTERING, 0}, {1 + CENTERING, 1}, {2 + CENTERING, 0}, {2 + CENTERING, 1} };
	case T:
		return { {1 + CENTERING, 1}, {0 + CENTERING, 1}, {1 + CENTERING, 0}, {2 + CENTERING, 1} };
	case L:
		return { {1 + CENTERING, 1}, {2 + CENTERING, 0}, {0 + CENTERING, 1}, {2 + CENTERING, 1} };
	case J:
		return { {1 + CENTERING, 1}, {0 + CENTERING, 1}, {0 + CENTERING, 0}, {2 + CENTERING, 1} };
	case Z:
		return { {1 + CENTERING, 1}, {0 + CENTERING, 0}, {1 + CENTERING, 0}, {2 + CENTERING, 1} };
	case S:
		return { {1 + CENTERING, 1}, {0 + CENTERING, 1}, {1 + CENTERING, 0}, {2 + CENTERING, 0} };
	}
	
}

TPosition getWallKickData(bool is_I, int rotation) {
	//all the moves allowed when we rotate and hit a wall or a filled out cell according to the tetris rules
	//moves possible for an I tetromino
	if (is_I) {
		switch (rotation) {
		case 0:
			return { {0, 0}, {-2, 0}, {1, 0}, {-2, 1}, {1, -2} };
		case 1:
			return { {0, 0}, {-1, 0}, {2, 0}, {-1, -2}, {2, 1} };
		case 2:
			return { {0, 0}, {2, 0}, {-1, 0}, {2, -1}, {-1, 2} };
		case 3:
			return { {0, 0}, {1, 0}, {-2, 0}, {1, 2}, {-2, -1} };
		}
	}
	//moves possible for all the other tetromino
	else {
		switch (rotation) {
		case 0:
			return { {0, 0}, {-1, 0}, {-1, -1}, {0, 2}, {-1, 2} };
		case 1:
			return { {0, 0}, {1, 0}, {1, 1}, {0, -2}, {1, -2} };
		case 2:
			return { {0, 0}, {1, 0}, {1, -1}, {0, 2}, {1, 2} };
		case 3:
			return { {0, 0}, {-1, 0}, {-1, 1}, {0, -2}, {-1, -2} };
		}
	}
	return { {0, 0} };
}

Tetromino::Tetromino(type_tetromino t) {
	type = t;
	pos = getInitialPos(t);
	rotation = 0;
}

bool Tetromino::newTetromino(type_tetromino t, const Matrix& board) {
	//reset the attributes
	type = t;
	pos = getInitialPos(t);
	rotation = 0;

	//if the initial position is filled already, game over
	for (auto& cell : pos) {
		if (board[cell.first][cell.second] != 0) return false;
	}
	//no problems encountered
	return true;
}

bool Tetromino::moveDown(const Matrix& board) {
	//If it reached the bottom or landed on a piece
	for (auto& cell : pos) {
		if (cell.second + 1 == LINES) {
			return false;
		}
		if (board[cell.first][cell.second + 1] != 0) {
			return false;
		}
	}
	//move down by 1
	for (auto& cell : pos) {
		cell.second++;
	}
	//no problems encountered
	return true;
}

void Tetromino::moveLeft(const Matrix& board) {
	//check for collisions
	for (auto& cell : pos) {
		if (cell.first - 1 < 0) {
			return;
		}
		// ignore when above the board
		if (0 > cell.second) {
			continue;
		}
		// check if new spot empty
		if (board[cell.first - 1][cell.second] != 0) {
			return;
		}
	}
	//move left by 1
	for (auto& cell : pos) {
		cell.first--;
	}
}

void Tetromino::moveRight(const Matrix& board) {
	//check for collisions
	for (auto& cell : pos) {
		if (cell.first + 1 >= COLUMNS) {
			return;
		}
		// ignore when above the board
		if (0 > cell.second) {
			continue;
		}
		// check if new spot empty
		if (board[cell.first + 1][cell.second] != 0) {
			return;
		}
	}
	//move right by 1
	for (auto& cell : pos) {
		cell.first++;
	}
}

void Tetromino::rotate(const Matrix& board) {
	//no rotation for O shape
	if (type == O) return;

	//save the current position to revert back if we can't rotate
	TPosition currentPos = pos;

	//case of I shape, unique rotation as it doesn't have a center cell to rotate around
	if (type == I) {
		//find the center to rotate around
		float center_x = 0.5f * (pos[1].first + pos[2].first);
		float center_y = 0.5f * (pos[1].second + pos[2].second);
		//adjust based on rotation
		switch (rotation) {
		case 0:
			center_y += 0.5f;
			break;
		case 1:
			center_x -= 0.5f;
			break;
		case 2:
			center_y -= 0.5f;
			break;
		case 3:
			center_x += 0.5f;
			break;
		}
		//rotate based on center
		for (auto& cell : pos) {
			float x = cell.first - center_x;
			float y = cell.second - center_y;
			cell.first = center_x - y;
			cell.second = center_y + x;
		}
	}
	else {
		//rotate based on center, in this case pos[0] is always defined as the center cell when initializing
		for (auto& cell : pos) {
			int x = cell.first - pos[0].first;
			int y = cell.second - pos[0].second;
			cell.first = pos[0].first - y;
			cell.second = pos[0].second + x;
		}
	}
	//If we hit a wall, we try the provided wall kick moves from the tetris wiki
	for (auto wall_kick : getWallKickData(type == I, rotation)) {
		bool canTurn = true;
		for (auto& cell : pos) {
			//if a wall kick move is still hitting a wall or it collides with another tetromino
			if (cell.first + wall_kick.first < 0 || cell.first + wall_kick.first >= COLUMNS || cell.second + wall_kick.second >= LINES) {
				canTurn = false;
				break;
			}
			// ignore when above the board
			if (0 > cell.second + wall_kick.second){
				continue;
			}
			// check if new spot empty
			if (board[cell.first + wall_kick.first][cell.second + wall_kick.second] != 0) {
				canTurn = false;
				break;
			}
		}
		if (canTurn) { // apply rotation
			rotation = (rotation + 1) % 4;
			for (auto& cell : pos) {
				cell.first += wall_kick.first;
				cell.second += wall_kick.second;
			}
			return;
		}	
	}
	pos = currentPos; // could not rotate
}

TPosition Tetromino::quickDrop(const Matrix& board) {
	bool canMoveDown = true;
	int layersToMove = 0;
	TPosition finalPos = pos;
	//as long as it can move down we add another layer that we can move down
	while (canMoveDown) {
		for (auto& cell : pos) {
			if (cell.second + layersToMove + 1 == LINES) {
				canMoveDown = false;
				break;
			}
			// ignore when above the board
			if (0 > cell.second + layersToMove + 1) {
				continue;
			}
			// check if new spot empty
			if (board[cell.first][cell.second + layersToMove + 1] != 0) {
				canMoveDown = false;
				break;
			}
		}
		layersToMove++;
	}
	//we move down all the layers possible to reach a collision and top, that's where the instant drop would end up
	for (auto& cell : finalPos) {
		cell.second += layersToMove - 1;
	}
	return finalPos;
}

void Tetromino::drop(const Matrix& board) {
	pos = quickDrop(board); // apply a quick drop
}