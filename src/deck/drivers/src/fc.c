#include "fc.h"
#include "log.h"
#include "param.h"
#include "commander.h"
#include "debug.h"
#include <stdio.h>
#include "cf_math.h"

setpoint_t setpoint;
uint8_t formationControl = 0;

//               5 --- 3 --- 1
//               |  \  | \ /    )
//               |    /|\        > 0
//               |  /  | / \    )
//               6 --- 4 --- 2

// #define numberOfDrones 7
// static uint8_t missing = 255;
// uint8_t droneIds[numberOfDrones] = {0x17, 0x18, 0x19, 0x20, 0x26, 0x30, 0x32};

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

// pos_t p[numberOfDrones] = {
//     {2.0, 0.0, 0.0},
//     {1.0, 1.0, 0.0},
//     {1.0, -1.0, 0.0},
//     {0.0, 1.0, 0.0},
//     {0.0, -1.0, 0.0},
//     {-1.0, 1.0, 0.0},
//     {-1.0, -1.0, 0.0}};

/*
               3 ------- 1
               |  \   /  |  \
               |    X    |   > 0
               |  /   \  |  /
               4 ------- 2
*/

#define numberOfDrones 5
static uint8_t missing = 255;
uint8_t droneIds[numberOfDrones] = {0x17, 0x18, 0x19, 0x20, 0x26};

float weights[numberOfDrones][numberOfDrones] = {
    {0.585805498607917, -0.439352868538937, -0.439355379372939, 0.146450119234978, 0.146452630068980},
    {-0.439352868538937, 0.659856030894306, -0.000826728085900423, -0.440179596624837, 0.220503162355369},
    {-0.439355379372939, -0.000826728085900423, 0.659859797145309, 0.220504417772370, -0.440182107458840},
    {0.146450119234978, -0.440179596624837, 0.220504417772370, 0.366954537007348, -0.293729477389859},
    {0.146452630068980, 0.220503162355369, -0.440182107458840, -0.293729477389859, 0.366955792424349}};

neighbours_t adjacency[numberOfDrones] = {
    {(uint8_t[]){1, 2, 3, 4}, 4},
    {(uint8_t[]){0, 2, 3, 4}, 4},
    {(uint8_t[]){0, 1, 3, 4}, 4},
    {(uint8_t[]){0, 1, 2, 4}, 4},
    {(uint8_t[]){0, 1, 2, 3}, 4}};

pos_t positions[numberOfDrones] = {
    {0.0, 0.0, 0.0},
    {0.0, 0.0, 0.0},
    {0.0, 0.0, 0.0},
    {0.0, 0.0, 0.0},
    {0.0, 0.0, 0.0}};

pos_t p[numberOfDrones] = {
    {2.0, 0.0, 0.0},
    {1.0, 1.0, 0.0},
    {1.0, -1.0, 0.0},
    {-1.0, 1.0, 0.0},
    {-1.0, -1.0, 0.0}};

//               3 ------ 2
//               | \    / |
//               |   /\   |               3 ----- 0 ----- 2 ----- 1
//               | /    \ |
//               0 ------ 1

// #define numberOfDrones 4
// uint8_t missing = 255;
// uint8_t droneIds[numberOfDrones] = {0x18, 0x19, 0x20, 0x26};

// float weights[numberOfDrones][numberOfDrones] = {
//     {1.0, -1.0, 1.0, -1.0},
//     {-1.0, 1.0, -1.0, 1.0},
//     {1.0, -1.0, 1.0, -1.0},
//     {-1.0, 1.0, -1.0, 1.0}};

// neighbours_t adjacency[numberOfDrones] = {
//     {(uint8_t[]){1, 2, 3}, 3},
//     {(uint8_t[]){0, 2, 3}, 3},
//     {(uint8_t[]){0, 1, 3}, 3},
//     {(uint8_t[]){0, 1, 2}, 3}};

// pos_t positions[numberOfDrones] = {
//     {-1.0, 1.0, 0.0},
//     {-1.0, -1.0, 0.0},
//     {1.0, -1.0, 0.0},
//     {1.0, 1.0, 0.0}};

// pos_t p[numberOfDrones] = {
//     {-1.0, 1.0, 0.0},
//     {-1.0, -1.0, 0.0},
//     {1.0, -1.0, 0.0},
//     {1.0, 1.0, 0.0}};

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
    if (id != 255 && id != missing)
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

    setpoint->velocity_body = false;

    commanderSetSetpoint(setpoint, 2);
}

