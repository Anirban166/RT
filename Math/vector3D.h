#ifndef VECTOR3D_H
#define VECTOR3D_H

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

float length(float *vector);
float angle(float *vectorOne, float *vectorTwo);
float distance(float *vectorOne, float *vectorTwo);
float angleQuick(float *vectorOne, float *vectorTwo);
float dotProduct(float *vectorOne, float *vectorTwo);

void scale(float *vector, float scalar);
void normalization(float *result, float *vector);
void reflection(float *result, float *vector, float *normal);
void addition(float *result, float *vectorOne, float *vectorTwo);
void subtraction(float *result, float *vectorOne, float *vectorTwo);
void crossProduct(float *result, float *vectorOne, float *vectorTwo);
void matrixMultiplication(float *result, float *matrix, float *vector);

bool equalityCheck(float *vectorOne, float *vectorTwo, float tolerance);
bool equalityCheckVector4D(float *vectorOne, float *vectorTwo, float tolerance);
#endif