#pragma once

#include "Setup.hpp"

TPosition getInitialPos(type_tetromino type);
TPosition getWallKickData(bool is_I, int rotation);

class Tetromino {
private:
	type_tetromino type = I;
	TPosition pos;
	int rotation = 0;
public:
	Tetromino() {}; // Default constructor
	Tetromino(type_tetromino t); // Construct tetromino based on type

	type_tetromino getType() const { return type; }
	TPosition getPos() const { return pos; }

	bool newTetromino(type_tetromino t, const Matrix& board); // Make a new tetromino from the same instance when done with the previous
	bool moveDown(const Matrix& board); // Move tetromino down, return false if it can't
	void moveLeft(const Matrix& board); // Move tetromino left
	void moveRight(const Matrix& board); // Move tetromino right
	void rotate(const Matrix& board); // Rotate tetromino
	TPosition quickDrop(const Matrix& board); // Find position of tetromino for a quick drop
	void drop(const Matrix& board); // Apply a quick drop
};