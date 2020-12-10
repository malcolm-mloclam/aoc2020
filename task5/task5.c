#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define DEBUG_ON
// #define DEBUG_FIRST_ONLY

int main (int argc, char* argv[]) {
    FILE *inputFile = fopen("task5input.txt", "r");
    char *buffer;
    size_t n;

    double row;
    double col;
    double maxid = 0;
    double start = 1;
    double end = 128;

    float sIds[837];

    int curseat = 0;

    while (getline(&buffer, &n, inputFile) != -1) {
        start = 0;
        end = 128;

        row = 0;
        col = 0;

        buffer[strcspn(buffer, "\n")] = 0;

        for (int i = 0; i < 6; i++) {
            if (buffer[i] == 'F') {
                if (start > 0) {
                    end = start + ((end - start) / 2);
                } else {
                    end /= 2;
                }
            } else if (buffer[i] == 'B') {
                if (start > 0) {
                    start = start + ((end - start) / 2);
                } else {
                    start = end / 2;
                }
            }
#ifdef DEBUG_ON
            printf("start = %f ; end = %f\n", start, end);
#endif
        }
#ifdef DEBUG_ON
        printf("\n\n");
#endif
        if (buffer[6] == 'F') {
            row = start + 1;
        } else {
            row = end;
        }

        row -= 1;
        row = (row == 1) ? 0 : row;

        start = 0;
        end = 8;

        for (int i = 7; i < 9; i++) {
            if (buffer[i] == 'L') {
                if (start > 1) {
                    end = start + ((end - start) / 2);
                } else {
                    end /= 2;
                }
            } else if (buffer[i] == 'R') {
                if (start > 1) {
                    start = start + ((end - start) / 2);
                } else {
                    start = end / 2;
                }
            }
#ifdef DEBUG_ON
            printf("start = %f ; end = %f\n", start, end);
#endif
        }
#ifdef DEBUG_ON
        printf("\n\n");
#endif
        printf("%f ; %f\n\n", start, end);
        if (buffer[9] == 'R') {
            col = end - 1;
        } else {
            col = start;
            col = (col == 1) ? 0 : col;
        }

        // col -= 1;
#ifdef DEBUG_ON
        printf("%s :: row = %f ; col = %f\n", buffer, row, col);
        printf("maxid before = %f\n", maxid);
#endif
        sIds[curseat] = (row * 8) + col;

        if (maxid < (row * 8) + col) {
            maxid = (row * 8) + col;
        }

        curseat += 1;
#ifdef DEBUG_ON
        printf("seat id = %f\n", (row * 8) + col);
#ifdef DEBUG_FIRST_ONLY
        return 0;
#endif
#endif
    }
    
    float mySeat = 0;
    
    for (int i = 0; i < 836; i++) {
        for (int j = i + 1; j < 837; j++) {
            if (sIds[j] < sIds[i]) {
                sIds[i] = sIds[i] + sIds[j];
                sIds[j] = sIds[i] - sIds[j];
                sIds[i] = sIds[i] - sIds[j];
            }
        }
    }

    for (int i = 0; i < 837; i++) {
        if (sIds[i] == (sIds[i + 1] - 2)) {
            mySeat = sIds[i] + 1;
            break;
        }
    }

    printf("=====================================\nmaxid = %f\n", maxid);
    printf("my seat: %f\n", mySeat);
    return 0;
}