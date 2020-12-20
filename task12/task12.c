#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define REALLOCATION_STEP 100

typedef enum {
    NORTH = 0,
    EAST,
    SOUTH,
    WEST,
    LEFT,
    RIGHT,
    FORWARD,
    UNKNOWN
} Direction;

typedef struct {
    Direction direction;
    int units;
} Action;

typedef struct {
    int northsouth;
    int eastwest;
    Direction direction;
    int waypointNS;
    int waypointEW;
} Ship;

void initShip(Ship *newShip) {
    newShip->northsouth = 0;
    newShip->eastwest = 0;
    newShip->waypointEW = 10;
    newShip->waypointNS = 1;
    newShip->direction = EAST;
}

Direction afterTurn(Direction initial, Direction turn, int units) {
    if (turn == FORWARD) {
        return FORWARD;
    }

    Direction map[4] = {NORTH, EAST, SOUTH, WEST};
    int turnCount = units / 90;
    int currentDirection = initial;

    int mult = 1;

    if (turn == LEFT) {
        mult = -1;
    }

    for (int i = 0; i < turnCount; i++) {
        currentDirection += (1 * mult); 

        if (currentDirection < 0) {
            currentDirection = WEST;
        } else if (currentDirection >= 4) {
            currentDirection = NORTH;
        }
    }

    return currentDirection;
}

void turnWaypoint(Ship *ship, Direction turn, int degrees) {
    if (degrees == 180) {
        ship->waypointEW *= -1;
        ship->waypointNS *= -1;
    } else if (degrees == 270) {
        degrees = 90;
        if (turn == LEFT) {
            turn = RIGHT;
        } else {
            turn = LEFT;
        }
    }

    if (degrees == 90) {
        int ewmult = 1;
        int nsmult = 1;

        if (ship->waypointEW != 0) {
            ewmult = ship->waypointEW / abs(ship->waypointEW);
        }

        if (ship->waypointNS != 0) {
            nsmult = ship->waypointNS / abs(ship->waypointNS);
        }

        ship->waypointEW = abs(ship->waypointNS) + abs(ship->waypointEW);
        ship->waypointNS = abs(ship->waypointEW) - abs(ship->waypointNS);
        ship->waypointEW = abs(ship->waypointEW) - abs(ship->waypointNS);

        ship->waypointEW *= ewmult;
        ship->waypointNS *= nsmult;

        if (ship->waypointEW == 0 || ship->waypointNS == 0) {
            ship->waypointEW *= nsmult;
            ship->waypointNS *= ewmult;

            if (turn == RIGHT) {
                ship->waypointNS *= -1;
            } else {
                ship->waypointEW *= -1;
            }
        } else {
            if (turn == RIGHT) {
                if (ship->waypointEW > 0 && ship->waypointNS > 0) {
                    ship->waypointNS *= -1;
                } else if (ship->waypointEW > 0 && ship->waypointNS < 0) {
                    ship->waypointEW *= -1;
                } else if (ship->waypointEW < 0 && ship->waypointNS < 0) {
                    ship->waypointNS *= -1;
                } else if (ship->waypointEW < 0 && ship->waypointNS > 0) {
                    ship->waypointEW *= -1;
                }
            } else {
                if (ship->waypointEW > 0 && ship->waypointNS > 0) {
                    ship->waypointEW *= -1;
                } else if (ship->waypointEW < 0 && ship->waypointNS > 0) {
                    ship->waypointNS *= -1;
                } else if (ship->waypointEW < 0 && ship->waypointNS < 0) {
                    ship->waypointEW *= -1;
                } else if (ship->waypointEW > 0 && ship->waypointNS < 0) {
                    ship->waypointNS *= -1;
                }
            }
        }
    }
}

Direction codeToDirection(char code) {
    static const char *directionCodes = "NESWLRF";
    char *found = strchr(directionCodes, code);

    if (found != NULL) {
        return (Direction) strlen(directionCodes) - strlen(found);
    }

    return UNKNOWN;
}

int main (int argc, char *argv[]) {
    FILE *inputFile = fopen("task12input.txt", "r");
    char *buffer = NULL;
    size_t size = 0;
    int count = 0;
    int allocated = 0;

    Action *actions = malloc(sizeof(Action));

    while (getline(&buffer, &size, inputFile) != -1) {
        count += 1;

        if (allocated < count) {
            actions = realloc(actions, REALLOCATION_STEP * sizeof(Action));
            allocated += REALLOCATION_STEP;
        }

        buffer[strcspn(buffer, "\n")] = 0;

        actions[count - 1].direction = codeToDirection(buffer[0]);
        actions[count - 1].units = atoi(buffer + 1);
    }

    Ship myShip;
    initShip(&myShip);

    for (int i = 0; i < count; i++) {
        if (actions[i].direction <= 3) {
            switch (actions[i].direction) {
                case NORTH :
                    myShip.northsouth += actions[i].units; break;
                case SOUTH : 
                    myShip.northsouth -= actions[i].units; break;
                case EAST :
                    myShip.eastwest += actions[i].units; break;
                case WEST : 
                    myShip.eastwest -= actions[i].units; break;
            }
        } else {
            if (actions[i].direction == FORWARD) {
                switch(myShip.direction) {
                    case NORTH :
                        myShip.northsouth += actions[i].units; break;
                    case SOUTH : 
                        myShip.northsouth -= actions[i].units; break;
                    case EAST :
                        myShip.eastwest += actions[i].units; break;
                    case WEST : 
                        myShip.eastwest -= actions[i].units; break;
                }
            } else {
                myShip.direction = afterTurn(myShip.direction, actions[i].direction, actions[i].units);
            }
        }
    }

    int manDistance = abs(myShip.eastwest) + abs(myShip.northsouth);

    initShip(&myShip);

    for (int i = 0; i < count; i++) {
        if (actions[i].direction <= 3) {
            switch (actions[i].direction) {
                case NORTH :
                    myShip.waypointNS += actions[i].units; break;
                case SOUTH : 
                    myShip.waypointNS -= actions[i].units; break;
                case EAST :
                    myShip.waypointEW += actions[i].units; break;
                case WEST : 
                    myShip.waypointEW -= actions[i].units; break;
            }
        } else {
            if (actions[i].direction == FORWARD) {
                myShip.northsouth += myShip.waypointNS * actions[i].units;
                myShip.eastwest += myShip.waypointEW * actions[i].units;
            } else {
                turnWaypoint(&myShip, actions[i].direction, actions[i].units);
            }
        }
    }

    manDistance = abs(myShip.eastwest) + abs(myShip.northsouth);
 
    free(actions);
    fclose(inputFile);
    return 0;
}