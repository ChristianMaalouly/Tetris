#include "Game.hpp"

// Random value generation setup
random_device dev;
mt19937 rng(dev());
uniform_int_distribution<mt19937::result_type> dist7(1, 7);

// Positions for preview display
TPosition getPreviewPos(int type) {
    switch (type) {
    case 1:
        return { {0, 2}, {1, 2}, {2, 2}, {3, 2} };
    case 2:
        return { {1, 1}, {1, 2}, {2, 1}, {2, 2} };
    case 3:
        return { {1, 2}, {0, 2}, {1, 1}, {2, 2} };
    case 4:
        return { {0, 2}, {1, 2}, {2, 2}, {2, 1} };
    case 5:
        return { {0, 2}, {1, 2}, {2, 2}, {0, 1} };
    case 6:
        return { {0, 1}, {1, 1}, {1, 2}, {2, 2} };
    case 7:
        return { {0, 2}, {1, 2}, {1, 1}, {2, 1} };
    }
}

// Draw rectangle more easily
void drawRectangle(int x, int y, wxDC& dc) {
    dc.DrawRectangle(x * blockSize, y * blockSize, blockSize, blockSize);
}

TetrisGame::TetrisGame(wxFrame* parent) : 
    wxPanel(parent), score(0), level(1), board(Matrix(COLUMNS, vector<int>(LINES, 0))), nextTetromino(dist7(rng)){
    timer = new wxTimer(this, 1);
    InitColors();

    // Connect to the events
    Connect(wxEVT_PAINT, wxPaintEventHandler(TetrisGame::OnPaint));
    Connect(wxEVT_KEY_DOWN, wxKeyEventHandler(TetrisGame::OnKeyDown));
    Connect(wxEVT_TIMER, wxTimerEventHandler(TetrisGame::OnTimer));

    // Set up the buttons
    startButton = new wxButton(this, 1, "Start Game", wxPoint((COLUMNS +  2) * blockSize, 8 * blockSize), wxSize(4 * blockSize, blockSize));
    startButton->SetBackgroundColour(wxColour(0, 255, 255));
    multiplayerButton = new wxButton(this, 2, "Multiplayer", wxPoint((COLUMNS + 2) * blockSize, 10 * blockSize), wxSize(4 * blockSize, blockSize));
    multiplayerButton->SetBackgroundColour(wxColour(255, 255, 0));
    pauseButton = new wxButton(this, 3, "Pause", wxPoint((COLUMNS + 2) * blockSize, 10 * blockSize), wxSize(4 * blockSize, blockSize));
    pauseButton->SetBackgroundColour(wxColour(255, 255, 0));
    pauseButton->Show(false);
    quitButton = new wxButton(this, 4, "Quit Game", wxPoint((COLUMNS + 2) * blockSize, 12 * blockSize), wxSize(4 * blockSize, blockSize));
    quitButton->SetBackgroundColour(wxColour(255, 0, 255));

    startButton->Bind(wxEVT_BUTTON, &TetrisGame::OnStart, this);
    multiplayerButton->Bind(wxEVT_BUTTON, &TetrisGame::OnMultiplayer, this);
    pauseButton->Bind(wxEVT_BUTTON, &TetrisGame::OnPause, this);
    quitButton->Bind(wxEVT_BUTTON, &TetrisGame::OnQuit, this);

    // Initialize the game without starting
    InitBoard();
    NewTetromino();
}

void TetrisGame::Start() {
    // Adjust menu buttons
    startButton->SetLabel("Restart");
    multiplayerButton->Show(false);
    pauseButton->Show(true);
    pauseButton->SetLabel("Pause");

    // Initialize and start the game
    InitBoard();
    NewTetromino();

    timer->Start(1050 - 50 * level); // Move tetromino down every second
    pause = false;
    playing = true;
}

void TetrisGame::InitBoard() {
    // Initialize an empty board
    for (int i = 0; i < COLUMNS; i++) {
        for (int j = 0; j < LINES; j++) {
            board[i][j] = 0;
        }
    }
}

void TetrisGame::InitColors() {
    colors[0] = wxColour(0, 255, 255); // Cyan
    colors[1] = wxColour(255, 255, 0); // Yellow
    colors[2] = wxColour(128, 0, 128); // Purple
    colors[3] = wxColour(255, 165, 0); // Orange
    colors[4] = wxColour(0, 0, 255); // Blue
    colors[5] = wxColour(255, 0, 0); // Red
    colors[6] = wxColour(0, 255, 0); // Green
}

void TetrisGame::NewTetromino() {
    // Set the next tetromino randomly
    type_tetromino t = type_tetromino(nextTetromino);
    nextTetromino = dist7(rng);

    // Check if spawn is blocked, meaning Game Over
    if (!currentTetromino.newTetromino(t, board)) {
        GameOver();
        return;
    }
    Refresh();
    makingNewPiece = false;
}

