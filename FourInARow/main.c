#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define ROWS 6
#define COLS 7

typedef struct {
    char symbol;
} Schijf;

Schijf* board[ROWS][COLS] = {NULL};

void printBoard() {
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            if (board[i][j] != NULL) {
                printf("%c ", board[i][j]->symbol);
            } else {
                printf(". ");
            }
        }
        printf("\n");
    }
    for (int j = 0; j < COLS; j++) {
        printf("%d ", j);
    }
    printf("\n");
}

bool placeDisc(int col, char symbol) {
    if (col < 0 || col >= COLS) {
        printf("Ongeldige kolom.\n");
        return false;
    }
    for (int i = ROWS - 1; i >= 0; i--) {
        if (board[i][col] == NULL) {
            Schijf* newDisc = (Schijf*)malloc(sizeof(Schijf));
            newDisc->symbol = symbol;
            board[i][col] = newDisc;
            return true;
        }
    }
    printf("Kolom is vol.\n");
    return false;
}

void removeFourInARow() {
    bool found = false;

    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j <= COLS - 4; j++) {
            if (board[i][j] && board[i][j + 1] &&
                board[i][j + 2] && board[i][j + 3]) {
                char symbol = board[i][j]->symbol;
                if (board[i][j + 1]->symbol == symbol &&
                    board[i][j + 2]->symbol == symbol &&
                    board[i][j + 3]->symbol == symbol) {
                    for (int k = 0; k < 4; k++) {
                        free(board[i][j + k]);
                        board[i][j + k] = NULL;
                    }
                    found = true;
                }
            }
        }
    }

    for (int i = 0; i <= ROWS - 4; i++) {
        for (int j = 0; j < COLS; j++) {
            if (board[i][j] && board[i + 1][j] &&
                board[i + 2][j] && board[i + 3][j]) {
                char symbol = board[i][j]->symbol;
                if (board[i + 1][j]->symbol == symbol &&
                    board[i + 2][j]->symbol == symbol &&
                    board[i + 3][j]->symbol == symbol) {
                    for (int k = 0; k < 4; k++) {
                        free(board[i + k][j]);
                        board[i + k][j] = NULL;
                    }
                    found = true;
                }
            }
        }
    }

    if (found) {

        for (int j = 0; j < COLS; j++) {
            for (int i = ROWS - 1; i >= 0; i--) {
                if (board[i][j] == NULL) {
                    for (int k = i - 1; k >= 0; k--) {
                        if (board[k][j] != NULL) {
                            board[i][j] = board[k][j];
                            board[k][j] = NULL;
                            break;
                        }
                    }
                }
            }
        }
    }
}

int main() {
    int currentPlayer = 1;
    char symbols[2] = {'^', '*'};
    int turnCount = 0;

    while (1) {
        printBoard();
        printf("Speler %d (%c), kies een kolom: ", currentPlayer, symbols[currentPlayer - 1]);
        int col;
        scanf("%d", &col);

        if (placeDisc(col, symbols[currentPlayer - 1])) {
            removeFourInARow();
            currentPlayer = 3 - currentPlayer;
            turnCount++;
        }

        if (turnCount >= ROWS * COLS) {
            printf("Het bord is vol. Gelijkspel!\n");
            break;
        }
    }

    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            if (board[i][j] != NULL) {
                free(board[i][j]);
            }
        }
    }

    return 0;
}
