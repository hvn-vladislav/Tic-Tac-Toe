#include <iostream>
#include <ctime>
#include <Windows.h>
#include <conio.h>
#include <vector>
using namespace std;

const int ROWS = 3;
const int COLS = 3;

// Function to check if someone has won
char checkWin(char matrix[ROWS][COLS]) {
    // Check rows and columns
    for (int i = 0; i < ROWS; i++) {
        if (matrix[i][0] == matrix[i][1] && matrix[i][1] == matrix[i][2])
            return matrix[i][0]; // Row win
        if (matrix[0][i] == matrix[1][i] && matrix[1][i] == matrix[2][i])
            return matrix[0][i]; // Column win
    }
    // Check diagonals
    if (matrix[0][0] == matrix[1][1] && matrix[1][1] == matrix[2][2])
        return matrix[0][0];    
    if (matrix[0][2] == matrix[1][1] && matrix[1][1] == matrix[2][0])
        return matrix[0][2];

    return ' '; // No winner
}

int main()
{
    // Get console handle
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);

    // Hide console cursor
    CONSOLE_CURSOR_INFO curs;
    GetConsoleCursorInfo(h, &curs);
    curs.bVisible = false;
    SetConsoleCursorInfo(h, &curs);

    // Get console screen size
    CONSOLE_SCREEN_BUFFER_INFO screen;
    GetConsoleScreenBufferInfo(h, &screen);

    // Game variables
    COORD coord = { 0, 0 };
    char userKeyChoice = 0;
    int move = 0;
    int activeItem = 0;
    char playerMove = 'X';
    char computerMove = 'O';
    char winner = 'X';
    char key = 'W';
    int chosenItem = -1;
    bool gameRunning = true;
    bool menuActive = true;
    int emptyCount = -1;
    int currentFrame = 0;
    int animationTimer = 0;
    int colorMenuActiveItem = 2;
    int frameDelay = 12;
    // Game board
    char matrix[ROWS][COLS] = {
        {'1', '2', '3'},
        {'4', '5', '6'},
        {'7', '8', '9'}
    };

    srand(time(NULL));

    // Calculate starting position (centered board)
    COORD startPos;
    startPos.Y = screen.srWindow.Bottom / 2 - 5;
    startPos.X = screen.srWindow.Right / 2 - 9;

    // Menu loop
    while (menuActive) {
        // Display menu text
        coord.X = screen.srWindow.Right / 2 - 15;
        coord.Y = screen.srWindow.Bottom / 2 - 2;
        SetConsoleCursorPosition(h, coord);
        cout << "Choose your symbol and press Enter:";

        // Animated selector for X and O
        coord.Y += 2;
        coord.X = screen.srWindow.Right / 2;
        SetConsoleCursorPosition(h, coord);
        if (activeItem == 0) {
            switch (currentFrame) {
            case 0: cout << "> "; break;
            case 1: cout << " >"; break;
            }
        }
        else {
            cout << "  ";
        }
        cout << 'X';
        if (activeItem == 0) {
            switch (currentFrame) {
            case 0: cout << " <"; break;
            case 1: cout << "< "; break;
            }
        }
        else {
            cout << "  ";
        }

        coord.Y++;
        SetConsoleCursorPosition(h, coord);
        if (activeItem == 1) {
            switch (currentFrame) {
            case 0: cout << "> "; break;
            case 1: cout << " >"; break;
            }
        }
        else {
            cout << "  ";
        }
        cout << 'O';
        if (activeItem == 1) {
            switch (currentFrame) {
            case 0: cout << " <"; break;
            case 1: cout << "< "; break;
            }
        }
        else {
            cout << "  ";
        }

        // Detect key press
        if (_kbhit()) {
            key = _getch();
            switch (key) {
            case 'W': case 'w': activeItem = 0; break;
            case 'S': case 's': activeItem = 1; break;
            case 13:
                if (activeItem == 0) { playerMove = 'X'; computerMove = 'O'; }
                else { playerMove = 'O'; computerMove = 'X'; }
                menuActive = false; // Exit menu
                break;
            }
        }

        // Animation
        animationTimer++;
        if (animationTimer >= frameDelay) {
            animationTimer = 0;
            currentFrame = (currentFrame + 1) % 2;
        }

        Sleep(50);
    }

    system("cls");

    // Game loop
    while (gameRunning)
    {
        COORD coord = startPos;

        // Drawing board
        for (int i = 0; i < 2; ++i) {
            SetConsoleCursorPosition(h, coord);
            cout << "     |     |     ";
            coord.Y++;

            SetConsoleCursorPosition(h, coord);
            for (int j = 0; j < 3; ++j) {
                if (j > 0)
                    cout << "|";

                // Setting different colors for X and O
                if (matrix[i][j] == 'X')
                    SetConsoleTextAttribute(h, 10);
                else if (matrix[i][j] == 'O')
                    SetConsoleTextAttribute(h, 12);

                cout << "  " << matrix[i][j] << "  ";

                SetConsoleTextAttribute(h, 15);
            }
            coord.Y++;

            SetConsoleCursorPosition(h, coord);
            cout << "_____|_____|_____";
            coord.Y++;
        }

        SetConsoleCursorPosition(h, coord);
        cout << "     |     |     ";
        coord.Y++;

        SetConsoleCursorPosition(h, coord);
        for (int j = 0; j < 3; ++j) {
            if (j > 0) cout << "|";

            if (matrix[2][j] == 'X')
                SetConsoleTextAttribute(h, 10);
            else if (matrix[2][j] == 'O')
                SetConsoleTextAttribute(h, 12);

            cout << "  " << matrix[2][j] << "  ";

            SetConsoleTextAttribute(h, 15);
        }
        coord.Y++;

        SetConsoleCursorPosition(h, coord);
        cout << "     |     |     ";
        coord.Y += 5;
        coord.X = 38;

        SetConsoleCursorPosition(h, coord);
        cout << "Press Number (1-9) on your keyboard to move.";

        if (move != 9) {
            while (true) {
                userKeyChoice = _getch() - '0';
                if (userKeyChoice >= 1 && userKeyChoice <= 9) {
                    int row = (userKeyChoice - 1) / COLS;
                    int col = (userKeyChoice - 1) % COLS;

                    // Check if cell is empty
                    if (matrix[row][col] == '0' + userKeyChoice) {
                        matrix[row][col] = playerMove;
                        move++;
                        break;
                    }
                    else
                        continue;
                }
            }

            // Check if player won
            winner = checkWin(matrix);
            if (winner != ' ') {
                gameRunning = false;
                break;
            }

            // Computer move (random empty cell)
            vector<pair<int, int>> emptyCells(9);
            emptyCount = 0;

            for (int i = 0; i < ROWS; ++i) {
                for (int j = 0; j < COLS; ++j) {
                    if (matrix[i][j] != 'X' && matrix[i][j] != 'O') {
                        emptyCells[emptyCount++] = { i, j };
                    }
                }
            }

            if (emptyCount > 0) {
                int randIndex = rand() % emptyCount;
                int row = emptyCells[randIndex].first;
                int col = emptyCells[randIndex].second;
                matrix[row][col] = computerMove;
                move++;
            }

            // Check if computer won
            winner = checkWin(matrix);
            if (winner != ' ') {
                gameRunning = false;
                break;
            }
        }
        else
            gameRunning = false; // Board full = tie

        Sleep(50);
    }

    system("cls");

    // Result screen
    while (!gameRunning) {
        COORD coord = startPos;
        coord.Y = screen.srWindow.Bottom / 2 - 2;
        coord.X = screen.srWindow.Right / 2 - 2;
        SetConsoleCursorPosition(h, coord);

        if (winner == playerMove)
            cout << "Win!";
        else if (winner == computerMove)
            cout << "Lose";
        else
            cout << "Tie!";

        coord.Y += 2;
        coord.X -= 9;

        SetConsoleCursorPosition(h, coord);
        cout << "Press X twice to exit.";

        userKeyChoice = _getch();
        if (userKeyChoice == 'X' || userKeyChoice == 'x')
            break;
    }

    return 0;
}