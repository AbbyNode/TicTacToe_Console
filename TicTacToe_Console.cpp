// TicTacToe_Console.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <iomanip>
#include <vector>

using std::cout;
using std::cin;
using std::endl;
using std::flush;
using std::setw;
using std::vector;

enum class Mark : char {
    EMPTY = ' ',
    X = 'x',
    O = 'o'
};

class Box {
private:
    bool filled;
    Mark mark;

public:
    Box() {
        filled = false;
        mark = Mark::EMPTY;
    }

    bool isEmpty() {
        return !filled;
    }

    void setMark(Mark mark) {
        this->mark = mark;
        filled = true;
    }

    Mark getMark() {
        return mark;
    }

    void clear() {
        mark = Mark::EMPTY;
        filled = false;
    }
};

class Board {
private:
    Box boxes[9];
    int moves;
    bool gameOver;
    Mark winner;

public:
    Board() {
        moves = 0;
        gameOver = false;
        winner = Mark::EMPTY;
    }

    bool isSpotEmpty(int spot) {
        return boxes[spot].isEmpty();
    }

    bool isGameOver() {
        return gameOver;
    }

    Mark getWinner() {
        return winner;
    }

    bool attemptMove(Mark mark, int spot) {
        if (gameOver) {
            return false;
        }

        if (!isSpotEmpty(spot)) {
            return false;
        }

        makeMove(mark, spot);
        checkWin(mark, spot);
        return true;
    }

    void displayBoard() {
        cout << "=======" << endl;
        cout << "|";
        for (int i = 0; i < 9; i++) {
            Box& box = boxes[i];

            if (!box.isEmpty()) {
                //cout << static_cast<char>(box.mark);
                switch (box.getMark())
                {
                case Mark::X:
                    cout << "x";
                    break;
                case Mark::O:
                    cout << "o";
                    break;
                default:
                    break;
                }
            }
            else {
                cout << setw(2);
            }
            cout << "|";

            if (i == 2 || i == 5) {
                cout << endl << "|";
            }
        }
        cout << endl << "=======" << endl;
    }

    void restart() {
        for (int i = 0; i < 9; i++) {
            boxes[i].clear();
        }
        moves = 0;
        gameOver = false;
        winner = Mark::EMPTY;
    }

private:
    void makeMove(Mark mark, int spot) {
        boxes[spot].setMark(mark);
        moves++;
    }

    template <typename CheckSpotFunc>
    void checkLine(Mark mark, CheckSpotFunc checkSpotFunc) {
        int markCount = 0;

        for (int i = 0; i < 3; i++) {
            int checkSpot = checkSpotFunc(i);
            //cout << checkSpot << endl;
            if (boxes[checkSpot].getMark() != mark) {
                break;
            }
            else {
                markCount++;
            }
        }

        if (markCount == 3) {
            setGameOver(mark);
            return;
        }
    }

    void checkWin(Mark mark, int spot) {
        int spotRow = spot / 3;
        int spotCol = spot % 3;

        //cout << "col " << spotCol << " row " << spotRow << endl;

        // Check row win
        //int marksOnRow = 0;
        //for (int col = 0; col < 3; col++) {
        //    int checkSpot = (spotRow * 3) + col;
        //    cout << checkSpot << endl;
        //    if (boxes[checkSpot].mark != mark) {
        //        break;
        //    }
        //    else {
        //        marksOnRow++;
        //    }
        //}
        //if (marksOnRow == 3) {
        //    setGameOver(mark);
        //    return;
        //}

        // Check row win
        auto checkSpotRow = [spotRow](int col) { return (spotRow * 3) + col; };
        checkLine(mark, checkSpotRow);

        // Check col win
        //int marksOnCol = 0;
        //for (int row = 0; row < 3; row++) {
        //    int checkSpot = spotCol + (row*3);
        //    //cout << checkSpot << endl;
        //    if (boxes[checkSpot].mark != mark) {
        //        break;
        //    }
        //    else {
        //        marksOnCol++;
        //    }
        //}
        //if (marksOnCol == 3) {
        //    setGameOver(mark);
        //    return;
        //}

        // Check col win
        auto checkSpotCol = [spotCol](int row) { return spotCol + (row * 3); };
        checkLine(mark, checkSpotCol);

        // Check diag win
        if (spotRow == spotCol) {
            //for (int i = 0; i < 3; i++) {
            //    int checkSpot = i * 4;
            //    //cout << checkSpot << endl;
            //}

            auto checkSpotDiag = [](int i) { return i * 4; };
            checkLine(mark, checkSpotDiag);
        }

        // Check anti diag win
        if (spotRow == 2 - spotCol) {
            //cout << spot << " " << spotRow << " " << spotCol << endl;
            //for (int i = 0; i < 3; i++) {
            //    int checkSpot = ;
            //    //cout << checkSpot << endl;
            //}

            auto checkSpotDiag = [](int i) { return (i * 2) + 2; };
            checkLine(mark, checkSpotDiag);
        }
        
        // Check tie
        if (moves >= 9) {
            gameOver = true;
        }
    }

