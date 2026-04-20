#include <iostream> // lets you use cout and cin (printing + input)
#include <string>  //lets you use words like "Player X"
#include <vector>  //lets you use lists like history of games
using namespace std; //means “don’t make me type std:: every time

const int SIZE = 3;
const int MAX_CELLS = 9;

// history box to keep track of who won each game
struct MatchRecord {
    string player1;
    string player2;
    string result;
};

// list of all match history (global so menu can access it)
vector<MatchRecord> history;

// Player class to represent a player in game
class Player {
private: //secret box SO only class can access
    char symbol; //X or O
    string name; //player name

public:
    //This creates a player automatically
    Player(char sym = 'X', string n = "Player X") : symbol(sym), name(n) {}

    // Getter methods, look at symbol/name but don’t change it
    char getSymbol() const { return symbol; }
    string getName() const { return name; }
};

// Board class to manage game board
class Board {
private:
    char grid[SIZE][SIZE]; //3X3 grid for visual representation of the board
    int filledCells; // how many spots are already taken
    
public:
    Board() : filledCells(0) {
        for (int i = 0; i < SIZE; i++) {
            for (int j = 0; j < SIZE; j++) {
                grid[i][j] = ' ';
            }
        }
    }

    void drawBoard() const {
        cout << "-------------" << endl;
        for (int i = 0; i < SIZE; i++) {
            cout << "| ";
            for (int j = 0; j < SIZE; j++) {
                cout << grid[i][j] << " | ";
            }
            cout << endl << "-------------" << endl;
        }
    }

    bool isValidMove(int row, int col) const {
        return (row >= 0 && row < SIZE && col >= 0 && col < SIZE && grid[row][col] == ' ');
    }

    void makeMove(int row, int col, char symbol) {
        if (isValidMove(row, col)) {
            grid[row][col] = symbol;
            filledCells++;
        }
    }

    bool checkWin(char symbol) const {
        for (int i = 0; i < SIZE; i++) {
            if (grid[i][0] == symbol && grid[i][1] == symbol && grid[i][2] == symbol)
                return true;
        }

        for (int i = 0; i < SIZE; i++) {
            if (grid[0][i] == symbol && grid[1][i] == symbol && grid[2][i] == symbol)
                return true;
        }

        if (grid[0][0] == symbol && grid[1][1] == symbol && grid[2][2] == symbol)
            return true;

        if (grid[0][2] == symbol && grid[1][1] == symbol && grid[2][0] == symbol)
            return true;

        return false;
    }

    bool isFull() const {
        return filledCells == MAX_CELLS;
    }
};

// Game class to manage the game logic
class TicTacToe {
private:
    Board board;
    Player players[2];
    int currentPlayerIndex;

public:
    TicTacToe() : currentPlayerIndex(0) {

        string name1, name2;

        cout << "Enter Player 1 name (X): ";
        cin >> name1;

        cout << "Enter Player 2 name (O): ";
        cin >> name2;

        players[0] = Player('X', name1);
        players[1] = Player('O', name2);
    }

    Player& getCurrentPlayer() {
        return players[currentPlayerIndex];
    }

    void switchTurn() {
        currentPlayerIndex = (currentPlayerIndex + 1) % 2;
    }

    void play() {
        int row, col;

        cout << "Welcome to Tic-Tac-Toe! Let's play!" << endl;

        while (!board.isFull()) {

            board.drawBoard();

            Player& currentPlayer = getCurrentPlayer();

            while (true) {
                cout << currentPlayer.getName() << " (" << currentPlayer.getSymbol()
                     << "), enter row (1-3) and column (1-3): ";

                cin >> row >> col;

                row--; col--;

                if (board.isValidMove(row, col)) break;

                cout << "Invalid move. Try again." << endl;
            }

            board.makeMove(row, col, currentPlayer.getSymbol());

            if (board.checkWin(currentPlayer.getSymbol())) {
                board.drawBoard();

                cout << currentPlayer.getName() << " wins!" << endl;

                history.push_back({
                    players[0].getName(),
                    players[1].getName(),
                    currentPlayer.getName() + string(" won")
                });

                return;
            }

            switchTurn();
        }

        board.drawBoard();
        cout << "It's a draw!" << endl;

        history.push_back({
            players[0].getName(),
            players[1].getName(),
            "DRAW"
        });
    }

    void showHistory() {
        cout << "\n=== MATCH HISTORY ===\n";

        if (history.empty()) {
            cout << "No games played yet.\n";
            return;
        }

        for (int i = 0; i < history.size(); i++) {
            cout << i + 1 << ". "
                 << history[i].player1
                 << " vs "
                 << history[i].player2
                 << " -> "
                 << history[i].result
                 << endl;
        }
    }
};

// MAIN MENU UI
int main() {

    TicTacToe game;
    int choice;

    while (true) {
        cout << "\n=== TIC TAC TOE MENU ===\n";
        cout << "1. Play Game\n";
        cout << "2. Show History\n";
        cout << "3. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;

        if (choice == 1) {
            game.play();
        }
        else if (choice == 2) {
            game.showHistory();
        }
        else {
            cout << "Goodbye 👋\n";
            break;
        }
    }

    return 0;
}