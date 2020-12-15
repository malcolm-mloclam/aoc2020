#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

int main (int argc, char **argv) {
    long int nums[1000] = {0};
    char *buffer = NULL;
    size_t n = 0;
    FILE *inputFile = fopen("task9input.txt", "r");
    int counter = 0;

    while (getline(&buffer, &n, inputFile) != -1) {
        buffer[strcspn(buffer, "\n")] = 0;
        nums[counter] = atoi(buffer);
        counter += 1;
    }

    counter += 1;

    bool rightNumber = 1;
    long int wrongNumber = 0;

    for (int i = 25; i < counter && rightNumber; i++) {
        rightNumber = 0;

        for (int j = i - 25; j < i - 1; j++) {
            for (int k = j + 1; k < i; k++) {
                if (nums[i] == nums[j] + nums[k]) {
                    rightNumber = 1;
                }
            }
        }

        if (!rightNumber) {
            wrongNumber = nums[i];
            printf("%ld\n", nums[i]);
        }
    }

    bool seqWrong = 0;
    bool seqFound = 0;
    long int seq[1000];
    int seqLength = 1;
    long int currentSum = 0;

    for (int i = 0; i < counter - 1 && !seqFound; i++) {
        seqLength = 1;
        seq[seqLength - 1] = nums[i];
        currentSum = nums[i];
        seqWrong = 0;
        
        for (int j = i + 1; j < counter && !seqWrong && !seqFound; j++) {
            currentSum += nums[j];

            if (currentSum > wrongNumber) {
                seqWrong = 1;
            } else if (currentSum == wrongNumber) {
                seqFound = 1;
            }

            seqLength += 1;
            seq[seqLength - 1] = nums[j];
        }
    }

    long int min = seq[0];
    long int max = 0;

    for (int i = 0; i < seqLength - 1; i++) { 
        for (int j = i + 1; j < seqLength; j++) {
            if (seq[j] < min) {
                min = seq[j];
            }

            if (seq[j] > max) {
                max = seq[j];
            }
        }
    }

    printf("%ld\n", min + max);

    fclose(inputFile);
    return 0;
}