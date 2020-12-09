#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main (int argc, char* argv[]) {
    FILE *inputFile = fopen("task5input.txt", "r");
    char* buffer;
    size_t n;

    double row;
    double col;
    double maxid = 0;
    double start = 1;
    double end = 128;

    while (getline(&buffer, &n, inputFile) != -1) {
        start = 1;
        end = 128;

        row = 0;
        col = 0;

        buffer[strcspn(buffer, "\n")] = 0;

        for (int i = 0; i < 6; i++) {
            if (buffer[i] == 'F') {
                if (start > 1) {
                    end = start + ((end - start) / 2);
                } else {
                    end /= 2;
                }
            } else if (buffer[i] == 'B') {
                if (start > 1) {
                    start = start + ((end - start) / 2);
                } else {
                    start = end / 2;
                }
            }

            //printf("start = %f ; end = %f\n", start, end);
        }
        //printf("\n\n");
        if (buffer[6] == 'F') {
            row = start + 1;
        } else {
            row = end - 1;
        }

        start = 1;
        end = 8;

        for (int i = 7; i < 9; i++) {
            if (buffer[i] == 'R') {
                if (start > 1) {
                    end = start + ((end - start) / 2);
                } else {
                    end /= 2;
                }
            } else if (buffer[i] == 'L') {
                if (start > 1) {
                    start = start + ((end - start) / 2);
                } else {
                    start = end / 2;
                }
            }

            //printf("start = %f ; end = %f\n", start, end);
        }
        //printf("\n\n");
        if (buffer[9] == 'R') {
            col = end - 1;
        } else {
            col = start + 1;
        }

        //printf("%s :: row = %f ; col = %f\n", buffer, row, col);
        if (maxid < (row * 8) + col) {
            maxid = (row * 8) + col;
        }
    }

    printf("maxid = %f\n", maxid);
    return 0;
}