#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

int main (int argc, char* argv[]) {
    FILE *inputFile = fopen("test6input.txt", "r");
    char *buffer = NULL;
    //char *answers = NULL;
    size_t n;
    getline(&buffer, &n, inputFile);
    printf("%s\n", buffer);
    return 0;
}