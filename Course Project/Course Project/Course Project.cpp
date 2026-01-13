#include <iostream>
#include <fstream>
#include <iomanip> 
#include <random>
#include <ctime>

const size_t NAME_LENGTH = 100;
const int MIN_BOARD = 4;
const int MAX_BOARD = 10;
const int WIN_TILE = 2048;

void clearTerminal() {
    system("cls");
}

void initializeBoard(int board[MAX_BOARD][MAX_BOARD], int size) {
    for (int row = 0; row < size; row++) {
        for (int col = 0; col < size; col++) {
            board[row][col] = 0;
        }
    }
}

void printBoard(int board[MAX_BOARD][MAX_BOARD], int size, int score) {
    std::cout << "\n";
    for (int row = 0; row < size; row++) {
        for (int col = 0; col < size; col++) {
            std::cout << std::setw(4) << board[row][col];
        }
        std::cout << "\n\n";
    }
    std::cout << "\nScore: " << score << "\n\n";
    std::cout << "Enter a move(w,a,s,d): ";
}

bool hasWon(int board[MAX_BOARD][MAX_BOARD], int size) {
    for (int row = 0; row < size; row++) {
        for (int col = 0; col < size; col++) {
            if (board[row][col] >= WIN_TILE) return true;
        }
    }
    return false;
}

bool canMove(int board[MAX_BOARD][MAX_BOARD], int size) {

	//empty cell check
    for (int row = 0; row < size; row++) {
        for (int col = 0; col < size; col++) {
            if (board[row][col] == 0) return true;
        }
    }

	//horizontal check
    for (int row = 0; row < size; row++) {
        for (int col = 0; col < size - 1; col++) {
            if (board[row][col] == board[row][col + 1]) return true;
        }
    }

	//vertical check
    for (int row = 0; row < size - 1; row++) {
        for (int col = 0; col < size; col++) {
            if (board[row][col] == board[row + 1][col]) return true;
        }
    }

    return false;
}

bool slideAndMerge(int line[], int size) {
    int compact[MAX_BOARD];
    int compactLen = 0;

    for (int i = 0; i < size; i++) {
        if (line[i] != 0) {
            compact[compactLen++] = line[i];
        }
    }

    int merged[MAX_BOARD] = { 0 };
    int mlen = 0;
    int i = 0;
    while (i < compactLen) {
        if (i + 1 < compactLen && compact[i] == compact[i + 1]) {
            int val = compact[i] * 2;
            merged[mlen++] = val;
            i += 2;
        }
        else {
            merged[mlen++] = compact[i];
            i += 1;
        }
    }

    for (int k = mlen; k < size; k++) merged[k] = 0;

    bool changed = false;
    for (int k = 0; k < size; k++) {
        if (line[k] != merged[k]) {
            line[k] = merged[k];
            changed = true;
        }
    }

    return changed;
}

bool moveLeft(int board[MAX_BOARD][MAX_BOARD], int size) {
     bool moved = false;
     int line[MAX_BOARD];

     for (int row = 0; row < size; row++) {
         for (int col = 0; col < size; col++) {
             line[col] = board[row][col];
         }

         if (slideAndMerge(line, size)) {
             moved = true;
             for (int col = 0; col < size; col++) {
                 board[row][col] = line[col];
             }
         }
     }
     return moved;
}

bool moveRight(int board[MAX_BOARD][MAX_BOARD], int size) {
    bool moved = false;
    int line[MAX_BOARD];

    for (int row = 0; row < size; row++) {
        for (int col = 0; col < size; col++) {
            line[col] = board[row][size - 1 - col];
        }

        if (slideAndMerge(line, size)) {
            moved = true;
            for (int col = 0; col < size; col++) {
                board[row][size - 1 - col] = line[col];
            }
        }
    }
    return moved;
}

bool moveUp(int board[MAX_BOARD][MAX_BOARD], int size) {
    bool moved = false;
    int line[MAX_BOARD];

    for (int col = 0; col < size; col++) {
        for (int row = 0; row < size; row++) {
            line[row] = board[row][col];
        }

        if (slideAndMerge(line, size)) {
            moved = true;
            for (int row = 0; row < size; row++) {
                board[row][col] = line[row];
            }
        }
    }
    return moved;
}

