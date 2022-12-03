#ifndef RAYCAST_H
#define RAYCAST_H

#include <math.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include "../Math/vector3D.h"

typedef enum OBJECT_KLASS
{
    NONE, CAMERA, LIGHT, SPHERE, PLANE, QUADRIC

} OBJECT_KLASS;

typedef enum POSITION_FLAG
{
    INSIDE, OUTSIDE

} POSITION_FLAG;

struct RGBPixel
{
    uint8_t red;
    uint8_t green;
    uint8_t blue;
};

struct HeaderInfo
{
    char ppmVersion;
    int width;
    int height;
    int maxColor;
    int depth;
};

struct Dimensions
{
    float deltaX;
    float deltaY;
    float minX;
    float maxY;
    int imageWidth;
    int imageHeight;
};

typedef struct Object
{
    // Struct # in the union below:
    // 0 = not an object (default)
    // 1 = camera
    // 2 = sphere
    // 3 = plane

    // Common for all values:
    OBJECT_KLASS klass;
    float x, y, z;
    float reflectivity;

    // Colors:
    float red;
    float green;
    float blue;

    // Diffuse Colors:
    float d_red;
    float d_green;
    float d_blue;

    // Specular Colors:
    float s_red;
    float s_green;
    float s_blue;

    union
    {
        // Camera properties:
        struct
        {
            float width;
            float height;
        };
        // Sphere properties:
        struct
        {
            float radius;
            float refractivity;
            float ior;
        };
        // Plane properties:
        struct
        {
            float n0;
            float n1;
            float n2;
        };
        // Quadric function alphabet soup:
        struct
        {
            float A;
            float B;
            float C;
            float D;
            float E;
            float F;
            float G;
            float H;
            float I;
            float J;
        };
        // Light struct:
        struct
        {
            float a0;
            float a1;
            float a2;
            float theta;
            float dir_x;
            float dir_y;
            float dir_z;
        };
    };
} Object;

int checkEndLine(FILE *fh);
void getNextNumber(FILE *fh, float *number);
void readPlaneFields(FILE *fh, Object *plane);
void readLightFields(FILE *fh, Object *light);
void readCameraFields(FILE *fh, Object *camera);
void readSphereFields(FILE *fh, Object *sphere);
void readQuadricFields(FILE *fh, Object *quadric);
int checkForResize(int capacity, int objectCount);
int validateOutputPath(FILE *fh, char *outputFilename);
void resizeArray(struct Object **objects, int newCapacity);
void printObjectProperties(struct Object *objects, int objectCount);
void readObjects(FILE *fh, struct Object *objects, struct Object *lights, int *countTuple);
int writePPM3(struct RGBPixel *rgbArray, struct HeaderInfo *headerData, char *outputFilename);
void computeIntersections(struct RGBPixel *outputImage, struct Object *objects, struct Object *lights, struct Dimensions dimensions, int objectCount, int lightCount);

// Engine functions:
void getPoint(float *object, float *origin, float t, float *newPoint);
float shoot(struct Object currentObject, float *RdNorm, struct Object origin);
void illuminate(float *point, struct Object *lights, int lightCount, int currentObject, struct Object *objects, int objectCount, struct RGBPixel *outputImage, int outIndex, float *RdNorm, int recursionCount, POSITION_FLAG position);

#endif