void TetrisGame::DrawBoard(wxDC& dc) {

    // Draw empty board
    wxBrush brush(wxColour(200, 200, 200)); // Grey background brush
    dc.SetBrush(brush);
    dc.SetPen(*wxTRANSPARENT_PEN);
    dc.DrawRectangle(0, 0, COLUMNS * blockSize, LINES * blockSize); // Draw the board rectangle

    // Fill the board
    for (int i = 0; i < COLUMNS; i++) {
        for (int j = 0; j < LINES; j++) {
            if (board[i][j] != 0) { // If the cell is filled
                wxBrush brush(colors[board[i][j] - 1]); // Set the brush color to the tetromino's color
                dc.SetBrush(brush);
                dc.SetPen(wxPen(wxColour(20, 20, 20))); // Set a dark border around the cell
                drawRectangle(i, j, dc); // Draw the cell
            }
            else {
                wxBrush brush(wxColour(200, 200, 200)); // Grey background brush
                dc.SetBrush(brush);
                dc.SetPen(wxPen(wxColour(180, 180, 180))); // Set a dark border around the cell
                drawRectangle(i, j, dc); // Draw the cell
            }
        }
    }
}

void TetrisGame::DrawTetromino(wxDC& dc) {
    // Grey background brush for the preview of quickdrop
    wxBrush brush(wxColour(200, 200, 200)); 
    dc.SetBrush(brush);
    // QuickDrop preview
    for (auto cell : currentTetromino.quickDrop(board)) {
        dc.SetPen(wxPen(wxColour(20, 20, 20))); // Set a dark border around the cell
        drawRectangle(cell.first, cell.second, dc); // Draw the block empty with border
    }
    // Get the current tetromino's color
    wxBrush newbrush(colors[currentTetromino.getType() - 1]);
    dc.SetBrush(newbrush);
    // Draw each block of the tetromino
    for (auto cell : currentTetromino.getPos()) {
        dc.SetPen(wxPen(wxColour(20, 20, 20))); // Set a dark border around the cell
        drawRectangle(cell.first, cell.second, dc); // Draw the block
    }
}

