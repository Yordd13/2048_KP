#include <iostream>
#include <fstream>
#include <iomanip> 

const size_t NAME_LENGTH = 100;
const int MIN_BOARD = 4;
const int MAX_BOARD = 10;

void clearTerminal() {
    system("cls");
}

int** createBoard(int size) {
    int** board = new int* [size];
    for (int i = 0; i < size; i++) {
        board[i] = new int[size] {0};
    }
    return board;
}

void destroyBoard(int** board, int size) {
    for (int i = 0; i < size; i++) {
        delete[] board[i];
    }
    delete[] board;
}

void initializeBoard(int** board, int size) {
    for (int row = 0; row < size; row++) {
        for (int col = 0; col < size; col++) {
            board[row][col] = 0;
        }
    }
}

void printBoard(int** board, int size, int score) {
	clearTerminal();
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

    int** board = createBoard(boardSize);
	int score = 0;

	initializeBoard(board, boardSize);
	printBoard(board, boardSize, score);

    destroyBoard(board, boardSize);
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

	startGame();
    
    return 0;
}