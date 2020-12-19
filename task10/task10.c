#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define CACHING_ON

typedef struct {
    int joltage;
    bool connected;
    unsigned long long cache;
} Adapter;

int compareAdapters(const void *a, const void *b) {
    Adapter *first = (Adapter*) a;
    Adapter *second = (Adapter*) b;
    return (first->joltage - second->joltage);
}

void clearAdapters(Adapter *adapters, int count) {
    for (int i = 0; i < count; i++) {
        adapters[i].connected = 0;
    }
}

void printAdapters(Adapter *adapters, int count) {
    for (int i = 0; i < count; i++) {
        printf("adapter[%d].joltage = %d\n", i, adapters[i].joltage);
    }
}

int findAdapter(Adapter *adapters, int count, int joltage) {
    for (int i = 0; i < count; i++) {
        if (adapters[i].joltage == joltage && !adapters[i].connected) {
            adapters[i].connected = 1;
            return i;
        }
    }

    return -1;
}

unsigned long long traverseAdapters(Adapter *adapters, unsigned long long *cache, int count, int root, int start) {
    unsigned long long result = 0;

    if (root == adapters[count - 1].joltage) {
        return 1;
    }
    
    for (int i = start; i < start + 3 && i < count; i++) {
        if ((adapters[i].joltage - root) <= 3) {
#ifdef CACHING_ON
            if (cache[adapters[i].joltage] != 0) {
                result += cache[adapters[i].joltage];
            } else {
                result += traverseAdapters(adapters, cache, count, adapters[i].joltage, i + 1);
            }
#else
            result += traverseAdapters(adapters, cache, count, adapters[i].joltage, i + 1);
#endif
        } else {
            break;
        }
    }

    if (cache[root] == 0) {
        cache[root] = result;
    }

    return result;
}

int main (int argc, char* argv[]) {
    FILE *inputFile = fopen("task10input.txt", "r");
    char *buffer = NULL;
    size_t n;
    Adapter adapters[99];
    int count = 0;

    while (getline(&buffer, &n, inputFile) != -1) {
        buffer[strcspn(buffer, "\n")] = 0;
        adapters[count].joltage = atoi(buffer);
        adapters[count].connected = 0;
        adapters[count].cache = 0;
        count += 1;
    }

    fclose(inputFile);

    count += 1;
    adapters[count - 1].joltage = 0;
    adapters[count - 1].connected = 0;

    for (int i = 0; i < count; i++) {
        if (adapters[i].joltage > adapters[count - 1].joltage) {
            adapters[count - 1].joltage = adapters[i].joltage;
        }
    }

    adapters[count - 1].joltage += 3;

    int stats[3] = {0, 0, 0};

    int currentJoltage = 0;
    bool allConnected = 0;
    int nextJoltage = 0;

    while (!allConnected && nextJoltage >= 0) {
        nextJoltage = -1;

        for (int i = 1; i <= 3; i++) {
            nextJoltage = findAdapter(adapters, count, currentJoltage + i);

            if (nextJoltage > -1) {
                stats[i - 1] += 1;
                break;
            }
        }

        if (nextJoltage > -1) {
            currentJoltage = adapters[nextJoltage].joltage;

            if (nextJoltage == count - 1) {
                allConnected = 1;
            }
        }
    }

    int mult = stats[0] * stats[2];
    printf("lvl 1 = %d ; lvl 2 = %d ; lvl 3 = %d ; mult = %d\n", stats[0], stats[1], stats[2], mult);

    qsort(adapters, count, sizeof(Adapter), compareAdapters);
    unsigned long long cache[200] = {0};

    unsigned long long traversion = traverseAdapters(adapters, cache, count, 0, 0);
    printf("traversion = %llu\n", traversion);

    return 0;
}