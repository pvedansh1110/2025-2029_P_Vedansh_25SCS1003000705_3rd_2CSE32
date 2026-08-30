#include <iostream>
#include <cstdlib>
using namespace std;

char board[3][3] = {
    {'1', '2', '3'},
    {'4', '5', '6'},
    {'7', '8', '9'}
};

// Display the game board
void displayBoard() {
    cout << "\n";
    cout << "-------------\n";

    for (int i = 0; i < 3; i++) {
        cout << "| ";
        for (int j = 0; j < 3; j++) {
            cout << board[i][j] << " | ";
        }
        cout << "\n-------------\n";
    }
}

// Check if a player has won
bool checkWin(char player) {
    // Check rows
    for (int i = 0; i < 3; i++) {
        if (board[i][0] == player &&
            board[i][1] == player &&
            board[i][2] == player) {
            return true;
        }
    }

    // Check columns
    for (int j = 0; j < 3; j++) {
        if (board[0][j] == player &&
            board[1][j] == player &&
            board[2][j] == player) {
            return true;
        }
    }

    // Check diagonals
    if (board[0][0] == player &&
        board[1][1] == player &&
        board[2][2] == player) {
        return true;
    }

    if (board[0][2] == player &&
        board[1][1] == player &&
        board[2][0] == player) {
        return true;
    }

    return false;
}

// Check whether the board is full
bool checkDraw() {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (board[i][j] != 'X' && board[i][j] != 'O') {
                return false;
            }
        }
    }

    return true;
}

// Make a player's move
bool makeMove(int position, char player) {
    if (position < 1 || position > 9) {
        return false;
    }

    int row = (position - 1) / 3;
    int col = (position - 1) % 3;

    if (board[row][col] == 'X' || board[row][col] == 'O') {
        return false;
    }

    board[row][col] = player;
    return true;
}

// Reset the board for a new game
void resetBoard() {
    char value = '1';

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            board[i][j] = value++;
        }
    }
}

int main() {
    char playAgain;

    do {
        resetBoard();

        char currentPlayer = 'X';
        bool gameOver = false;

        cout << "\n=================================\n";
        cout << "          TIC TAC TOE\n";
        cout << "=================================\n";
        cout << "Player 1: X\n";
        cout << "Player 2: O\n";

        while (!gameOver) {
            displayBoard();

            int position;

            cout << "\nPlayer " << currentPlayer
                 << ", enter a position (1-9): ";
            cin >> position;

            if (!makeMove(position, currentPlayer)) {
                cout << "Invalid move! Choose an empty position from 1 to 9.\n";
                continue;
            }

            // Display the board immediately after every valid move
            displayBoard();

            if (checkWin(currentPlayer)) {
                cout << "\nPlayer " << currentPlayer << " wins!\n";
                gameOver = true;
            }
            else if (checkDraw()) {
                cout << "\nIt's a draw!\n";
                gameOver = true;
            }
            else {
                if (currentPlayer == 'X')
                    currentPlayer = 'O';
                else
                    currentPlayer = 'X';
            }
        }

        cout << "\nDo you want to play again? (Y/N): ";
        cin >> playAgain;

    } while (playAgain == 'Y' || playAgain == 'y');

    cout << "\nThanks for playing Tic Tac Toe!\n";

    return 0;
}
