# 2048 Console Game in C++

A lightweight C++ console implementation of the popular 2048 puzzle game. This version features customizable board sizes and persistent leaderboards.



## 🚀 Features

### Main Menu
* **Start Game:** Jump straight into the action.
* **Leaderboard:** View the top performers for each board size.
* **Exit:** Safely close the application.

### Gameplay
* **Custom Board Sizes:** Choose any size from **4×4 up to 10×10**.
* **Player Profiles:** Supports player names up to 100 characters (including spaces).
* **Intuitive Controls:**
    * `W` → Up
    * `A` → Left
    * `S` → Down
    * `D` → Right
* **Logic:** Standard 2048 rules—new tiles (2 or 4) spawn randomly after each move. The game ends when no valid moves remain.
* **Scoring:** Your score is the sum of all tiles currently on the board.

### Persistent Leaderboards
* **Top 5 Tracking:** Stores the five highest scores for every board size.
* **Dynamic Updates:** High scores are automatically inserted in the correct rank.
* **File Storage:** Data is saved in separate files (e.g., `leaderboard_4x4.txt`) so your progress persists after closing the game.

### Robustness
* **Input Validation:** Gracefully handles non-numeric menu inputs and invalid board sizes.
* **Clean UI:** Uses screen clearing to maintain a smooth console experience.
