#include "Tetromino.hpp"
#include "Setup.hpp"
#include "Board.hpp"
#include "Game.hpp"
#include "Window.hpp"

class MyApp : public wxApp
{
public:
    virtual bool OnInit() {
        Tetris* tetris = new Tetris(wxT("Tetris"));
        tetris->Centre();
        tetris->Show(true);

        return true;
    }

};

BEGIN_EVENT_TABLE(TetrisGame, wxPanel)
    EVT_TIMER(1, TetrisGame::OnTimer)
    EVT_KEY_DOWN(TetrisGame::OnKeyDown)
    EVT_KEY_UP(TetrisGame::OnKeyUp)
    EVT_PAINT(TetrisGame::OnPaint)
    EVT_BUTTON(1, TetrisGame::OnStart)
    EVT_BUTTON(2, TetrisGame::OnMultiplayer)
    EVT_BUTTON(3, TetrisGame::OnPause)
    EVT_BUTTON(4, TetrisGame::OnQuit)
END_EVENT_TABLE()

wxIMPLEMENT_APP(MyApp);