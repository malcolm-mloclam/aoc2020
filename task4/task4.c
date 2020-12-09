#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

bool checkField(char* fieldname, char* value) {
    //printf("%s , %s ", fieldname, value);
    if (strcmp(fieldname, "byr") == 0) {
        int byr = atoi(value);
        if (byr >= 1920 && byr <= 2002) {
            return 1;
        }
    } else if (strcmp(fieldname, "iyr") == 0) {
        int iyr = atoi(value);
        if (iyr >= 2010 && iyr <= 2020) {
            return 1;
        }
    } else if (strcmp(fieldname, "eyr") == 0) {
        int eyr = atoi(value);
        if (eyr >= 2020 && eyr <= 2030) {
            return 1;
        }
    } else if (strcmp(fieldname, "hgt") == 0) {
        size_t idx = strcspn(value, "cm");
        bool iscm = 1;

        if (idx == strlen(value)) {
            idx = strcspn(value, "in");
            iscm = 0;
        }
        
        if (idx != strlen(value)) {
            value[idx] = '\0';
            int height = atoi(value);

            if ((iscm && height >= 150 && height <= 193) || (!iscm && height >= 59 && height <= 76)) {
                return 1;
            }
        }
    } else if (strcmp(fieldname, "hcl") == 0) {
        if (strlen(value) != 7 || value[0] != '#') {
            return 0;
        }

        for (int i = 1; i < strlen(value); i++) {
            if (((int)value[i] < 97 || (int)value[i] > 102) && ((int)value[i] < 48 || (int)value[i] > 57)) {
                return 0;
            }
        }

        return 1;
    } else if (strcmp(fieldname, "ecl") == 0) {
        if (
            strstr(value, "amb") == NULL && strstr(value, "blu") == NULL && strstr(value, "brn") == NULL && 
            strstr(value, "gry") == NULL && strstr(value, "grn") == NULL && strstr(value, "hzl") == NULL &&
            strstr(value, "oth") == NULL
        ) {
            return 0;
        }

        return 1;
    } else if (strcmp(fieldname, "pid") == 0) {
        if (strlen(value) == 9) {
            for (int i = 0; i < 9; i++) {
                if ((int)value[i] < 48 || (int)value[i] > 57) {
                    return 0;
                }
            }
        } else {
            return 0;
        }

        return 1;
    } else if (strcmp(fieldname, "cid") == 0) {
        return 1;
    }

    return 0;
}

int main (int argc, char *argv[]) {
    FILE *inputFile = fopen("task4input.txt", "r");
    char *passport = NULL;
    char *buffer = NULL;
    size_t n;
    size_t lng = 0;

    int valid = 0;
    int valid2 = 0;
    int params = 0;

    bool allocated = 0;

    int times = 0;

    while (lng != -1) {
        lng = getline(&buffer, &n, inputFile);

        params = 0;

        if (buffer[0] == '\n' || lng == -1) {
            allocated = 0;

            // part 1
            for (int i = 0; i < strlen(passport); i++) {
                if (passport[i] == ':') {
                    params += 1;
                }
            }

            if (params == 8 || (params == 7 && strstr(passport, "cid:") == NULL)) {
                // printf("%s : %d\n", passport, params);
                valid += 1;

                // part 2
                char *pcopy = malloc(strlen(passport) + 1);
                strcpy(pcopy, passport);
                char *token = strtok(pcopy, " ");
                char *context = NULL;
                bool validFlag = 1;

                char params[2][20] = {0};
                int curparam = 0;

                while (token != NULL) {
                    char *tokenCopy = malloc(strlen(token) + 1);
                    strcpy(tokenCopy, token);

                    curparam = 0;

                    char *token2 = strtok_r(tokenCopy, ":", &context);
                    
                    while (token2 != NULL) {
                        strcpy(params[curparam], token2);
                        token2 = strtok_r(NULL, ":", &context);
                        curparam += 1;
                    }

                    free(tokenCopy);
                    tokenCopy = NULL;

                    if (!checkField(params[0], params[1])) {
                        validFlag = 0;
                    }

                    token = strtok(NULL, " ");
                }

                free(pcopy);
                pcopy = NULL;

                if (validFlag) {
                    //printf("valid\n");
                    printf("valid: %s\n", passport);
                    valid2 += 1;
                } else {
                    printf("invalid\n");
                }
            } else {
                //printf("invalid: %s\n", passport);
            }
        } else {
            buffer[strcspn(buffer, "\n")] = 0;

            if (!allocated) {
                passport = realloc(passport, strlen(buffer) + 1);
                strcpy(passport, buffer);
                allocated = 1;
            } else {
                passport = realloc(passport, strlen(passport) + strlen(buffer) + 1);
                strcat(passport, " ");
                strcat(passport, buffer);
            }
        }
    }

    free(passport);
    printf("valid passports [1]: %d\n", valid);
    printf("valid passports [2]: %d\n", valid2);

    return 0;
}