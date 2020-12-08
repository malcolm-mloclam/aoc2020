#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

void printbits(int num) {
    for (int i = 0; i < 8; i++) {
        printf("%i", (num & 0x1));
        num = num >> 1;
    }
}

bool isUpperLetter(char sym) {
    if ((int) sym >= 65 && (int) sym <= 90) {
        return 1;
    }

    return 0;
}

bool isLowerLetter(char sym) {
    if ((int) sym >= 97 && (int) sym <= 122) {
        return 1;
    }

    return 0;
}

bool isLetter(char sym) {
    if (isUpperLetter(sym) || isLowerLetter(sym)) {
        return 1;
    }

    return 0;
}

bool isDigit(char sym) {
    if ((int) sym >= 48 && (int) sym <= 57) {
        return 1;
    }

    return 0;
}

int checkPassword(char* pwdDescription, int n) {
    int min = -1;
    int max = -1;
    char mandatory = '\0';

    // printf("pwd = %s\n", pwd);

    char buffer[3] = {0};
    int bufsize = 0;

    bool found = 0;
    int pos = -1;

    int lettercount = 0;

    int result = 0x3;

    for (int i = 0; i < n; i++) {
        if (i > 0) {
            if (isDigit(pwdDescription[i - 1]) && ((int)pwdDescription[i] == 32 || (int)pwdDescription[i] == 45)) {
                if (min < 0) {
                    min = atoi(buffer);
                } else {
                    max = atoi(buffer);
                }

                strcpy(buffer, "");
                bufsize = 0;
            } else if (isDigit(pwdDescription[i])) {
                buffer[bufsize] = pwdDescription[i];
                bufsize += 1;
            } else if (pwdDescription[i] == ':') {
                mandatory = pwdDescription[i - 1];
            } else if (isLowerLetter(pwdDescription[i]) && mandatory != '\0') {
                if (pos < 0) {
                    pos = i;
                }
                
                if (pwdDescription[i] == mandatory) {
                    lettercount += 1;

                    if (i - pos == min - 1 || i - pos == max - 1) {
                        if (found) {
                            result = result & 0x1;
                        } else {
                            found = 1;
                        }
                    }
                }

                
            }
        } else {
            if ((int) pwdDescription[i] >= 48 && (int) pwdDescription[i] <= 57) {
                buffer[bufsize] = pwdDescription[i];
                bufsize += 1;
            }
        }
    }

    if (!found) {
        result = result & 0x1;
    }

    if (lettercount < min || lettercount > max) {
        result = result & 0x2;
    }

    printf("(%d, %d) [%d] %d : ", min, max, pos, lettercount);

    return result;
}

int main (int argc, char* argv[]) {
    char** input = malloc(sizeof(char**));
    int* lengths = malloc(sizeof(int));
    FILE* inputFile = fopen("task2input.txt", "r");
    char* buffer = NULL;
    size_t lineLength = 0;
    size_t n = 0;
    size_t size = 0;

    int wrongs1 = 0;
    int wrongs2 = 0;
    int rights1 = 0;
    int rights2 = 0;

    if (inputFile == NULL) {
        perror("Unable to open input file");
        return 1;
    }

    int result = 0x0;

    while ((lineLength = getline(&buffer, &n, inputFile)) != -1) {
        char** temp = realloc(input, (size + 1) * sizeof(char**));

        if (!temp) {
            free (buffer);
            perror("realloc failed");
            return 1;
        }

        input = temp;

        buffer[strcspn(buffer, "\n")] = 0;

        result = checkPassword(buffer, lineLength);

        printf("%s : %d\n", buffer, result);

        if (result & 0x1) {
            rights1 += 1;
        }  else {
            wrongs1 += 1;
        }

        if (result & 0x2) {
            rights2 += 1;
        } else {
            wrongs2 += 1;
        }

        *(input + size) = malloc((lineLength + 1) * sizeof(char));
        strcpy(*(input + size), buffer);

        int* tempi = realloc(lengths, (size + 1) * sizeof(int));

        if (!tempi) {
            free(buffer);
            perror("realloc failed 2");
            return 1;
        }

        lengths = tempi;
        *(lengths + size) = lineLength;

        size += 1;

        free (buffer);
    }
    
    printf("wrongs1: %d\n", wrongs1);
    printf("wrongs2: %d\n", wrongs2);
    printf("rights1: %d\n", rights1);
    printf("rights2: %d\n", rights2);

    free (lengths);

    for (int i = 0; i < size; i++) {
        free(*(input + i));
    }
    free (input);

    return 0;
}