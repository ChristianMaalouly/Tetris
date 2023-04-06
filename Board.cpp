#include "Board.hpp"

int clearLines(Matrix& board) {
	int linesCleared = 0;
	
	// Go through each line
	for (int l = 0; l < LINES; l++) {
		bool complete = true;

		// Check if line complete to clear
		for (int c = 0; c < COLUMNS; c++) {
			if (board[c][l] == 0) {
				complete = false;
				break;
			}
		}
		if (complete) {
			linesCleared++;
			//clear line and move all the above down by 1 line to fill in the gap
			for (int c = 0; c < COLUMNS; c++) {
				for (int i = l; i > 0; i--) {
					board[c][i] = board[c][i - 1];
					board[c][i - 1] = 0;
				}
			}
		}
	}
	//return number of lines cleared at once so wwe can calculate the score
	return linesCleared;
}

void mergeTetromino(Matrix& board, const Tetromino& t) {
	int type = t.getType();
	
	// Add the tetromino to the board
	for (auto& cell : t.getPos()) {
		board[cell.first][cell.second] = type;
	}
}