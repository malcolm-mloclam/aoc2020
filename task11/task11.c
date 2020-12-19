#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

//#define PART_ONE

#ifndef PART_ONE
#define PART_TWO
#endif

typedef enum {
    FLOOR,
    FREE,
    OCCUPIED,
    NONE
} CellState;

typedef struct {
    CellState state;
    CellState pendingChange;
    int row;
    int col;
} Cell;

CellState parseState(char code) {
    if (code == 'L') {
        return FREE;
    } else if (code == '#') {
        return OCCUPIED;
    } 

    return FLOOR;
}

CellState nextState(Cell **cells, Cell current, int maxRow, int maxCol) {
    if (current.state == FREE) {
        bool noAdjacentOccupied = 1;

        for (int i = -1; i <= 1 && noAdjacentOccupied; i++) {
            for (int j = -1; j <= 1 && noAdjacentOccupied; j++) {
                if ((current.row + i) >= 0 && (current.row + i) < maxRow && (current.col + j) >= 0 && (current.col + j) < maxCol && !(i == 0 && j == 0)) {
                    if (cells[current.row + i][current.col + j].state == OCCUPIED) {
                        noAdjacentOccupied = 0;
                    }
                }
            }
        }

        if (noAdjacentOccupied) {
            return OCCUPIED;
        }
    } else if (current.state == OCCUPIED) {
        int adjacentOccupied = 0;

        for (int i = -1; i <= 1 && adjacentOccupied < 4; i++) {
            for (int j = -1; j <= 1 && adjacentOccupied < 4; j++) {
                if ((current.row + i) >= 0 && (current.row + i) < maxRow && (current.col + j) >= 0 && (current.col + j) < maxCol && !(i == 0 && j == 0)) {
                    if (cells[current.row + i][current.col + j].state == OCCUPIED) {
                        adjacentOccupied += 1;
                    }
                }
            }
        }

        if (adjacentOccupied >= 4) {
            return FREE;
        }
    }

    return NONE;
}

CellState nextVisibleState(Cell **cells, Cell current, int maxRow, int maxCol) {
    int currentRow = 0;
    int currentCol = 0;

    if (current.state == FREE) {
        bool noVisibleOccupied = 1;

        for (int i = -1; i <= 1 && noVisibleOccupied; i++) {
            for (int j = -1; j <= 1 && noVisibleOccupied; j++) {
                currentRow = current.row + i;
                currentCol = current.col + j;

                if (i != 0 || j != 0) {
                    while (currentRow >= 0 && currentRow < maxRow && currentCol >= 0 && currentCol < maxCol && cells[currentRow][currentCol].state == FLOOR) {
                        currentRow += i;
                        currentCol += j;
                    }

                    if (currentRow >= 0 && currentRow < maxRow && currentCol >= 0 && currentCol < maxCol) {
                        if (cells[currentRow][currentCol].state == OCCUPIED) {
                            noVisibleOccupied = 0;
                        }
                    }
                }
            }
        }

        if (noVisibleOccupied) {
            return OCCUPIED;
        }
    } else if (current.state == OCCUPIED) {
        int visibleOccupied = 0;

        for (int i = -1; i <= 1 && visibleOccupied < 5; i++) {
            for (int j = -1; j <= 1 && visibleOccupied < 5; j++) {
                currentRow = current.row + i;
                currentCol = current.col + j;

                if (i != 0 || j != 0) {
                    while (currentRow >= 0 && currentRow < maxRow && currentCol >= 0 && currentCol < maxCol && cells[currentRow][currentCol].state == FLOOR) {
                        currentRow += i;
                        currentCol += j;
                    }

                    if (currentRow >= 0 && currentRow < maxRow && currentCol >= 0 && currentCol < maxCol) {
                        if (cells[currentRow][currentCol].state == OCCUPIED) {
                            visibleOccupied += 1;
                        } 
                    }
                }
            }
        }

        if (visibleOccupied >= 5) {
            return FREE;
        }
    }

    return NONE;
}

void printCells(Cell **cells, int maxRow, int maxCol) {
    for (int i = 0; i < maxRow; i++) {
        for (int j = 0; j < maxCol; j++) {
            if (cells[i][j].state == FREE) {
                printf("L");
            } else if (cells[i][j].state == OCCUPIED) {
                printf("#");
            } else if (cells[i][j].state == FLOOR) {
                printf(".");
            } else {
                printf("?");
            }
        }
        printf("\n");
    }
}

void applyPendingChanges(Cell **cells, int maxRow, int maxCol) {
    for (int i = 0; i < maxRow; i++) {
        for (int j = 0; j < maxCol; j++) {
            if (cells[i][j].pendingChange != NONE) {
                cells[i][j].state = cells[i][j].pendingChange;
                cells[i][j].pendingChange = NONE;
            }
        }
    }
}

void clearCellMemory(Cell **cells, int rows) {
    for (int i = 0; i < rows; i++) {
        free(cells[i]);
    }

    free(cells);
}

int main (int argc, char *argv[]) {
    FILE *inputFile = fopen("task11input.txt", "r");
    char *buffer = NULL;
    size_t size = 0;

    Cell **cells = NULL;

    int rows = 0;
    int cols = 0;

    while (getline(&buffer, &size, inputFile) != -1) {
        rows += 1;
        cells = realloc(cells, sizeof(Cell*) * rows);

        buffer[strcspn(buffer, "\n")] = 0;
        int bufLength = strlen(buffer);
        cols = bufLength;

        cells[rows - 1] = realloc(NULL, sizeof(Cell) * bufLength);

        for (int i = 0; i < bufLength; i++) {
            cells[rows - 1][i].state = parseState(buffer[i]); 
            cells[rows - 1][i].pendingChange = NONE;
            cells[rows - 1][i].row = rows - 1;
            cells[rows - 1][i].col = i;
        }
    }
    
    int pendingChanges = 1;

    while (pendingChanges > 0) {
        pendingChanges = 0;

        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
#ifdef PART_ONE
                cells[i][j].pendingChange = nextState(cells, cells[i][j], rows, cols);
#elif defined PART_TWO
                cells[i][j].pendingChange = nextVisibleState(cells, cells[i][j], rows, cols);
#endif
                if (cells[i][j].pendingChange != NONE) {
                    pendingChanges += 1;
                }
            }
        }

        if (pendingChanges > 0) {
            applyPendingChanges(cells, rows, cols);
        }
    }

    int seatsOccupied = 0;

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (cells[i][j].state == OCCUPIED) {
                seatsOccupied += 1;
            }
        }
    }

#ifdef PART_ONE
    printf("(part 1) seats occupied = %d\n", seatsOccupied);
#elif defined PART_TWO
    printf("(part 2) seats occupied = %d\n", seatsOccupied);
#endif

    clearCellMemory(cells, rows);
    fclose(inputFile);
    return 0;
}