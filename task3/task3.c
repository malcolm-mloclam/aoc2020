#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main (int argc, char *argv[]) {
    FILE *inputFile = fopen("task3input.txt", "r");
    char **input = malloc(sizeof(char**));
    char *buffer = NULL;
    size_t n = 0;
    size_t size = 0;
    size_t lng = 0;
    size_t lngsave = 0;

    while ((lng = getline(&buffer, &n, inputFile)) != -1) {
        char** temp = realloc(input, (size + 1) * sizeof(char**));

        if (lngsave == 0) {
            lngsave = lng;
        }

        if (!temp) {
            perror("realloc failed");
            return 1;
        }

        input = temp;

        buffer[strcspn(buffer, "\n")] = 0;

        *(input + size) = malloc((lng + 1) * sizeof(char));
        strcpy(*(input + size), buffer);

        size += 1;
    }

    if (lngsave > 0) {
        lngsave -= 1;
    }

    int right = 0;
    int trees = 0;

    int slopes[5][2] = {
        {1, 1}, {3, 1}, {5, 1}, {7, 1}, {1, 2}
    };

    int sloperesults[5] = {0};
    int mult = 1;

    for (int c = 0; c < 5; c++) {
        right = 0;

        for (int i = 0; i < size; i += slopes[c][1]) {
            if (input[i][right] == '#') {
                sloperesults[c] += 1;
            }

            right += slopes[c][0];

            if (right > (lngsave - 1)) {
                right = right - lngsave;
            }
        }

        mult *= sloperesults[c];
        printf("slope %d = %d\n", (c + 1), sloperesults[c]);
    }

    printf("\nmult = %d\n", mult);

    // printf("%d\n", trees);

    free(buffer);
    free(input);
    return 0;
}