bool moveDown(int board[MAX_BOARD][MAX_BOARD], int size) {
    bool moved = false;
    int line[MAX_BOARD];

    for (int col = 0; col < size; col++) {
        for (int row = 0; row < size; row++) {
            line[row] = board[size - 1 - row][col];
        }

        if (slideAndMerge(line, size)) {
            moved = true;
            for (int row = 0; row < size; row++) {
                board[size - 1 - row][col] = line[row];
            }
        }
    }
    return moved;
}


bool spawnTile(int board[MAX_BOARD][MAX_BOARD], int size) {
    const int MAX_CELLS = MAX_BOARD * MAX_BOARD;
    int emptyR[MAX_CELLS];
    int emptyC[MAX_CELLS];
    int emptyCount = 0;

    for (int row = 0; row < size; row++) {
        for (int col = 0; col < size; col++) {
            if (board[row][col] == 0) {
                emptyR[emptyCount] = row;
                emptyC[emptyCount] = col;
                emptyCount++;
            }
        }
    }

    if (emptyCount == 0) return false;

    int pick = std::rand() % emptyCount;
    int value = ((std::rand() % 2) * 2) + 2;

    board[emptyR[pick]][emptyC[pick]] = value;

    return true;
}

int calculateTotalScore(int board[MAX_BOARD][MAX_BOARD], int size) {
    int total = 0;
    for (int row = 0; row < size; row++) {
        for (int col = 0; col < size; col++) {
            total += board[row][col];
        }
    }
    return total;
}

void newGame() {
	clearTerminal(); 

	char playerName[NAME_LENGTH];
    std::cout << "Enter your name: ";
    std::cin.ignore(1000, '\n');
    std::cin.getline(playerName, NAME_LENGTH);


    int boardSize;
    do {
        std::cout << "Enter board size (4 - 10): ";
        std::cin >> boardSize;

        if (boardSize < MIN_BOARD || boardSize > MAX_BOARD) {
            clearTerminal();
            std::cout << "Invalid input. Board size must be between 4 and 10.\n";
        }

    } while (boardSize < MIN_BOARD || boardSize > MAX_BOARD);

    int board[MAX_BOARD][MAX_BOARD];
    int score = 0;
    initializeBoard(board, boardSize);


    spawnTile(board, boardSize);

    bool gameOver = false;

    while (!gameOver) {

        score = calculateTotalScore(board, boardSize);

        clearTerminal();
        std::cout << "Player: " << playerName << "\n";
        printBoard(board, boardSize, score);

		//TODO: check for valid input
        char move;
        std::cin >> move;

        bool moved = false;
        switch (move) {
        case 'w':
            moved = moveUp(board, boardSize);
            break;
        case 'a':
            moved = moveLeft(board, boardSize);
            break;
        case 's':
            moved = moveDown(board, boardSize);
            break;
        case 'd':
            moved = moveRight(board, boardSize);
            break;
        default:
			//TODO: logic if invalid input
            continue;
        }


        if (moved) {
            spawnTile(board, boardSize);

            if (hasWon(board, boardSize)) {
                clearTerminal();
                printBoard(board, boardSize, score);
                std::cout << "\nYou win!\n";
                //TODO: leaderboard logic and return to menu
                return;
            }

            if (!canMove(board, boardSize)) {
                clearTerminal();
                printBoard(board, boardSize, score);
                std::cout << "\nGame Over!\n";
				//TODO: leaderboard logic and return to menu
                gameOver = true;
            }
        }
    }
}

void startGame() {
    int choice;

    do {
        std::cout << "1. Start Game\n";
        std::cout << "2. Leaderboard\n";
        std::cout << "3. Exit\n";
        std::cin >> choice;

        switch (choice) {
        case 1:
            newGame();
            break;
        case 2:
            //showLeaderboard();
            break;
        case 3:
            //exitGame();
            break;
        default:
            clearTerminal();
            std::cout << "Invalid input. Try again.\n";
        }

    } while (choice != 3);
}

int main()
{
    std::srand(static_cast<unsigned int>(std::time(0)));

	startGame();
    
    return 0;
}