    void setGameOver(Mark winner) {
        gameOver = true;
        this->winner = winner;
    }
};

int generateMoveRandom(Board& board) {
    vector<int> emptySpots;
    int numEmptySpots = 0;

    for (int i = 0; i < 9; i++) {
        if (board.isSpotEmpty(i)) {
            emptySpots.push_back(i);
            numEmptySpots++;
        }
    }

    if (numEmptySpots == 0) {
        return 0;
    }

    int selectedIndex = (rand() % numEmptySpots);
    return emptySpots[selectedIndex];
}

void testRandomMove(Board& board) {
    // Test random move
    for (int i = 0; i < 100; i++) {
        int move = generateMoveRandom(board);
        Mark mark = (i % 2 == 0 ? Mark::X : Mark::O);
        board.attemptMove(mark, move);
        board.displayBoard();
        if (board.isGameOver()) {
            board.restart();
        }
    }
    board.restart();
}

void showGameOver(Board& board, int& playerChoice) {
    cout << "Game over!" << endl;
    switch (board.getWinner()) {
    case Mark::X:
        cout << "You won! :)";
        break;
    case Mark::O:
        cout << "You lost :(";
        break;
    case Mark::EMPTY:
        cout << "It's a tie";
    }

    cout << endl << "Play again? (y/n) :" << endl;
    char input;
    cin >> input;
    if (input == 'y' || input == 'Y') {
        board.restart();
        system("cls");
        cout << "Tic Tac Toe" << endl << "You are X" << endl;
        board.displayBoard();
    }
    else {
        playerChoice = -1;
    }
}

int main()
{
    srand((unsigned)time(NULL)); // init rand

    Board board;
    //board.attemptMove(Mark::X, 0);
    //board.attemptMove(Mark::X, 1);
    //board.attemptMove(Mark::O, 2);
    //board.attemptMove(Mark::X, 3);
    //board.attemptMove(Mark::O, 4);
    //board.attemptMove(Mark::X, 5);
    //board.attemptMove(Mark::X, 6);
    //board.attemptMove(Mark::X, 7);
    //board.attemptMove(Mark::X, 8);
    
    //testRandomMove(board);

    cout << "Tic Tac Toe" << endl << "You are X" << endl;
    board.displayBoard();

    int playerChoice;

    do {
        cout << "Choose a spot (1 - 9), or exit (-1) :" << endl;
        cin >> playerChoice;

        while (cin.fail()) {
            cout << "Invalid input" << endl << "Enter a number:" << endl;
            cin.clear();
            cin.ignore(256, '\n');
            cin >> playerChoice;
        }

        if (playerChoice >= 1 && playerChoice <= 9) {
            bool allowed = board.isSpotEmpty(playerChoice - 1);
            if (allowed) {
                system("cls");

                cout << "Your move" << endl;
                board.attemptMove(Mark::X, playerChoice - 1);
                board.displayBoard();

                if (board.isGameOver()) {
                    showGameOver(board, playerChoice);
                }
                else {
                    cout << "Computer move" << endl;
                    int computerMove = generateMoveRandom(board);
                    board.attemptMove(Mark::O, computerMove);
                    board.displayBoard();
                    if (board.isGameOver()) {
                        showGameOver(board, computerMove);
                    }
                }
            }
        }
        else if (playerChoice != -1) {
            cout << "Invalid choice" << endl;
        }

    } while (playerChoice != -1);
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
