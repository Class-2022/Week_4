#include <iostream>
#include <random>
#include <vector>
#include <limits>

using namespace std;

class TicTacToe {
public:
    static const int BOARD_SIZE = 3;

    char board[BOARD_SIZE][BOARD_SIZE] = {
        {' ', ' ', ' '},
        {' ', ' ', ' '},
        {' ', ' ', ' '}
    };

    enum Entry {
        EMPTY = ' ',
        PLAYER_ONE = 'X',
        PLAYER_TWO = 'O'
    };

    bool isFull() const {
        for (int i = 0; i < BOARD_SIZE; i++) {
            for (int j = 0; j < BOARD_SIZE; j++) {
                if (board[i][j] == Entry::EMPTY)
                    return false;
            }
        }
        return true;
    }

    bool isVictory(Entry e) const {
        return (checkRowsForWin(e) || checkColsForWin(e) || checkDiagonalsForWin(e));
    }

    void displayBoard() const {
        cout << "-------------------" << endl;
        for (int i = 0; i < BOARD_SIZE; i++) {
            for (int j = 0; j < BOARD_SIZE; j++) {
                cout << " | " << board[i][j] << " | ";
            }
            cout << endl;
        }
        cout << "-------------------" << endl;
    }

    bool makeMove(int row, int col, Entry player) {
        if (row >= 0 && row < BOARD_SIZE && col >= 0 && col < BOARD_SIZE && board[row][col] == Entry::EMPTY) {
            board[row][col] = static_cast<char>(player);
            return true;
        }
        return false;
    }

private:
    bool checkRowsForWin(Entry e) const {
        for (int i = 0; i < BOARD_SIZE; i++) {
            if (board[i][0] == static_cast<char>(e) && board[i][1] == static_cast<char>(e) && board[i][2] == static_cast<char>(e))
                return true;
        }
        return false;
    }

    bool checkColsForWin(Entry e) const {
        for (int i = 0; i < BOARD_SIZE; i++) {
            if (board[0][i] == static_cast<char>(e) && board[1][i] == static_cast<char>(e) && board[2][i] == static_cast<char>(e))
                return true;
        }
        return false;
    }

    bool checkDiagonalsForWin(Entry e) const {
        return ((board[0][0] == static_cast<char>(e) && board[1][1] == static_cast<char>(e) && board[2][2] == static_cast<char>(e)) ||
            (board[0][2] == static_cast<char>(e) && board[1][1] == static_cast<char>(e) && board[2][0] == static_cast<char>(e)));
    }
};

static bool getHumanMove(TicTacToe::Entry human, TicTacToe& t) {
    int rowChoice, colChoice;

    do {
        cout << "Enter column number (0-" << TicTacToe::BOARD_SIZE - 1 << "): ";
        if (!(cin >> colChoice)) {
            cout << "Invalid input! Please enter a valid number." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        cout << "Enter row number (0-" << TicTacToe::BOARD_SIZE - 1 << "): ";
        if (!(cin >> rowChoice)) {
            cout << "Invalid input! Please enter a valid number." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        if (colChoice >= 0 && colChoice < TicTacToe::BOARD_SIZE && rowChoice >= 0 && rowChoice < TicTacToe::BOARD_SIZE && t.board[rowChoice][colChoice] == TicTacToe::EMPTY) {
            t.board[rowChoice][colChoice] = static_cast<char>(human);
            return false;
        }
        else {
            cout << "Invalid move! Please try again..." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    } while (true);

    return false;
}


static bool getComputerMove(TicTacToe::Entry computer, TicTacToe& t) {
    // Create a random number generator and distribution
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(0, TicTacToe::BOARD_SIZE - 1);

    // a) Check if the computer can win immediately
    for (int row = 0; row < TicTacToe::BOARD_SIZE; ++row) {
        for (int col = 0; col < TicTacToe::BOARD_SIZE; ++col) {
            if (t.board[row][col] == TicTacToe::EMPTY) {
                t.board[row][col] = static_cast<char>(computer);
                if (t.isVictory(computer))
                    return true; // Computer wins
                t.board[row][col] = TicTacToe::EMPTY;
            }
        }
    }

    // b) Check if the human's next move would result in a victory and block it
    for (int row = 0; row < TicTacToe::BOARD_SIZE; ++row) {
        for (int col = 0; col < TicTacToe::BOARD_SIZE; ++col) {
            if (t.board[row][col] == TicTacToe::EMPTY) {
                t.board[row][col] = static_cast<char>(t.PLAYER_ONE);
                if (t.isVictory(t.PLAYER_ONE)) {
                    t.board[row][col] = static_cast<char>(computer); // Block the human's winning move
                    return false; // Continue the game
                }
                t.board[row][col] = TicTacToe::EMPTY;
            }
        }
    }

    // c) Check if the center square is open and choose it
    if (t.board[1][1] == TicTacToe::EMPTY) {
        t.board[1][1] = static_cast<char>(computer);
        return false; // Continue the game
    }

    // d) Check if any of the corner squares are open and choose one of them
    vector<pair<int, int>> availableCorners;
    int corners[4][2] = { {0, 0}, {0, TicTacToe::BOARD_SIZE - 1}, {TicTacToe::BOARD_SIZE - 1, 0}, {TicTacToe::BOARD_SIZE - 1, TicTacToe::BOARD_SIZE - 1} };
    for (int i = 0; i < 4; ++i) {
        int row = corners[i][0];
        int col = corners[i][1];
        if (t.board[row][col] == TicTacToe::EMPTY) {
            availableCorners.push_back(make_pair(row, col));
        }
    }

    if (!availableCorners.empty()) {
        // Randomly select one of the available corners
        int randomIndex = dis(gen);
        int row = availableCorners[randomIndex].first;
        int col = availableCorners[randomIndex].second;
        t.board[row][col] = static_cast<char>(computer);
        return false; // Continue the game
    }

    // e) Choose any open square at random
    vector<pair<int, int>> availableSquares;
    for (int row = 0; row < TicTacToe::BOARD_SIZE; ++row) {
        for (int col = 0; col < TicTacToe::BOARD_SIZE; ++col) {
            if (t.board[row][col] == TicTacToe::EMPTY) {
                availableSquares.push_back(make_pair(row, col));
            }
        }
    }

    if (!availableSquares.empty()) {
        // Randomly select one of the available squares
        int randomIndex = dis(gen);
        int row = availableSquares[randomIndex].first;
        int col = availableSquares[randomIndex].second;
        t.board[row][col] = static_cast<char>(computer);
    }

    return false; // Continue the game
}

int main() {
    TicTacToe t;
    TicTacToe::Entry human = TicTacToe::PLAYER_ONE;
    TicTacToe::Entry computer = TicTacToe::PLAYER_TWO;

    cout << "You are '" << static_cast<char>(human) << "', "
        << "the computer is: '" << static_cast<char>(computer) << "'" << endl << endl;

    while (!t.isFull() && !t.isVictory(human) && !t.isVictory(computer)) {
        t.displayBoard();
        if (getHumanMove(human, t)) {
            cout << "Invalid move! Please try again..." << endl;
            continue;
        }

        if (!t.isFull() && !t.isVictory(human)) {
            if (getComputerMove(computer, t)) {
                cout << "Computer Victory" << endl;
                break;
            }
        }
    }

    t.displayBoard();

    cout << endl;

    if (t.isVictory(human))
        cout << "Human Victory" << endl;
    else if (t.isFull())
        cout << "It's a Tie" << endl;

    return 0;
}
