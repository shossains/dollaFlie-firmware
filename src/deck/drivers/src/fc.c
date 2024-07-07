#include "fc.h"
#include "log.h"
#include "param.h"
#include "commander.h"
#include "debug.h"
#include <stdio.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_linalg.h>

setpoint_t setpoint;
uint8_t formationControl = 0;

//               5 --- 3 --- 1
//               |  \  | \ /    )
//               |    /|\        > 0
//               |  /  | / \    )
//               6 --- 4 --- 2

#define numberOfDrones 7
uint8_t missing = 255;
uint8_t droneIds[numberOfDrones] = {0x16, 0x26, 0x27, 0x28, 0x30, 0x33, 0x34};

float weights[numberOfDrones][numberOfDrones] = {
    {0.2741, -0.2741, -0.2741, 0.1370, 0.1370, 0.0000, 0.0000},
    {-0.2741, 0.6852, 0.0000, -0.5482, 0.0000, 0.0000, 0.1370},
    {-0.2741, 0.0000, 0.6852, 0.0000, -0.5482, 0.1370, 0.0000},
    {0.1370, -0.5482, 0.0000, 0.7537, -0.0685, -0.2741, 0.0000},
    {0.1370, 0.0000, -0.5482, -0.0685, 0.7537, 0.0000, -0.2741},
    {0.0000, 0.0000, 0.1370, -0.2741, 0.0000, 0.2741, -0.1370},
    {0.0000, 0.1370, 0.0000, 0.0000, -0.2741, -0.1370, 0.2741}};

neighbours_t adjacency[numberOfDrones] = {
    {(uint8_t[]){1, 2, 3, 4}, 4},
    {(uint8_t[]){0, 3, 6}, 3},
    {(uint8_t[]){0, 4, 5}, 3},
    {(uint8_t[]){0, 1, 4, 5}, 4},
    {(uint8_t[]){0, 2, 3, 6}, 4},
    {(uint8_t[]){2, 3, 6}, 3},
    {(uint8_t[]){1, 4, 5}, 3}};

pos_t positions[numberOfDrones] = {
    {0.0, 0.0, 0.0},
    {0.0, 0.0, 0.0},
    {0.0, 0.0, 0.0},
    {0.0, 0.0, 0.0},
    {0.0, 0.0, 0.0},
    {0.0, 0.0, 0.0},
    {0.0, 0.0, 0.0}};

pos_t p[numberOfDrones] = {
    {2.0, 0.0, 0.0},
    {1.0, 1.0, 0.0},
    {1.0, -1.0, 0.0},
    {0.0, 1.0, 0.0},
    {0.0, -1.0, 0.0},
    {-1.0, 1.0, 0.0},
    {-1.0, -1.0, 0.0}};

//               3 ------ 2
//               | \    / |
//               |   /\   |               3 ----- 0 ----- 2 ----- 1
//               | /    \ |
//               0 ------ 1

// #define numberOfDrones 4
// uint8_t droneIds[numberOfDrones] = {0x26, 0x27, 0x28, 0x33};

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
//     {0.0, 0.0, 0.0},
//     {0.0, 0.0, 0.0},
//     {0.0, 0.0, 0.0},
//     {0.0, 0.0, 0.0}};

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

