#include "fc.h"
#include "log.h"
#include "commander.h"
#include "debug.h"
#include <stdio.h>

setpoint_t setpoint;
uint8_t formationControl = 0;

// //              33 --- 28 --- 26
// //               |  \  | \ /    )
// //               |    /|\     >  16
// //               |  /  | / \    )
// //              34 --- 30 --- 27

// #define numberOfDrones 7
// uint8_t droneIds[numberOfDrones] = {0x16, 0x26, 0x27, 0x28, 0x30, 0x33, 0x34};

// float weights[numberOfDrones][numberOfDrones] = {
//     {0.2741, -0.2741, -0.2741, 0.1370, 0.1370, 0.0000, 0.0000},
//     {-0.2741, 0.6852, 0.0000, -0.5482, 0.0000, 0.0000, 0.1370},
//     {-0.2741, 0.0000, 0.6852, 0.0000, -0.5482, 0.1370, 0.0000},
//     {0.1370, -0.5482, 0.0000, 0.7537, -0.0685, -0.2741, 0.0000},
//     {0.1370, 0.0000, -0.5482, -0.0685, 0.7537, 0.0000, -0.2741},
//     {0.0000, 0.0000, 0.1370, -0.2741, 0.0000, 0.2741, -0.1370},
//     {0.0000, 0.1370, 0.0000, 0.0000, -0.2741, -0.1370, 0.2741}};

// neighbours_t adjacency[numberOfDrones] = {
//     {(uint8_t[]){1, 2, 3, 4}, 4},
//     {(uint8_t[]){0, 3, 6}, 3},
//     {(uint8_t[]){0, 4, 5}, 3},
//     {(uint8_t[]){0, 1, 4, 5}, 4},
//     {(uint8_t[]){0, 2, 3, 6}, 4},
//     {(uint8_t[]){2, 3, 6}, 3},
//     {(uint8_t[]){1, 4, 5}, 3}};

// pos_t positions[numberOfDrones] = {
//     {0.0, 0.0, 0.0},
//     {0.0, 0.0, 0.0},
//     {0.0, 0.0, 0.0},
//     {0.0, 0.0, 0.0},
//     {0.0, 0.0, 0.0},
//     {0.0, 0.0, 0.0},
//     {0.0, 0.0, 0.0}};

//               3 ------ 2
//               | \    / |
//               |   /\   |                  3 ----- 0 ----- 2 ----- 1
//               | /    \ |
//               0 ------ 1

#define numberOfDrones 4
uint8_t droneIds[numberOfDrones] = {0x26, 0x27, 0x28, 0x33};

float weights[numberOfDrones][numberOfDrones] = {
    {1.0, -1.0, 1.0, -1.0},
    {-1.0, 1.0, -1.0, 1.0},
    {1.0, -1.0, 1.0, -1.0},
    {-1.0, 1.0, -1.0, 1.0}};

neighbours_t adjacency[numberOfDrones] = {
    {(uint8_t[]){1, 2, 3}, 3},
    {(uint8_t[]){0, 2, 3}, 3},
    {(uint8_t[]){0, 1, 3}, 3},
    {(uint8_t[]){0, 1, 2}, 3}};

pos_t positions[numberOfDrones] = {
    {0.0, 0.0, 0.0},
    {0.0, 0.0, 0.0},
    {0.0, 0.0, 0.0},
    {0.0, 0.0, 0.0}};

uint8_t getNormalisedDroneId(uint8_t droneId)
{
    for (int i = 0; i < numberOfDrones; i++)
    {
        if (droneIds[i] == droneId)
        {
            return i;
        }
    }

    return 255;
}

void updatePositions(uint8_t droneId, float x, float y, float z)
{
    // DEBUG_PRINT("FC = 0x%x: (%.2f, %.2f, %.2f)\n", droneId, (double)x, (double)y, (double)z);
    uint8_t id = getNormalisedDroneId(droneId);

    // Update position of given drone
    if (id != 255)
    {
        positions[id].x = x;
        positions[id].y = y;
        positions[id].z = z;
    }
}

void setVelocitySetpoint(setpoint_t *setpoint, float vx, float vy, float z, float yawrate)
{
    setpoint->mode.x = modeVelocity;
    setpoint->mode.y = modeVelocity;
    setpoint->mode.z = modeAbs;
    setpoint->mode.yaw = modeVelocity;
    setpoint->velocity.x = vx;
    setpoint->velocity.y = vy;
    setpoint->position.z = z;
    setpoint->attitudeRate.yaw = yawrate;

    setpoint->velocity_body = true;

    commanderSetSetpoint(setpoint, 2);
}

void setPositionSetpoint(setpoint_t *setpoint, float x, float y, float z, float yawrate)
{
    setpoint->mode.x = modeAbs;
    setpoint->mode.y = modeAbs;
    setpoint->mode.z = modeAbs;
    setpoint->mode.yaw = modeAbs;

    setpoint->position.x = x;
    setpoint->position.y = y;
    setpoint->position.z = z;
    setpoint->attitudeRate.yaw = yawrate;

    setpoint->velocity_body = true;

    commanderSetSetpoint(setpoint, 2);
}

void calcVelocity(uint8_t droneId)
{
    // Get most recent known position of self
    pos_t curPos;
    curPos.x = logGetFloat(logGetVarId("stateEstimate", "x"));
    curPos.y = logGetFloat(logGetVarId("stateEstimate", "y"));
    curPos.z = logGetFloat(logGetVarId("stateEstimate", "z"));

    uint8_t id = getNormalisedDroneId(droneId);
    if (droneId != 255)
    {
        uint8_t numberOfNeighbours = adjacency[id].numberOfNeighbours;
        pos_t sum = {0.0, 0.0, 0.0};
        uint8_t i;

        for (i = 0; i < numberOfNeighbours; i++)
        {
            uint8_t curNeighbour = adjacency[id].neighbours[i];

            pos_t neighbourPosition = positions[curNeighbour];
            sum.x = sum.x + (weights[id][curNeighbour] * (curPos.x - neighbourPosition.x));
            sum.y = sum.y + (weights[id][curNeighbour] * (curPos.y - neighbourPosition.y));
        }

        setVelocitySetpoint(&setpoint, sum.x, sum.y, 1.5, 0);
    }
}

void formationControlLoop(uint8_t droneId)
{
    switch (droneId)
    {
    case 0x26:
        setPositionSetpoint(&setpoint, -0.75, -0.75, 1.5, 0);
        break;
    case 0x27:
        setPositionSetpoint(&setpoint, 0.75, -0.75, 1.5, 0);
        break;
    case 0x28:
        setPositionSetpoint(&setpoint, 0.75, 0.75, 1.5, 0);
        break;
    default:
        calcVelocity(droneId);
        break;
    }
}