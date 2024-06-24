#include <string.h>
#include <math.h>
#include <stdint.h>

typedef struct
{
    float x;
    float y;
    float z;
} pos_t;

typedef struct
{
    uint8_t *neighbours;
    size_t numberOfNeighbours;
} neighbours_t;

void formationControlLoop(uint8_t droneId);
uint8_t getNormalisedDroneId(uint8_t droneId);
void updatePositions(uint8_t droneId, float x, float y, float z);
void calcVelocity(uint8_t droneId);
