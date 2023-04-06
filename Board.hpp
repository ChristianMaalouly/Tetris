#pragma once

#include "Setup.hpp"
#include "Tetromino.hpp"

// Call to clear the lines and drop what's above, also returns the number of lines cleared
int clearLines(Matrix& board);

// Call to add the tetromino to the board once it can no longer move
void mergeTetromino(Matrix& board, const Tetromino& t);
