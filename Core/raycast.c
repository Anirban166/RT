#include "raycast.h"

int main(int argc, char **argv)
{
    int imageWidth = atoi(argv[1]);
    int imageHeight = atoi(argv[2]);
    char *inputScene = argv[3];
    char *outputFile = argv[4];

    // Output image has z = -1.
    struct RGBPixel *outputImage = malloc(sizeof(struct RGBPixel) * imageWidth * imageHeight);
    struct Dimensions dimensions;
    struct HeaderInfo headerInfo;
    dimensions.imageWidth = imageWidth;
    dimensions.imageHeight = imageHeight;

    int objectCount, lightCount;

    // Stores shape objects and cameras:
    struct Object objects[256];
    // Stores the lights in the scene:
    struct Object lights[256];

    FILE *fh = fopen(inputScene, "r");

    if(!fh)
    {
        fprintf(stderr, "Invalid file.\n");
        exit(-1);
    }

    printf("File found: %s\n", inputScene);

    int countTuple[2];
    readObjects(fh, objects, lights, countTuple);
    objectCount = countTuple[0];
    lightCount = countTuple[1];
    printf("Number of objects: %d, Number of lights: %d\n", objectCount, lightCount);

    // objectCount = readSceneHeader(fh, &objects, objectCount);
    struct Object camera = objects[0];

    // Computing deltaX and deltaY from objects[0], i.e. the camera:
    dimensions.deltaX = camera.width / imageWidth;
    dimensions.deltaY = camera.height / imageHeight;
    dimensions.minX = -1 * (camera.width / 2);
    dimensions.maxY = (camera.height / 2);

    // Printing object information:
    printObjectProperties(objects, objectCount);
    printObjectProperties(lights, lightCount);

    computeIntersections(outputImage, objects, lights, dimensions, objectCount, lightCount);

    // Building the header for the output ppm3 file:
    headerInfo.ppmVersion = '3';
    headerInfo.width = imageWidth;
    headerInfo.height = imageHeight;
    headerInfo.maxColor = 255;
    headerInfo.depth = 3;
    writePPM3(outputImage, &headerInfo, outputFile);

    return 0;
}

void computeIntersections(struct RGBPixel *outputImage, struct Object *objects, struct Object *lights, struct Dimensions dimensions, int objectCount, int lightCount)
{
    int x, y, z = -1;
    float RdNorm[3], Rd[3], point[3];
    float t, nearestT;
    int nearestObj;

    Object camera = objects[0];

    // Z for the view plane is fixed to -1. Only x and y vary.
    Rd[2] = z;

    for (x = 0; x < dimensions.imageWidth; x++)
    {
        Rd[0] = ((dimensions.minX + (x * dimensions.deltaX)) + (dimensions.deltaX / 2)) - camera.x;

        for (y = 0; y < dimensions.imageHeight; y++)
        {
            struct Object currObject;
            nearestT = -1;
            nearestObj = -1;
            t = -1;
            Rd[1] = ((dimensions.maxY - (y * dimensions.deltaY)) - (dimensions.deltaY / 2)) - camera.y;
            int index = x + (y * dimensions.imageWidth);

            normalization(RdNorm, Rd);

            for (int i = 1; i < objectCount; i++)
            {
                currObject = objects[i];
                t = shoot(currObject, RdNorm, camera);
                // printf("shoot t: %f\n", t);

                if ((t > 0) && (t < nearestT || nearestObj == -1))
                {
                    // printf("Found intersection t = %f, x = %d, y = %d\n", t, x, y);
                    nearestObj = i;
                    nearestT = t;
                }
            }

            if (nearestObj == -1)
            {
                outputImage[index].red = 0;
                outputImage[index].green = 0;
                outputImage[index].blue = 0;
            }
            // Found an intersection!
            else
            {
                // printf("Found object intersection with t = %f, x = %d, y = %d, klass = %d\n", nearestT, x, y, objects[nearestObj].klass);
                float cameraPosition[3] = {camera.x, camera.y, camera.z};
                getPoint(Rd, cameraPosition, nearestT, point);
                illuminate(point, lights, lightCount, nearestObj, objects, objectCount, outputImage, index, RdNorm, 7, OUTSIDE);
            }
        }
    }
}
