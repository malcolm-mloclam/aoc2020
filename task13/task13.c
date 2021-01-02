#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define REALLOC_STEP 10
#define REALLOC_STEP_TIMESTAMP 100
#define MISSING_ID -1

typedef struct {
    int id;
    int *timestamps;
    int maxStamp;
    int timestampCount;
    int index;
    long long currentStamp;
} Bus;

int initBusses(Bus **busses, char *bussesString, int timeMax) {
    char *token = strtok(bussesString, ",");
    int count = 1;
    int allocated = 1;

    while (token != NULL) {
        if (allocated < count) {
            allocated += REALLOC_STEP;
            (*busses) = realloc((*busses), allocated * sizeof(Bus));
        }

        (*busses)[count - 1].timestamps = NULL;
        (*busses)[count - 1].maxStamp = MISSING_ID;
        (*busses)[count - 1].timestampCount = MISSING_ID;
        (*busses)[count - 1].currentStamp = MISSING_ID;

        if (strcmp(token, "x") == 0) {
            (*busses)[count - 1].id = MISSING_ID;
        } else {
            int allocStamps = 0;
            int stampCount = 1;
            int currentStamp = 0;
            
            (*busses)[count - 1].id = atoi(token);     
            (*busses)[count - 1].currentStamp = (*busses)[count - 1].id;

            while (currentStamp < timeMax) {
                if (allocStamps < stampCount) {
                    allocStamps += REALLOC_STEP_TIMESTAMP;
                    (*busses)[count - 1].timestamps = realloc((*busses)[count - 1].timestamps, allocStamps * sizeof(int));
                }

                (*busses)[count - 1].timestamps[stampCount - 1] = currentStamp;
                (*busses)[count - 1].index = count - 1;
                currentStamp += (*busses)[count - 1].id;

                stampCount += 1;
            }

            (*busses)[count - 1].timestampCount = stampCount;
            (*busses)[count - 1].maxStamp = currentStamp;
            (*busses)[count - 1].timestamps = realloc((*busses)[count - 1].timestamps, (stampCount - 1) * sizeof(int));
        }

        token = strtok(NULL, ",");
        count += 1;
    }

    (*busses) = realloc((*busses), (count - 1) * sizeof(Bus));

    return count - 1;
}

void clearBussesMemory(Bus *busses, int count) {
    for (int i = 0; i < count; i++) {
        if (busses[i].id != MISSING_ID) {
            free(busses[i].timestamps);
        }
    }
}

int main (int argc, char *argv[]) {
    FILE *inputFile = fopen("task13input.txt", "r");
    char *buffer = NULL;
    size_t size;
    char *bussesString;

    getline(&buffer, &size, inputFile);
    int timelimit = atoi(buffer);
    getline(&buffer, &size, inputFile);
    bussesString = buffer;
    bussesString[strcspn(bussesString, "\n")] = 0;
    
    Bus *busses = realloc(NULL, sizeof(Bus));
    int totalBusses = initBusses(&busses, bussesString, timelimit);
    int minStamp = -1;
    int foundId = -1;

    for (int i = 0; i < totalBusses; i++) {
        if (i == 0) {
            minStamp = busses[i].maxStamp;
            foundId = busses[i].id;
        } else if (busses[i].maxStamp < minStamp && busses[i].id != MISSING_ID) {
            minStamp = busses[i].maxStamp;
            foundId = busses[i].id;
        }
    }

    int diff = minStamp - timelimit;
    int result1 = diff * foundId;

    printf("(part 1) result = %d\n", result1);

    long long minStamp2 = busses[0].currentStamp;
    bool part2done = 0;
    int timestamp2 = 1;
    long long step = busses[0].id;

    while (!part2done) {
        part2done = 1;

        for (int i = 1; i < totalBusses && part2done; i++) {
            if (busses[i].id != MISSING_ID) {
                if ((minStamp2 + busses[i].index) % busses[i].id != 0) {
                    part2done = 0;
                } else {
                    step = 1;
                    for (int j = 0; j <= i; j++) {
                        if (busses[j].id != MISSING_ID){
                            step *= busses[j].id;
                        }
                    }
                }
            }
        }

        if (!part2done) {
            busses[0].currentStamp += step;
            minStamp2 = busses[0].currentStamp;
        }
    }

    printf("(part 2) result = %lld\n", minStamp2);

    clearBussesMemory(busses, totalBusses);
    free(busses);
    fclose(inputFile);
    return 0;
}
