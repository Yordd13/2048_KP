#include <iostream>
#include <fstream>
#include <iomanip> 
#include <random>
#include <ctime>

const size_t NAME_LENGTH = 100;
const size_t FILE_NAME_LENGTH = 50;
const int MIN_BOARD = 4;
const int MAX_BOARD = 10;
const int WIN_TILE = 2048;
const char TERMINATE_ZERO = '\0';

void clearTerminal() {
    system("cls");
}

void pauseTerminal() {
    std::cout << "Press Enter to continue...";
    std::cin.ignore(1000, '\n');
    std::cin.get();
}

void initializeBoard(int board[MAX_BOARD][MAX_BOARD], int size) {
    for (int row = 0; row < size; row++) {
        for (int col = 0; col < size; col++) {
            board[row][col] = 0;
        }
    }
}

void printBoard(int board[MAX_BOARD][MAX_BOARD], int size, int score, bool gameOver) {
    std::cout << "\n";
    for (int row = 0; row < size; row++) {
        for (int col = 0; col < size; col++) {
            std::cout << std::setw(4) << board[row][col];
        }
        std::cout << "\n\n";
    }
    std::cout << "\nScore: " << score << "\n\n";

    if (!gameOver) {
        std::cout << "Enter a move(w,a,s,d): ";
    }
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

int myStrlen(const char* str) {
    int len = 0;
    while (str[len] != TERMINATE_ZERO) {
        len++;
    }
    return len;
}

void myStrcpy(char* dest, const char* src) {
    int i = 0;
    while (src[i] != TERMINATE_ZERO) {
        dest[i] = src[i];
        i++;
    }
    dest[i] = TERMINATE_ZERO;
}

void myStrcat(char* dest, const char* src) {
    int destLen = myStrlen(dest);
    int i = 0;
    while (src[i] != TERMINATE_ZERO) {
        dest[destLen + i] = src[i];
        i++;
    }
    dest[destLen + i] = TERMINATE_ZERO;
}

void myIntToString(int num, char* out) {
    int i = 0;
    if (num >= 10) {
        out[i++] = (num / 10) + '0';
        out[i++] = (num % 10) + '0';
    }
    else {
        out[i++] = num + '0';
    }
    out[i] = TERMINATE_ZERO;
}

void getFilename(int size, char* filename) {

    char sizeStr[3];
    if (size >= 10) {
        sizeStr[0] = (size / 10) + '0';
        sizeStr[1] = (size % 10) + '0';
        sizeStr[2] = TERMINATE_ZERO;
    }
    else {
        sizeStr[0] = size + '0';
        sizeStr[1] = TERMINATE_ZERO;
    }

    myStrcpy(filename, "leaderboard_");
    myStrcat(filename, sizeStr);
    myStrcat(filename, "x");
    myStrcat(filename, sizeStr);
    myStrcat(filename, ".txt");
}

void updateLeaderboard(int boardSize, const char* playerName, int finalScore) {
    char filename[FILE_NAME_LENGTH];
    getFilename(boardSize, filename);

    char names[6][NAME_LENGTH];
    int scores[6];
    int count = 0;

    std::ifstream inFile(filename);
    if (inFile.is_open()) {
        while (count < 5 && inFile.getline(names[count], NAME_LENGTH)) {
            if (inFile >> scores[count]) {
                inFile.ignore(1000, '\n');
                count++;
            }
        }
        inFile.close();
    }

    myStrcpy(names[count], playerName);
    scores[count] = finalScore;
    count++;

    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (scores[j] < scores[j + 1]) {
                int tempScore = scores[j];
                scores[j] = scores[j + 1];
                scores[j + 1] = tempScore;

                char tempName[NAME_LENGTH];
                myStrcpy(tempName, names[j]);
                myStrcpy(names[j], names[j + 1]);
                myStrcpy(names[j + 1], tempName);
            }
        }
    }

    std::ofstream outFile(filename);
    if (outFile.is_open()) {
        int leaderboardPlayers = (count > 5) ? 5 : count;
        for (int i = 0; i < leaderboardPlayers; i++) {
            outFile << names[i] << "\n" << scores[i] << "\n";
        }
        outFile.close();
    }
}

