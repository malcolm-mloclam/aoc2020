/**
 * --- Day 14: Docking Data ---	
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

#define BITMASK_SIZE 36
#define MEM_INIT_SIZE 1024

int main (int argc, char* argv[]) {
    FILE *inputFile = fopen("task14input.txt", "r");
    char *buffer = NULL;
	char *bufCopy = NULL;
    size_t size = 0; 

	uint8_t *memWrites = NULL; 
	uint8_t memWritesCount = 0;
	uint8_t memSize = (uint8_t) MEM_INIT_SIZE;
   	uint64_t *mem = malloc(sizeof(uint8_t) *  memSize);
	uint8_t mask[36] = {0};
	
	char *token = NULL;
	char *tokCopy = NULL;

    while (getline(&buffer, &size, inputFile) != -1) {
		buffer[strcspn(buffer, "\n")] = 0;
		printf("buffer = %s\n", buffer);
		bufCopy = malloc(strlen(buffer) + 1);
      	strcpy(bufCopy, buffer);
		printf("bufCopy = %s\n", bufCopy);
		token = strtok(bufCopy, " = ");
		size_t tokenLength = strlen(token);
		printf("token length 1 = %zu\n", tokenLength);
		tokCopy = malloc(tokenLength + 1); 
		printf("token before = %s\n", token);
		//strncpy(tokCopy, token, tokenLength);

		for (int i = 0; i < tokenLength + 1; i++) {
			*(tokCopy + i) = *(token + i);
			printf("0x%p = %c\n", tokCopy + i, (char)*(tokCopy + i));
		}

		token = strtok(NULL, " = ");
		token[strcspn(buffer, "\n")] = 0;
		
		char *memaddrString = NULL;
		printf("tokenLength = %zu\n", tokenLength);
		printf("tokCopy = %s\n", tokCopy);
		printf("token = %s\n", token);
		// The "mask" entry would be exactly 4 symbols long
		// while "mem[<value>]" entries would always be longer 

		if (tokenLength > 4) {	
			// If it is a mem[<value>] entry	
		
			size_t firstBracket = strcspn(tokCopy, "[");
			size_t secondBracket = strcspn(tokCopy, "]");
			size_t memaddrLength = secondBracket - firstBracket;

			printf("memaddrLength = %zu\n", memaddrLength);

			memaddrString = malloc(memaddrLength);
			strncpy(memaddrString, tokCopy + firstBracket + 1, memaddrLength - 1);
			printf("strncpy happened\n");	
			memaddrString[memaddrLength - 1] = '\0';
			
			printf("memaddrString = \"%s\"\n", memaddrString);
			printf("tokCopy = \"%s\"\n", tokCopy);
		} else {					
			// If it is a mask entry 
			
			for (int i = 0; i < BITMASK_SIZE; i++) {
				if (token[i] == 'X') {
					mask[i] = -1;
				} else {
					mask[i] = token[i] == '1' ? 1 : 0;
				}
			}
		}

		free(tokCopy);
		free(memaddrString);
		free(bufCopy);

		token = NULL;
		tokCopy = NULL;
		bufCopy = NULL;
    }

	return 0;
}


