void RAL(uint8_t i)
{
    // arm_status result;
    uint8_t numberOfNeighbours = adjacency[i].numberOfNeighbours;
    uint8_t M = numberOfNeighbours - 1; // Number of known neighbours
    uint8_t N = 2;                      // Dimensions

    float32_t vp_ij[N][1];
    float32_t vH_i[M][N];
    float32_t vX_i[M][N];
    float32_t vH_i_T[N * M];
    float32_t vH_i_T_H_i[N * N];
    float32_t vH_i_T_X_i[N * N];
    float32_t vH_i_T_H_i_inv[N * N];
    float32_t vtheta_i[N * N];
    float32_t vtheta_i_T[N * N];
    float32_t vtheta_i_T_p_ij[N * 1];

    // Populate p_ij = p[i] - p[missing]
    pos_t p_i = p[i];
    pos_t p_missing = p[missing];
    vp_ij[0][0] = p_i.x - p_missing.x;
    vp_ij[1][0] = p_i.y - p_missing.y;
    // vp_ij[2][0] = tempz;

    pos_t curPos;
    curPos.x = logGetFloat(logGetVarId("stateEstimate", "x"));
    curPos.y = logGetFloat(logGetVarId("stateEstimate", "y"));

    // Populate H_i and X_i
    uint8_t j = 0;
    uint8_t write = 0;
    for (j = 0; j < numberOfNeighbours; j++)
    {
        uint8_t curNeighbour = adjacency[i].neighbours[j];
        if (curNeighbour != missing)
        {
            // Populate H_i = formation displacement of known neighbours (p_ij)
            pos_t p_neighbour = p[curNeighbour];
            vH_i[write][0] = p_i.x - p_neighbour.x;
            vH_i[write][1] = p_i.y - p_neighbour.y;
            // vH_i[j][2] = p_i.y - p_neighbour.y;

            // Populate X_i = actual positions of known neighbours (z_ij)
            pos_t neighbourPosition = positions[curNeighbour];
            vX_i[write][0] = curPos.x - neighbourPosition.x;
            vX_i[write][1] = curPos.y - neighbourPosition.y;
            // vX_i[j][2] = neighbourPosition.z;

            write++;
        }
    }

    // Initialize the ARM matrix structures
    arm_matrix_instance_f32 p_ij = {N, 1, (float32_t *)vp_ij};
    arm_matrix_instance_f32 H_i = {M, N, (float32_t *)vH_i};
    arm_matrix_instance_f32 X_i = {M, N, (float32_t *)vX_i};
    arm_matrix_instance_f32 H_i_T = {N, M, vH_i_T};
    arm_matrix_instance_f32 H_i_T_H_i = {N, N, vH_i_T_H_i};
    arm_matrix_instance_f32 H_i_T_X_i = {N, N, vH_i_T_X_i};
    arm_matrix_instance_f32 H_i_T_H_i_inv = {N, N, vH_i_T_H_i_inv};
    arm_matrix_instance_f32 theta_i = {N, N, vtheta_i};
    arm_matrix_instance_f32 theta_i_T = {N, N, vtheta_i_T};
    arm_matrix_instance_f32 theta_i_T_p_ij = {N, 1, vtheta_i_T_p_ij};

    // Transpose H_i
    mat_trans(&H_i, &H_i_T);

    // Calculate H_i.T @ H_i
    mat_mult(&H_i_T, &H_i, &H_i_T_H_i);

    // Calculate H_i.T @ X_i
    mat_mult(&H_i_T, &X_i, &H_i_T_X_i);

    // Calculate inverse of H_i.T @ H_i
    mat_inv(&H_i_T_H_i, &H_i_T_H_i_inv);

    // Calculate theta_i
    mat_mult(&H_i_T_H_i_inv, &H_i_T_X_i, &theta_i);

    // Transpose theta_i
    mat_trans(&theta_i, &theta_i_T);

    // Calcutate z_ij
    mat_mult(&theta_i_T, &p_ij, &theta_i_T_p_ij);

    // Update position of missing drone
    positions[missing].x = curPos.x - theta_i_T_p_ij.pData[0];
    positions[missing].y = curPos.y - theta_i_T_p_ij.pData[1];
    // positions[missing].z = p[i].z - theta_i_T_p_ij.pData[2];
}

void calcVelocity(uint8_t droneId)
{
    // Get most recent known position of self
    pos_t curPos;
    curPos.x = logGetFloat(logGetVarId("stateEstimate", "x"));
    curPos.y = logGetFloat(logGetVarId("stateEstimate", "y"));
    // curPos.z = logGetFloat(logGetVarId("stateEstimate", "z"));

    uint8_t id = getNormalisedDroneId(droneId);
    if (id != 255)
    {
        uint8_t numberOfNeighbours = adjacency[id].numberOfNeighbours;
        pos_t sum = {0.0, 0.0, 0.0};
        uint8_t i;

        for (i = 0; i < numberOfNeighbours; i++)
        {
            uint8_t curNeighbour = adjacency[id].neighbours[i];
            if (curNeighbour == missing)
            {
                RAL(id);
            }

            pos_t neighbourPosition = positions[curNeighbour];
            sum.x = sum.x + (weights[id][curNeighbour] * (curPos.x - neighbourPosition.x));
            sum.y = sum.y + (weights[id][curNeighbour] * (curPos.y - neighbourPosition.y));
            // sum.z = sum.z + (weights[id][curNeighbour] * (curPos.z - neighbourPosition.z));
        }

        setVelocitySetpoint(&setpoint, sum.x, sum.y, 1.5, 0);
    }
}

void formationControlLoop(uint8_t droneId)
{
    switch (droneId)
    {
    case 0x17:
        setPositionSetpoint(&setpoint, 1.0, 0.0, 1.5, 0);
        break;
    case 0x18:
        setPositionSetpoint(&setpoint, 0.33, 1, 1.5, 0);
        break;
    case 0x19:
        setPositionSetpoint(&setpoint, 0.33, -1, 1.5, 0);
        break;
    default:
        calcVelocity(droneId);
        break;
    }

    return;
}

PARAM_GROUP_START(fc)
/**
 * @brief Id of missing drone
 */
PARAM_ADD(PARAM_UINT8, missingDrone, &missing)

PARAM_GROUP_STOP(fc)