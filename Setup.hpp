#pragma once

#include <wx/wx.h>
#include <iostream>
#include <vector>
#include <random>
#include <wx/dialog.h>
#include <wx/sizer.h>
#include <wx/button.h>
#include <enet/enet.h>
#include <thread>

using namespace std;

typedef vector<vector<int>> Matrix;
typedef vector<pair<int, int>> TPosition;
enum type_tetromino { EMPTY, I, O, T, L, J, Z, S };

const int LINES = 22; // Number of lines in the tetris board
const int COLUMNS = 10; // Number of columns in the tetris board
const int CENTERING = (COLUMNS - 3) / 2; // value is ((width of board - 3) / 2)
const int blockSize = 20; // Display size of a block/cell
const int linesPerLevel = 10; // Number of lines to clear to go up a level
const bool isHost = true; // Is this the host or a client, for multiplayer
const string hostAddress = "127.0.0.1"; // Change to the address of host pc
const int PORT = 7777;