void showLeaderboard(int size) {
    char filename[50];
    getFilename(size, filename);

    std::ifstream inFile(filename);
    clearTerminal();

    std::cout << "--- TOP 5 LEADERBOARD (" << size << "x" << size << ") ---\n";
    std::cout << std::setw(5) << "Pos" << std::setw(20) << "Name" << std::setw(10) << "Score" << "\n";
    std::cout << "-------------------------------------------\n";

    if (!inFile.is_open()) {
        std::cout << "No records yet for this size.\n";
    }
    else {
        char name[NAME_LENGTH];
        int score;
        int pos = 1;
        while (pos <= 5 && inFile.getline(name, NAME_LENGTH)) {
            if (inFile >> score) {
				inFile.ignore(1000, '\n');
                std::cout << std::setw(5) << pos++ << std::setw(20) << name << std::setw(10) << score << "\n";
			}
        }
        inFile.close();
    }

	pauseTerminal();
}

void newGame() {
	clearTerminal(); 

	char playerName[NAME_LENGTH];
    std::cout << "Enter your name: ";
    std::cin.ignore(1000, '\n');
    std::cin.getline(playerName, NAME_LENGTH);


    int boardSize;
    while (true) {
        std::cout << "Enter board size (4 - 10): ";

        if (!(std::cin >> boardSize)) {
            std::cin.clear();
            std::cin.ignore(1000, '\n');
            clearTerminal();
            std::cout << "Invalid input. Please enter a number.\n";
            continue;
        }

        if (boardSize >= MIN_BOARD && boardSize <= MAX_BOARD) {
            break;
        }

        clearTerminal();
        std::cout << "Invalid input. Board size must be between 4 and 10.\n";
    }

    int board[MAX_BOARD][MAX_BOARD];
    int score = 0;
    initializeBoard(board, boardSize);


    spawnTile(board, boardSize);

    bool gameOver = false;

    while (!gameOver) {

        score = calculateTotalScore(board, boardSize);

        clearTerminal();
        std::cout << "Player: " << playerName << "\n";
        printBoard(board, boardSize, score, false);

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
            std::cout << "\nInvalid move!\n";
			pauseTerminal();
            continue;
        }


        if (moved) {
            spawnTile(board, boardSize);

            if (hasWon(board, boardSize)) {
				score = calculateTotalScore(board, boardSize);
                updateLeaderboard(boardSize, playerName, score);

                clearTerminal();
                printBoard(board, boardSize, score, true);
                std::cout << "\nYou win!\n";
				pauseTerminal();
                return;
            }

            if (!canMove(board, boardSize)) {
                clearTerminal();
                printBoard(board, boardSize, score, true);
                std::cout << "\nGame Over!\n";

                pauseTerminal();
                gameOver = true;
            }
        }
    }

    updateLeaderboard(boardSize, playerName, score);
}

void startGame() {
    char choice;

    do {
		clearTerminal();
        std::cout << "1. Start Game\n";
        std::cout << "2. Leaderboard\n";
        std::cout << "3. Exit\n";
        std::cin >> choice;

        switch (choice) {
        case '1':
            newGame();
            break;
        case '2':
            clearTerminal();

            int size;
            std::cout << "Enter board size to view (4-10): ";

            if (!(std::cin >> size)) {
                std::cin.clear();
                std::cout << "Invalid input. Please enter a number.\n";
                pauseTerminal();
                break;
            }

            if (size >= MIN_BOARD && size <= MAX_BOARD) {
                showLeaderboard(size);
            }
            else {
                std::cout << "Invalid size.\n";
                pauseTerminal();
            }
            break;
        case '3':
            return;
        default:
            std::cout << "Invalid input. Try again.\n";
            pauseTerminal();
        }

    } while (choice != '3');
}

int main()
{
    std::srand(static_cast<unsigned int>(std::time(0)));

	startGame();
    
    return 0;
}