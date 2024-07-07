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

uint8_t getNormalisedDroneId(uint8_t droneId);
void updatePositions(uint8_t droneId, float x, float y, float z);
void RAL(uint8_t i);
void calcVelocity(uint8_t droneId);
void formationControlLoop(uint8_t droneId);