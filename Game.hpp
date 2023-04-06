#pragma once

#include "Tetromino.hpp"
#include "Setup.hpp"
#include "Board.hpp"
#include "Window.hpp"

class TetrisGame : public wxPanel
{
public:
    TetrisGame(wxFrame* parent);
private:
    Matrix board;
    ; // Integer matrix for the board
    wxColour colors[7]; // Colours for each tetromino type
    Tetromino currentTetromino; // Current tetromino on the board
    int score; // Player's score
    int level; // Player's level
    int linesCleared; // Number of lines cleared
    wxTimer* timer; // Timer to move tetromino down
    bool pause = true; // Whether the game is paused or not
    bool playing = false; // Wether a game is running or not
    bool makingNewPiece = true; // While new piece spawning, to ignore key presses
    bool canRotate = true; // Does not allow a rotation spam when key is held
    int nextTetromino; // The value to define the next tetromino and display preview

    wxButton* startButton; // Start game button
    wxButton* pauseButton; // Pause game button
    wxButton* multiplayerButton; // Multiplayer mode button
    wxButton* quitButton; // Quit game button

    ENetHost* host; // Server or client depending on whether hosting or joining
    bool multiplayer = false; // Whether we're in multiplayer or not

    void Start(); // Start a game
    void InitBoard(); // Initialize the board matrix
    void InitColors(); // Initialize the colours
    void NewTetromino(); // Create a new tetromino on the board
    void DrawBoard(wxDC& dc); // Draw the game board
    void DrawTetromino(wxDC& dc); // Draw the current tetromino
    void DrawScore(wxDC& dc); // Draw the score and level indicators
    void DrawNextTetromino(wxDC& dc); // Draw the next tetromino as a preview on the side
    void MoveDown(); // Move the current tetromino down
    void MoveLeft(); // Move the current tetromino left
    void MoveRight(); // Move the current tetromino right
    void Rotate(); // Rotate the current tetromino
    void QuickDrop(); // Quick drop the current tetromino
    void ClearLines(); // Clear any completed lines on the board
    void GameOver(); // End the game
    void Pause(); // Pause the game

    void OnStart(wxCommandEvent& event); // Function for Start Game button
    void OnPause(wxCommandEvent& event); // Function for Pause button
    void OnMultiplayer(wxCommandEvent& event); // Function for Multiplayer button
    void OnQuit(wxCommandEvent& event); // Function for Quit Game button

    void Host(); // Host a game
    void Join(); // Join a game

protected:
    void OnPaint(wxPaintEvent& event); // Event handler for paint events
    void OnKeyDown(wxKeyEvent& event); // Event handler for key down events
    void OnKeyUp(wxKeyEvent& event); // Event handler for key up events
    void OnTimer(wxTimerEvent& event); // Event handler for timer events

    DECLARE_EVENT_TABLE()
};

TPosition getPreviewPos(int type); // Get position for the preview of nex tetromino
void drawRectangle(int x, int y, wxDC& dc); // Simpler draw rectagle function