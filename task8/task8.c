#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define INSTRUCTION_COUNT 683

typedef struct {
    char in[4];
    int op;
    bool exec;
    bool switched; 
} Instruction;

int processInstruction(Instruction *current, int *acc) {
    int step = 1;
    
    if (strcmp(current->in, "jmp") == 0) {
        step = current->op;
    } else if (strcmp(current->in, "acc") == 0) {
        *acc += current->op;
        step = 1;
    }

    current->exec = 1;
    return step;
}

int main (int argc, char* argv[]) {
    Instruction code[INSTRUCTION_COUNT];
    FILE *inputFile = fopen("task8input.txt", "r");
    char *buffer = NULL;
    char *token = NULL;
    char *copy = NULL;
    size_t n;
    int count = 0;

    while (getline(&buffer, &n, inputFile) != -1) {
        copy = realloc(copy, strlen(buffer) + 1);
        strcpy(copy, buffer);
        token = strtok(copy, " ");
        strcpy(code[count].in, token);
        token = strtok(NULL, " ");
        code[count].op = atoi(token);
        code[count].exec = 0;
        code[count].switched = 0;

        count += 1;
    }

    free (copy);

    bool stopFlag = 0;
    bool successFlag = 0;
    int currentInstruction = 0;
    int accumulator = 0;

    while (!stopFlag) {
        currentInstruction += processInstruction(&code[currentInstruction], &accumulator);
        
        if (code[currentInstruction].exec) {
            stopFlag = true;
        }
    }

    printf("accumulator 1 = %d\n", accumulator);

    for (int i = 0; i < count; i++) {
        code[i].exec = 0;
    }

    stopFlag = 0;
    bool doneFlag = 0;
    bool switchHappened = 0;
    int switchIns = -1;
    accumulator = 0;
    currentInstruction = 0;
    char *ibackup = malloc(4);
    int testcount = 0;

    while (!doneFlag) {
        while (!stopFlag && !doneFlag) {
            if (!code[currentInstruction].switched && !switchHappened) {
                if (strcmp(code[currentInstruction].in, "jmp") == 0) {
                    strcpy(ibackup, code[currentInstruction].in);
                    strcpy(code[currentInstruction].in, "nop");
                    code[currentInstruction].switched = 1;
                    switchHappened = 1;
                    switchIns = currentInstruction;
                } else if (strcmp(code[currentInstruction].in, "nop") == 0) {
                    strcpy(ibackup, code[currentInstruction].in);
                    strcpy(code[currentInstruction].in, "jmp");
                    code[currentInstruction].switched = 1;
                    switchHappened = 1;
                    switchIns = currentInstruction;
                }
            }

            currentInstruction += processInstruction(&code[currentInstruction], &accumulator);

            if (currentInstruction >= (count - 1)) {
                doneFlag = 1;
            } else if (code[currentInstruction].exec) {
                stopFlag = 1;
            }
        }

        if (switchIns >= 0) {
            strcpy(code[switchIns].in, ibackup);
        }

        for (int i = 0; i < count; i++) {
            code[i].exec = 0;
        }

        switchIns = -1;
        switchHappened = 0;
        stopFlag = 0;
        currentInstruction = 0;

        if (!doneFlag) {
            accumulator = 0;
        }

        testcount += 1;
    }

    printf("accumulator 2 = %d\n", accumulator);

    free(ibackup);
    return 0;
}