void TetrisGame::DrawScore(wxDC& dc) {
    // Set the font and text color
    wxFont font(20, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
    dc.SetFont(font);
    dc.SetTextForeground(wxColour(255, 255, 255)); // White color

    // Draw the score and level
    wxString scoreStr = wxString::Format("Score: %d", score);
    wxString levelStr = wxString::Format("Level: %d", level);
    dc.DrawText(scoreStr, 20, (LINES + 1) * blockSize);
    dc.DrawText(levelStr, 20, (LINES + 3) * blockSize);
}

void TetrisGame::DrawNextTetromino(wxDC& dc) {
    // Draw the display for the preview
    wxBrush brush(wxColour(200, 200, 200)); // Grey background brush
    dc.SetBrush(brush);
    dc.SetPen(*wxTRANSPARENT_PEN);
    dc.DrawRectangle((COLUMNS + 2) * blockSize, 3 * blockSize, 4 * blockSize, 4 * blockSize); // Draw the preview rectangle
    dc.DrawText("NEXT", (COLUMNS + 2.3) * blockSize, 1 * blockSize);

    // Draw the next tetromino in the display
    wxBrush newbrush(colors[nextTetromino - 1]);
    dc.SetBrush(newbrush);
    for (auto cell : getPreviewPos(nextTetromino)) {
        dc.SetPen(wxPen(wxColour(20, 20, 20))); // Set a dark border around the cell
        drawRectangle(COLUMNS + 2 + cell.first, 3 + cell.second, dc); // Draw the block
    }
}

void TetrisGame::MoveDown() {
    // If it can't move down, place and clear lines, then spawn the next tetromino
    if (!currentTetromino.moveDown(board)) {
        makingNewPiece = true;
        mergeTetromino(board, currentTetromino);
        ClearLines();
        NewTetromino();
    }
}

void TetrisGame::MoveLeft() {
    currentTetromino.moveLeft(board);
}

void TetrisGame::MoveRight() {
    currentTetromino.moveRight(board);
}

void TetrisGame::Rotate() {
    currentTetromino.rotate(board);
}

void TetrisGame::QuickDrop() {
    // When quick dropped it's instantly placed on the board
    makingNewPiece = true;
    currentTetromino.drop(board);
    mergeTetromino(board, currentTetromino);
    ClearLines();
    NewTetromino();
}

void TetrisGame::ClearLines() {
    // Clear lines and get number of cleared lines for score calculation
    int lines = clearLines(board);
    if (lines > 0) {
        int scoreToAdd = 0;
        switch (lines) {
        case 1:
            scoreToAdd = 40;
            break;
        case 2:
            scoreToAdd = 100;
            break;
        case 3:
            scoreToAdd = 300;
            break;
        case 4:
            scoreToAdd = 1200;
            break;
        }
        score += (scoreToAdd * (level + 1));
        linesCleared += lines;

        // Update level based on the new number of lines cleared
        level = min(15, 1 + (linesCleared / linesPerLevel));
        timer->Start(1050 - 50 * level);
    }
}

void TetrisGame::GameOver() {
    timer->Stop(); // stop the game

    // Adjust the new menu
    startButton->SetLabel("Start Game");
    multiplayerButton->Show(true);
    pauseButton->Show(false);

    wxMessageBox(wxT("Game Over"), wxT("Game Over")); // game over notification
}

void TetrisGame::Pause() {
    // Pause
    if (!pause) {
        timer->Stop();
        pause = true;
        pauseButton->SetLabel("UnPause");
    }
    // Unpause
    else {
        timer->Start(1050 - 50 * level);
        pause = false;
        pauseButton->SetLabel("Pause");
    }
}

void TetrisGame::OnPaint(wxPaintEvent& event) {
    wxPaintDC dc(this);

    // update display each refresh
    DrawBoard(dc);
    DrawScore(dc);
    if (playing) {
        DrawTetromino(dc);
        DrawNextTetromino(dc);
    }
}

void TetrisGame::OnKeyDown(wxKeyEvent& event) {
    int key = event.GetKeyCode();
    if (key == 'p' || key == 'P') {
        Pause();
        return;
    }
    if (pause || makingNewPiece) return;

    switch (event.GetKeyCode())
    {
    case 'A':
        MoveLeft();
        Refresh();
        break;

    case 'D':
        MoveRight();
        Refresh();
        break;

    case 'S':
        MoveDown();
        Refresh();
        break;

    case 'W':
        // Can't hold rotate, each press rotates once
        if (canRotate) {
            canRotate = false;
            Rotate();
            Refresh();
        }
        break;

    case WXK_SPACE:
        QuickDrop();
        Refresh();
        break;
    }
}

void TetrisGame::OnKeyUp(wxKeyEvent& event) {
    if (event.GetKeyCode() == 'W') canRotate = true;
}

void TetrisGame::OnTimer(wxTimerEvent& event) {
    MoveDown();
    Refresh();
}

void TetrisGame::OnStart(wxCommandEvent& event) {
    Start();
    Refresh();

    // Remove focus from button
    startButton->Hide();
    startButton->Show();
}
void TetrisGame::OnPause(wxCommandEvent& event) {
    Pause();
    Refresh();

    // Remove focus from button
    pauseButton->Hide();
    pauseButton->Show();
}
void TetrisGame::OnQuit(wxCommandEvent& event) {
    Tetris* t = dynamic_cast<Tetris*>(this->GetParent());
    t->Close();
}

void TetrisGame::OnMultiplayer(wxCommandEvent& event) {
    return; // disable because not working yet
    if (enet_initialize() != 0) {
        wxMessageBox(wxT("An error occurred while initializing ENet!"), wxT("Error"));
        return;
    }
    atexit(enet_deinitialize);
    //startButton->Hide();
    if (isHost) Host();
    else Join();
}

void TetrisGame::Host() {
    ENetAddress address;

    enet_address_set_host(&address, hostAddress.c_str());
    address.port = PORT;
    host = enet_host_create(&address, 1, 1, 0, 0);    

    if (host == NULL) {
        wxMessageBox(wxT("An error occurred while trying to create an ENet server host."), wxT("Error"));
        return;
    }

    ENetEvent event;
    /* Wait up to 10 seconds for an event. */
    while (true) {
        if (enet_host_service(host, &event, 1000) > 0) {
            switch (event.type) {
            case ENET_EVENT_TYPE_CONNECT:
                multiplayer = true;
                Start();
                return;
            case ENET_EVENT_TYPE_RECEIVE:
                vector<int> data;  // data received from opponent, should contain their board
                int* dataPtr = (int*)event.packet->data;
                int dataSize = event.packet->dataLength / sizeof(int);
                for (int i = 0; i < dataSize; i++) {
                    data.push_back(dataPtr[i]);
                }
                break;
            }
        }
    }

    enet_host_destroy(host);
}

void TetrisGame::Join() {
    ENetAddress address;
    ENetEvent event;

    enet_address_set_host(&address, hostAddress.c_str());
    address.port = PORT;

    host = enet_host_create(nullptr, 1, 2, 0, 0);

    if (host == nullptr) {
        wxMessageBox(wxT("An error occurred while trying to create an ENet client host."), wxT("Error"));
        enet_deinitialize();
        return;
    }

    ENetPeer* server = enet_host_connect(host, &address, 1, 0);
    if (server == NULL) {
        wxMessageBox(wxT("No available peers for initiating an ENet connection!"), wxT("Error"));
    }

    if (!(enet_host_service(host, &event, 5000) > 0 && event.type == ENET_EVENT_TYPE_CONNECT)) {
        enet_peer_reset(server);
        wxMessageBox(wxT("No available peers for initiating an ENet connection!"), wxT("Error"));
        return;
    }
    else {
        wxMessageBox(wxT("Connection succeeded!"), wxT("Yay!"));
    }

    multiplayer = true;
    Start();
}
