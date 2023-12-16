#include <iostream>
#include <cstdlib>
#include <ctime>
#include <conio.h>
#include <chrono>
#include <thread>

const int BOARD_WIDTH = 10;
const int BOARD_HEIGHT = 20;
const int FALL_DELAY = 500;  // milliseconds

// Tetromino shapes (I, J, L, O, S, T, Z)
const int tetrominoes[7][4][4] = {
    {
        {0, 0, 0, 0},
        {1, 1, 1, 1},
        {0, 0, 0, 0},
        {0, 0, 0, 0}
    },
    {
        {0, 0, 1, 0},
        {1, 1, 1, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0}
    },
    {
        {1, 0, 0, 0},
        {1, 1, 1, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0}
    },
    {
        {1, 1, 0, 0},
        {1, 1, 0, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0}
    },
    {
        {0, 1, 1, 0},
        {1, 1, 0, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0}
    },
    {
        {0, 1, 0, 0},
        {1, 1, 1, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0}
    },
    {
        {1, 1, 0, 0},
        {0, 1, 1, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0}
    }
};

int board[BOARD_HEIGHT][BOARD_WIDTH];

struct Tetromino {
    int shape[4][4];
    int x, y;
};

void initializeBoard() {
    for (int i = 0; i < BOARD_HEIGHT; ++i) {
        for (int j = 0; j < BOARD_WIDTH; ++j) {
            board[i][j] = 0;
        }
    }
}

void drawBoard(const Tetromino& tetromino) {
    system("cls");  // Clear the console

    for (int i = 0; i < BOARD_HEIGHT; ++i) {
        for (int j = 0; j < BOARD_WIDTH; ++j) {
            if (board[i][j] == 0) {
                std::cout << ".";
            }
            else {
                std::cout << "#";
            }
        }
        std::cout << std::endl;
    }

    // Draw the current tetromino
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            if (tetromino.shape[i][j] != 0) {
                int boardX = tetromino.x + j;
                int boardY = tetromino.y + i;
                if (boardY >= 0) {
                    std::cout << "#";
                }
            }
        }
        std::cout << std::endl;
    }
}

void drawTetromino(const Tetromino& tetromino) {
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            if (tetromino.shape[i][j] != 0) {
                board[tetromino.y + i][tetromino.x + j] = 1;
            }
        }
    }
}

void undrawTetromino(const Tetromino& tetromino) {
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            if (tetromino.shape[i][j] != 0) {
                board[tetromino.y + i][tetromino.x + j] = 0;
            }
        }
    }
}

bool isCollision(const Tetromino& tetromino, int xOffset, int yOffset) {
    undrawTetromino(tetromino);  // Clear current position
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            if (tetromino.shape[i][j] != 0) {
                int boardX = tetromino.x + j + xOffset;
                int boardY = tetromino.y + i + yOffset;
                // Check for collisions with the walls and other blocks
                if (boardX < 0 || boardX >= BOARD_WIDTH || boardY >= BOARD_HEIGHT || board[boardY][boardX] != 0) {
                    drawTetromino(tetromino);  // Draw the current tetromino back
                    return true;  // Collision
                }
            }
        }
    }
    drawTetromino(tetromino);  // Draw the current tetromino back
    return false;  // No collision
}

void generateRandomTetromino(Tetromino& tetromino) {
    // Randomly select a tetromino shape
    int shapeIndex = rand() % 7;
    // Copy the shape to the tetromino
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            tetromino.shape[i][j] = tetrominoes[shapeIndex][i][j];
        }
    }
    // Initial position of the tetromino
    tetromino.x = BOARD_WIDTH / 2 - 2;
    tetromino.y = 0;

    // Check for collision with fixed blocks (game over condition)
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            if (tetromino.shape[i][j] != 0) {
                int boardX = tetromino.x + j;
                int boardY = tetromino.y + i;
                // Check for collisions with fixed blocks
                if (boardY >= 0 && board[boardY][boardX] != 0) {
                    std::cout << "Game Over!" << std::endl;
                    exit(0);  // Terminate the program
                }
            }
        }
    }
}


