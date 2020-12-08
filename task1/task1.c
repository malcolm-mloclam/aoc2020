#include <stdlib.h>
#include <stdio.h>

int main (int argc, char* argv[]) {
    int* input = malloc(sizeof(int));
    FILE* inputFile = fopen("task1input.txt", "r");
    int buffer;
    int size = 0;

    if (inputFile == NULL) {
        perror("Unable to open input file");
        return 1;
    }

    while (!feof(inputFile) && fscanf(inputFile, "%d", &buffer)) {
        input[size] = buffer;

        if (!feof(inputFile)) {
            input = realloc(input, sizeof(int) * (size + 1));
        }

        size += 1;
    }

    // This is pretty stupid

    // Part 1
    for (int i = 0; i < size - 2; i++) {
        for (int j = i + 1; j < size - 1; j++) {
            if (input[j] + input[i] == 2020) {
                printf("%d\n", input[i] * input[j]);
                return 0;
            }
        }
    }

    // Part 2
    for (int i = 0; i < size - 2; i++) {
        for (int j = i + 1; j < size - 1; j++) {
            for (int k = j + 1; k < size; k++) {
                if (input[j] + input[i] + input[k] == 2020) {
                    printf("%d\n", input[i] * input[j] * input[k]);
                    return 0;
                }
            }
        }
    }

    return 0;
}