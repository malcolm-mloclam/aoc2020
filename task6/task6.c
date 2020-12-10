#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define DEBUG_ON

void swap(char **str1_ptr, char **str2_ptr) 
{ 
    char *temp = *str1_ptr; 
    *str1_ptr = *str2_ptr; 
    *str2_ptr = temp; 
} 

void sortWorkaround(char*** input, int count) {
    int max = 0;

    for (int i = 0; i < count - 1; i++) {
        max = 0;

        for (int j = i + 1; j < count; j++) {
            if (strlen((*input)[i]) <= strlen((*input)[j])) {
                swap(input[i], input[j]);
            }
        }
    }
}

int countAnswers(char* input) {
    int lng = strlen(input);

    char *uniques = (char*)calloc(lng, sizeof(char));
    char *ptridx = NULL;

    int idx = lng;
    int ucnt = 0;

    for (int i = 0; i < lng; i++) {
        if ((int)input[i] != 32) {
            ptridx = strchr(uniques, input[i]);

            if (ptridx == NULL) {
                uniques[ucnt] = input[i];
                ucnt += 1;
            }
        }
    }

    free(uniques);
    return ucnt;
}

int sortstr(const void *a, const void *b) {
    char *pa = *(char **) a;
    char *pb = *(char **) b;

    size_t fa = strlen(pa);
    size_t fb = strlen(pb);

    return (fb > fa) - (fb < fa);
}

int countGroups(char **group, int count) {
    int result = 0;
    char *temp = NULL;

    bool consistent = 1;
    bool found = 0;

    qsort(group, count, sizeof(group[0]), sortstr);

    for (int i = 0; i < strlen(group[0]); i++) {
        consistent = 1;

        for (int j = 1; j < count; j++) {
            found = 0;

            for (int k = 0; k < strlen(group[j]); k++) {
                if (group[0][i] == group[j][k]) {
                    found = 1;
                }
            }

            if (!found) {
                consistent = 0;
            }
        }

        if (consistent) {
            result += 1;
        } 
    }

    return result;
}

int main (int argc, char* argv[]) {
    FILE *inputFile = fopen("task6input.txt", "r");
    char *buffer = NULL;
    char *answers = NULL;
    char **groups = NULL;
    size_t n;
    
    bool allocated = 0;
    int result = 0;
    int result2 = 0;

    int totalgroups = 1;
    int gcount = 0;

    int testtest = 0;

    while (getline(&buffer, &n, inputFile) != -1) {
        if (buffer[0] == '\n') {
            allocated = 0;
            result += countAnswers(answers);
            result2 += countGroups(groups, gcount);
            totalgroups += 1;
            gcount = 0;
        } else {
            buffer[strcspn(buffer, "\n")] = 0;
            groups = realloc(groups, (gcount + 1) * sizeof(char*));
            groups[gcount] = malloc(strlen(buffer) + 1);
    
            strcpy(groups[gcount], buffer);
            
            if (!allocated) {
                answers = realloc(answers, strlen(buffer) + 1);
                strcpy(answers, buffer);
                allocated = 1;
            } else {
                answers = realloc(answers, strlen(answers) + strlen(buffer) + 2);
                strcat(answers, " ");
                strcat(answers, buffer);
            }
            
            gcount += 1;
        }

        testtest += 1;
    }
    
    result += countAnswers(answers);
    result2 += countGroups(groups, gcount);
    printf("result = %d\n", result);
    printf("result2 = %d\n", result2);

    free(answers);
    for (int i = 0; i < gcount; i++) {
        free(groups[i]);
    }
    free(groups);

    fclose(inputFile);
    return 0;
}