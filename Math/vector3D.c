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

float distance(float *vectorOne, float *vectorTwo)
{
    float distance = 0.0;
    for (int i = 0; i < 3; i++)
        distance += pow((vectorOne[i] - vectorTwo[i]), 2);
    return sqrt(distance);
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

// Going with homogenous coordinates to take into account all possible matrix transformations:
void matrixMultiplication(float *result, float *matrix, float *vector)
{
    for(int i = 0; i < 4; i++)
    {
        result[i] = 0.0;
        for(int j = 0; j < 4; j++)
        {
            if(j != 3)
                result[i] += matrix[4 * j + i] * vector[j];
            else // Required vector's [x y z 1], so I'm making it possible to just pass a 3D vector: (i.e., not relying on the presence of vector[3])
                result[i] += matrix[4 * j + i] * 1;
        }
    }
}

bool equalityCheck(float *vectorOne, float *vectorTwo, float tolerance)
{
    for(int i = 0; i < 3; i++)
        if(fabsf(vectorOne[i] - vectorTwo[i]) > tolerance)
            return false;
    return true;
}

bool equalityCheckVector4D(float *vectorOne, float *vectorTwo, float tolerance)
{
    for(int i = 0; i < 4; i++)
        if(fabsf(vectorOne[i] - vectorTwo[i]) > tolerance)
            return false;
    return true;
}