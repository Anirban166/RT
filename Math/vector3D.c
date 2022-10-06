#include "vector3D.h"

float length(float *vector)
{
    return (sqrt(pow(vector[0], 2) + pow(vector[1], 2) + pow(vector[2], 2)));
}

void scale(float *vector, float scalar)
{
    for(int i = 0; i < 3; i++)
        vector[i] *= scalar;
}

float angleQuick(float *vectorOne, float *vectorTwo)
{
    return (dotProduct(vectorOne, vectorTwo)) / (length(vectorOne) * length(vectorTwo));
}

float angle(float *vectorOne, float *vectorTwo)
{
    return acos(angleQuick(vectorOne, vectorTwo));
}

void normalization(float *result, float *vector)
{
    for(int i = 0; i < 3; i++)
        result[i] = vector[i] / length(vector);
}

float dotProduct(float *vectorOne, float *vectorTwo)
{
    return vectorOne[0] * vectorTwo[0] + vectorOne[1] * vectorTwo[1] + vectorOne[2] * vectorTwo[2];
}

void addition(float *result, float *vectorOne, float *vectorTwo)
{
    for(int i = 0; i < 3; i++)
        result[i] = vectorOne[i] + vectorTwo[i];
}

void reflection(float *result, float *vector, float *normal)
{
    for(int i = 0; i < 3; i++)
        result[i] = vector[i] - (2 * dotProduct(normal, vector) * normal[i]);
}

void subtraction(float *result, float *vectorOne, float *vectorTwo)
{
    for(int i = 0; i < 3; i++)
        result[i] = vectorOne[i] - vectorTwo[i];
}

void crossProduct(float *result, float *vectorOne, float *vectorTwo)
{
    // Doing this for cases such as when I need to store the result in vectorOne or vectorTwo:
    float tempArray[3];
    tempArray[0] = vectorOne[1] * vectorTwo[2] - vectorOne[2] * vectorTwo[1];
    tempArray[1] = vectorOne[2] * vectorTwo[0] - vectorOne[0] * vectorTwo[2];
    tempArray[2] = vectorOne[0] * vectorTwo[1] - vectorOne[1] * vectorTwo[0];
    for(int i = 0; i < 3; i++)
        result[i] = tempArray[i];
}

bool equalityCheck(float *vectorOne, float *vectorTwo, float tolerance)
{
    for(int i = 0; i < 3; i++)
        if(fabsf(vectorOne[i] - vectorTwo[i]) > tolerance)
            return false;
    return true;
}