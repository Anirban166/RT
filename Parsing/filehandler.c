#include "../Core/raycast.h"

int validateOutputPath(FILE *fh, char *outputFilename)
{
    if(!fh)
    {
        fprintf(stderr, "Invalid output path: %s\n", outputFilename);
        return -1;
    }
    return 0;
}

int writePPM3(struct RGBPixel *rgbArray, struct HeaderInfo *headerData, char *outputFilename)
{
    FILE *fhWrite = fopen(outputFilename, "w");

    if(validateOutputPath(fhWrite, outputFilename) == -1)
    {
        return -1;
    }
    fwrite("P3\n", 1, sizeof(char) * 3, fhWrite);
    fprintf(fhWrite, "%d %d\n", headerData->width, headerData->height);
    fprintf(fhWrite, "%d\n", headerData->maxColor);

    int size = headerData->width * headerData->height;
    for(int i = 0; i < size; i++)
    {
        struct RGBPixel currPixel = rgbArray[i];
        fprintf(fhWrite, "%d %d %d", currPixel.red, currPixel.green, currPixel.blue);

        if((i + 1) % 3 == 0)
        {
            fprintf(fhWrite, "\n");
        }
        else
        {
            fprintf(fhWrite, " ");
        }
    }

    printf("Finished writing P3 file.\n");
    fclose(fhWrite);
    return 0;
}