#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdarg.h>

// #define DEBUG_ON

typedef struct {
    char *color;
    int count;
} BagStat;

typedef struct {
    char *color;
    int count;
    BagStat *stats;
} Bag;

typedef struct {
    int count;
    BagStat *stats;
} BagStatCreateResult;

void dbgmsg(const char *f, ...) {
#ifndef DEBUG_ON
    static bool debugOffMsg = 0;
    
    if (!debugOffMsg) {
        printf("Debug messages are turned off\n");
        debugOffMsg = 1;
    }
#else
    va_list args;
    va_start(args, f);
    vprintf(f, args);
    va_end(args);
#endif
}

BagStatCreateResult createBagStat(char* description) {
    BagStatCreateResult result;
    result.stats = (BagStat*)malloc(sizeof(BagStat));
    char* copy = malloc(strlen(description) + 1);
    strcpy(copy, description);

    char **sifted = realloc(NULL, sizeof(char*));
    char *token;
    int itercount = 2;

    token = strtok(copy, " ");
    sifted[0] = malloc(strlen(token) + 1);
    strcpy(sifted[0], token);

    while (token != NULL) {
        token = strtok(NULL, " ");

        if (token != NULL) {
            sifted = realloc(sifted, itercount * sizeof(char*));
            sifted[itercount - 1] = malloc(strlen(token) + 1);
            strcpy(sifted[itercount - 1], token);
            itercount += 1;
        }
    }

    int stcount = (itercount - 2) / 4;

    for (int i = 0; i < stcount; i++) {
        result.stats = (BagStat*)realloc(result.stats, (i + 1) * sizeof(BagStat));
        result.stats[i].count = atoi(sifted[1 + (i * 4)]);
        result.stats[i].color = realloc(NULL, strlen(sifted[1 + (i * 4) + 1]) + strlen(sifted[1 + (i * 4) + 2]) + 2);
        strcpy(result.stats[i].color, sifted[1 + (i * 4) + 1]);
        strcat(result.stats[i].color, " ");
        strcat(result.stats[i].color, sifted[1 + (i * 4) + 2]);
    }

    for (int i = 0; i < itercount - 2; i++) {
        free(sifted[i]);
    }
    free(sifted);
    free(copy);

    result.count = stcount;
    return result;
}

Bag createBag(char* description) {
    Bag result;
    int spcount = 0;
    int statidx = 0;
    size_t lng = strlen(description);

    for (int i = 0; i < lng && spcount < 3; i++) {
        if (description[i] == ' ') {
            spcount += 1;

            if (spcount == 3) {
                result.color = calloc((i + 1) - 5, sizeof(char));
                strncpy(result.color, description, i - 5);
                strcat(result.color, "\0");
                // dbgmsg("[%d] result.color = %s ; [%s]\n", i, result.color, description);
                statidx = i + 1;
            }
        }
    }

    BagStatCreateResult tempresult = createBagStat(description + statidx);
    result.stats = tempresult.stats;
    result.count = tempresult.count;

    return result;
}

Bag* getBagByColor(Bag* bags, int bagCount, char* color) {
    for (int i = 0; i < bagCount; i++) {
        if (strcmp(bags[i].color, color) == 0) {
            return &bags[i];
        }
    }

    return NULL;
}

int getAllBags(Bag* currentBag, Bag* allBags, int bagCount) {
    int result = 0;
    Bag* nextBag = NULL;
    
    for (int i = 0; i < currentBag->count; i++) {
        result += currentBag->stats[i].count;
        nextBag = getBagByColor(allBags, bagCount, currentBag->stats[i].color);
        result += currentBag->stats[i].count * getAllBags(nextBag, allBags, bagCount);
    }

    return result;
}

void destroyBag(Bag obj) {
    int statlength = obj.count;
    for (int i = 0; i < statlength; i++) {
        free(obj.stats[i].color);
    }
    free(obj.stats);
    free(obj.color);
}

int main (int argc, char* argv[]) {
    FILE *inputFile = fopen("task7input.txt", "r");
    char *buffer = NULL;
    char **entries = NULL;
    size_t n = 0;
    int entryCount = 0;

    const char mybag[] = "shiny gold";

    Bag *bags = NULL;

    while (getline(&buffer, &n, inputFile) != -1) {
        entryCount += 1;
        entries = realloc(entries, entryCount * sizeof(char*));
        buffer[strcspn(buffer, "\n")] = 0;
        entries[entryCount - 1] = realloc(NULL, strlen(buffer) + 1);
        strcpy(entries[entryCount - 1], buffer);
    }

    for (int i = 0; i < entryCount; i++) {
        bags = realloc(bags, sizeof(Bag) * (i + 1));
        bags[i] = createBag(entries[i]);
        free(entries[i]);
    }

    char foundColors[594][100];
    int foundCount = 0;
    int perfectFinds = 0;
    int iterFound = -1;

    bool foundFlag = 0;
    bool alreadyChecked = 0;


#ifdef DEBUG_ON
    for (int i = 0; i < entryCount; i++) {
        dbgmsg("bag: %s (contains %d)\n", bags[i].color, bags[i].count);

        for (int j = 0; j < bags[i].count; j++) {
            dbgmsg("\tstat: %s [%d]\n", bags[i].stats[j].color, bags[i].stats[j].count);
        }

        dbgmsg("\n");
    }
#endif

    int count = 0;
    int totalFound = 0;

    while (iterFound != 0) {
        iterFound = 0;

        for (int c = 0; c < entryCount; c++) {
            alreadyChecked = 0;

            for (int z = 0; z < totalFound; z++) {
                if (strcmp(bags[c].color, foundColors[z]) == 0) {
                    alreadyChecked = 1;
                }
            }
            if (!alreadyChecked) {
                for (int i = 0; i < bags[c].count && !foundFlag; i++) {
                    if (strcmp(bags[c].stats[i].color, mybag) == 0) {
                        //dbgmsg("%s -> found\n", bags[c].stats[i].color);
                        foundFlag = 1;
                        perfectFinds += 1;
                    } else {
                        for (int j = 0; j < totalFound && !foundFlag; j++) {
                            if (strcmp(foundColors[j], bags[c].stats[i].color) == 0) {
                                dbgmsg("found: %s\n", foundColors[j]);
                                foundCount += 1;
                                foundFlag = 1;
                            }
                        }
                    }

                    if (foundFlag) {
                        totalFound = foundCount + perfectFinds;
                        strcpy(foundColors[totalFound - 1], bags[c].color);
                        iterFound += 1;
                    }
                }

                foundFlag = 0;
            }
        }

        for (int i = 0; i < totalFound; i++) {
            dbgmsg("%s\n", foundColors[i]);
        }
        dbgmsg("\n%d\n", iterFound);
    }

    foundCount += perfectFinds;

    printf("bags found = %d\n", foundCount);

    Bag* myBagObject = getBagByColor(bags, entryCount, mybag);
    int allbags = getAllBags(myBagObject, bags, entryCount);
    printf("all bag count = %d\n", allbags);

    for (int i = 0; i < entryCount; i++) {
        destroyBag(bags[i]);
    }

    free(entries);
    free(buffer);
    return 0;
}