#pragma once

#include "Setup.hpp"
#include "Game.hpp"

class Tetris : public wxFrame {
public:
    Tetris(const wxString& title);
    void OnQuit();
};