void moveTetromino(Tetromino& tetromino, int xOffset, int yOffset) {
    undrawTetromino(tetromino);  // Clear current position
    tetromino.x += xOffset;
    tetromino.y += yOffset;
    drawTetromino(tetromino);  // Draw in the new position
}

void fixTetromino(const Tetromino& tetromino) {
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            if (tetromino.shape[i][j] != 0) {
                board[tetromino.y + i][tetromino.x + j] = 1;
            }
        }
    }
}

void deleteFullRows() {
    for (int i = BOARD_HEIGHT - 1; i >= 0; --i) {
        bool fullRow = true;
        for (int j = 0; j < BOARD_WIDTH; ++j) {
            if (board[i][j] == 0) {
                fullRow = false;
                break;
            }
        }
        if (fullRow) {
            // Shift rows above down
            for (int k = i; k > 0; --k) {
                for (int j = 0; j < BOARD_WIDTH; ++j) {
                    board[k][j] = board[k - 1][j];
                }
            }
            // Clear the top row
            for (int j = 0; j < BOARD_WIDTH; ++j) {
                board[0][j] = 0;
            }
        }
    }
}

bool isGameOver() {
    for (int j = 0; j < BOARD_WIDTH; ++j) {
        if (board[0][j] == 1) {
            return true;  // Top row is occupied
        }
    }
    return false;
}

// テトリミノとボードを描画する関数
void drawBoardAndTetromino(const Tetromino& currentTetromino) {
    drawBoard(currentTetromino);
    drawTetromino(currentTetromino);
}

int main() {
    initializeBoard();

    Tetromino currentTetromino;
    generateRandomTetromino(currentTetromino);

    std::clock_t lastFallTime = std::clock();

    while (true) {
        std::clock_t currentTime = std::clock();
        if ((currentTime - lastFallTime) * 1000 / CLOCKS_PER_SEC >= FALL_DELAY) {
            // Tetromino falls automatically
            if (!isCollision(currentTetromino, 0, 1)) {
                moveTetromino(currentTetromino, 0, 1);
            }
            else {
                // Tetromino reached the bottom
                fixTetromino(currentTetromino);
                deleteFullRows();  // Check and delete full rows
                if (isGameOver()) {
                    std::cout << "Game Over!" << std::endl;
                    break;
                }
                generateRandomTetromino(currentTetromino);
                moveTetromino(currentTetromino, 0, 1);  // Only one step down when a new tetromino appears
            }
            lastFallTime = currentTime;  // Update lastFallTime
        }

        // Handle user input (move left, right or rotate)
        if (_kbhit()) {
            char key = _getch();
            switch (key) {
            case 'a':
                if (!isCollision(currentTetromino, -1, 0)) {
                    moveTetromino(currentTetromino, -1, 0);
                }
                break;
            case 'd':
                if (!isCollision(currentTetromino, 1, 0)) {
                    moveTetromino(currentTetromino, 1, 0);
                }
                break;
            case 's':
                if (!isCollision(currentTetromino, 0, 1)) {
                    moveTetromino(currentTetromino, 0, 1);
                }
                break;
            case 'w':
                // Rotate clockwise
                Tetromino rotatedTetromino = currentTetromino;
                for (int i = 0; i < 4; ++i) {
                    for (int j = 0; j < 4; ++j) {
                        rotatedTetromino.shape[i][j] = currentTetromino.shape[3 - j][i];
                    }
                }
                if (!isCollision(rotatedTetromino, 0, 0)) {
                    undrawTetromino(currentTetromino);
                    currentTetromino = rotatedTetromino;
                    drawTetromino(currentTetromino);
                }
                break;
            }
        }

        // 0.2秒毎にボードとテトリミノを描画
        drawBoardAndTetromino(currentTetromino);
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }

    return 0;
}