void RAL(uint8_t i)
{
    uint8_t numberOfNeighbours = adjacency[i].numberOfNeighbours;

    gsl_matrix *X_i = gsl_matrix_alloc(numberOfNeighbours - 1, 2);
    gsl_matrix *H_i = gsl_matrix_alloc(numberOfNeighbours - 1, 2);

    uint8_t j;
    for (j = 0; j < numberOfNeighbours; j++)
    {
        uint8_t curNeighbour = adjacency[i].neighbours[j];
        if (curNeighbour != missing)
        {
            // X_i = actual positions of known neighbours (z_ij)
            pos_t neighbourPosition = positions[curNeighbour];
            gsl_matrix_set(X_i, j, 0, neighbourPosition.x);
            gsl_matrix_set(X_i, j, 1, neighbourPosition.y);
            // gsl_matrix_set(X_i, j, 2, neighbourPosition.z);

            // H_i = formation displacement of known neighbours (p_ij)
            pos_t p_i = p[i];
            pos_t p_neighbour = p[curNeighbour];
            double tempx = p_i.x - p_neighbour.x;
            double tempy = p_i.y - p_neighbour.y;
            // double tempz = p_i.z - p_neighbour.z;

            gsl_matrix_set(H_i, j, 0, tempx);
            gsl_matrix_set(H_i, j, 1, tempy);
            // gsl_matrix_set(H_i, j, 2, tempz);
        }
    }

    // Calculate H_i_transpose
    gsl_matrix *H_i_transpose = gsl_matrix_alloc(H_i->size2, H_i->size1);
    gsl_matrix_transpose_memcpy(H_i_transpose, H_i);

    // Calculate H_i.T @ H_i
    gsl_matrix *H_i_transpose_H_i = gsl_matrix_alloc(H_i->size2, H_i->size2);
    gsl_blas_dgemm(CblasNoTrans, CblasNoTrans, 1.0, H_i_transpose, H_i, 0.0, H_i_transpose_H_i);

    // Calculate the inverse of H_i.T @ H_i
    gsl_matrix *inv_H_i_transpose_H_i = gsl_matrix_alloc(H_i->size2, H_i->size2);
    gsl_matrix_memcpy(inv_H_i_transpose_H_i, H_i_transpose_H_i); // Copy H_i_transpose_H_i to inv_H_i_transpose_H_i
    gsl_linalg_cholesky_decomp(inv_H_i_transpose_H_i);           // Cholesky decomposition
    gsl_linalg_cholesky_invert(inv_H_i_transpose_H_i);           // Invert using Cholesky decomposition

    // Calculate H_i.T @ X_i
    gsl_matrix *H_i_transpose_X_i = gsl_matrix_alloc(H_i->size2, X_i->size2);
    gsl_blas_dgemm(CblasNoTrans, CblasNoTrans, 1.0, H_i_transpose, X_i, 0.0, H_i_transpose_X_i);

    // Calculate theta_i = inv(H_i.T @ H_i) @ H_i.T @ X_i
    gsl_matrix *theta_i = gsl_matrix_alloc(H_i->size2, X_i->size2);
    gsl_blas_dgemm(CblasNoTrans, CblasNoTrans, 1.0, inv_H_i_transpose_H_i, H_i_transpose_X_i, 0.0, theta_i);

    // Calculate theta_i_transpose
    gsl_matrix *theta_i_transpose = gsl_matrix_alloc(theta_i->size2, theta_i->size1);
    gsl_matrix_transpose_memcpy(theta_i_transpose, theta_i);

    // Calculate p_ij = p[i] - p[missing]
    pos_t p_i = p[i];
    pos_t p_neighbour = p[missing];
    double tempx = p_i.x - p_neighbour.x;
    double tempy = p_i.y - p_neighbour.y;
    // double tempz = p_i.z - p_neighbour.z;

    gsl_matrix *p_ij = gsl_matrix_alloc(1, 3);
    gsl_matrix_set(p_ij, 0, 0, tempx);
    gsl_matrix_set(p_ij, 0, 1, tempy);
    // gsl_matrix_set(p_ij, 0, 2, tempz);

    // Calculate theta_i_transpose_pij = @ p[i] - p[missing]
    gsl_matrix *theta_i_transpose_p_ij = gsl_matrix_alloc(theta_i_transpose->size2, p_ij->size2);
    gsl_blas_dgemm(CblasNoTrans, CblasNoTrans, 1.0, theta_i_transpose, p_ij, 0.0, theta_i_transpose_p_ij);

    // Set new position
    positions[missing].x = p[i].x - (float)gsl_matrix_get(theta_i_transpose_p_ij, 0, 0);
    positions[missing].y = p[i].y - (float)gsl_matrix_get(theta_i_transpose_p_ij, 0, 1);
    // positions[missing].z = p[i].z - (float)gsl_matrix_get(theta_i_transpose_p_ij, 0, 2);

    DEBUG_PRINT("( %.2f, %.2f )", (double)positions[missing].x, (double)positions[missing].y);

    // Free allocated memory
    gsl_matrix_free(H_i_transpose);
    gsl_matrix_free(H_i_transpose_H_i);
    gsl_matrix_free(inv_H_i_transpose_H_i);
    gsl_matrix_free(H_i_transpose_X_i);
    gsl_matrix_free(theta_i);
    gsl_matrix_free(theta_i_transpose);
    gsl_matrix_free(p_ij);
    gsl_matrix_free(theta_i_transpose_p_ij);
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

            if (curNeighbour == missing)
            {
                RAL(droneId);
            }

            pos_t neighbourPosition = positions[curNeighbour];
            sum.x = sum.x + (weights[id][curNeighbour] * (curPos.x - neighbourPosition.x));
            sum.y = sum.y + (weights[id][curNeighbour] * (curPos.y - neighbourPosition.y));
        }

        // setVelocitySetpoint(&setpoint, sum.x, sum.y, 1.5, 0);
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

PARAM_GROUP_START(fc)
/**
 * @brief Id of missing drone
 */
PARAM_ADD(PARAM_UINT8, missingDrone, &missing)

PARAM_GROUP_STOP(fc)