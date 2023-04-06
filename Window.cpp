#include "Setup.hpp"
#include "Game.hpp"
#include "Window.hpp"

//Initialize window size, set background, and start the game
Tetris::Tetris(const wxString& title) : wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxSize((COLUMNS + 9 + (COLUMNS/2))* blockSize, (LINES + 9)* blockSize)) {
    SetBackgroundColour(wxColor(0, 0, 0));
    TetrisGame* board = new TetrisGame(this);
    board->SetFocus();
}

void Tetris::OnQuit() {
    // Close the application
    Close(true);
}