#include "../Core/raycast.h"

void getNextNumber(FILE *fh, float *number)
{
    char c = fgetc(fh);
    int negation = 0;

    while(!isdigit(c))
    {
        if(c == '-') negation = 1;  
        c = fgetc(fh);
    }

    fseek(fh, -1, SEEK_CUR);
    fscanf(fh, "%f", number);
    if(negation) *number *= -1;
}

int checkEndLine(FILE *fh)
{
    char c = fgetc(fh);

    while(c == ' ' && !feof(fh)) c = fgetc(fh);

    if(feof(fh) || c == '\n') return 1;

    fseek(fh, -1, SEEK_CUR);
    return 0;
}

// Returns a tuple, with index 0 being the number of objects and index 1 being the number of lights:
void readObjects(FILE *fh, struct Object *objects, struct Object *lights, int *tuple)
{
    char c[100] = "";
    fscanf(fh, "%s", c);
    int objectIndex = 1, lightIndex = 0, camerasFound = 0;

    while(!feof(fh))
    {
        if(!strcmp("camera,", c))
        {
            objects[0].klass = CAMERA;
            objects[0].x = 0;
            objects[0].y = 0;
            objects[0].z = 0;
            readCameraFields(fh, &objects[0]);
            camerasFound++;
        }
        else if(!strcmp("light,", c))
        {
            lights[lightIndex].klass = LIGHT;
            readLightFields(fh, &lights[lightIndex]);
            lightIndex++;
        }
        else if(!strcmp("sphere,", c))
        {
            objects[objectIndex].klass = SPHERE;
            readSphereFields(fh, &objects[objectIndex]);
            objectIndex++;
        }
        else if(!strcmp("plane,", c))
        {
            objects[objectIndex].klass = PLANE;
            objects[objectIndex].s_red = 0;
            objects[objectIndex].s_green = 0;
            objects[objectIndex].s_blue = 0;
            readPlaneFields(fh, &objects[objectIndex]);
            objectIndex++;
        }
        else if(!strcmp("quadric,", c))
        {
            objects[objectIndex].klass = QUADRIC;
            readQuadricFields(fh, &objects[objectIndex]);
            objectIndex++;
        }

        if(!feof(fh)) fscanf(fh, "%s", c);
    }

    if(camerasFound != 1)
    {
        fprintf(stderr, "Either no cameras or multiple cameras were found. Exiting!\n");
        exit(-1);
    }
    tuple[0] = objectIndex;
    tuple[1] = lightIndex;
}

void readCameraFields(FILE *fh, Object *camera)
{
    char c[100] = "";
    int end = 0, heightFound = 0, weightFound = 0;

    while(!end)
    {
        if(!strcmp("width:", c))
        {
            fscanf(fh, "%f", &camera->width);
            weightFound++;
        }
        else if(!strcmp("height:", c))
        {
            fscanf(fh, "%f", &camera->height);
            heightFound++;
        }

        if(checkEndLine(fh))
        {
            end = 1;
        }
        else
        {
            fscanf(fh, "%s", c);
        }
    }

    if(!heightFound || !weightFound)
    {
        fprintf(stderr, "[Camera] Did not find all required properties before new line or end of file.\n");
        exit(-1);
    }
    else if(heightFound > 1 || weightFound > 1)
    {
        fprintf(stderr, "Incorrect file formatting - multiple widths or heights found for camera!\n");
        exit(-1);
    }
}

void readLightFields(FILE *fh, Object *light)
{
    char c[100] = "";
    int end = 0, pFound = 0, cFound = 0, tFound = 0, dirFound = 0, a0Found = 0, a1Found = 0, a2Found = 0;

    while(!end)
    {
        if(!strcmp("color:", c))
        {
            getNextNumber(fh, &light->red);

            getNextNumber(fh, &light->green);

            getNextNumber(fh, &light->blue);

            cFound++;
        }
        else if(!strcmp("position:", c))
        {
            getNextNumber(fh, &light->x);

            getNextNumber(fh, &light->y);

            getNextNumber(fh, &light->z);

            pFound++;
        }
        else if(!strcmp("theta:", c))
        {
            getNextNumber(fh, &light->theta);

            tFound++;
        }
        else if(!strcmp("direction:", c))
        {
            getNextNumber(fh, &light->dir_x);

            getNextNumber(fh, &light->dir_y);

            getNextNumber(fh, &light->dir_z);

            dirFound++;
        }
        else if(!strcmp("radial-a0:", c))
        {
            getNextNumber(fh, &light->a1);

            a0Found++;
        }
        else if(!strcmp("radial-a1:", c))
        {
            getNextNumber(fh, &light->a1);

            a1Found++;
        }
        else if(!strcmp("radial-a2:", c))
        {
            getNextNumber(fh, &light->a2);

            a2Found++;
        }

        if(checkEndLine(fh))
        {
            end = 1;
        }
        else
        {
            fscanf(fh, "%s", c);
        }
    }

    if(!cFound || !pFound || !a0Found || !a1Found || !a2Found)
    {
        fprintf(stderr, "[Light] Did not find all required properties before new line or end of file.\n");
        exit(-1);
    }
    else if(cFound > 1 || pFound > 1 || tFound > 1 || a0Found > 1 || a1Found > 1 || a2Found > 1)
    {
        fprintf(stderr, "Incorrect file formatting - multiple widths or heights found for camera!\n");
        exit(-1);
    }

    if(tFound && light->theta > 0)
    {
        if(!dirFound)
        {
            fprintf(stderr, "Malformed spot light detected!\n");
            exit(-1);
        }
    }
    else light->theta = 0;
}

void readSphereFields(FILE *fh, Object *sphere)
{
    char c[100] = "";
    fscanf(fh, "%s", c);
    int cdFound = 0, csFound = 0, pFound = 0, r_found = 0, reflectivityFound = 0, refractivityFound = 0, iorFound = 0, end = 0;

    while(!end)
    {
        if(!strcmp("diffuse_color:", c))
        {

            getNextNumber(fh, &sphere->d_red);
            getNextNumber(fh, &sphere->d_green);
            getNextNumber(fh, &sphere->d_blue);
            cdFound++;
        }
        else if(!strcmp("specular_color:", c))
        {
            getNextNumber(fh, &sphere->s_red);
            getNextNumber(fh, &sphere->s_green);
            getNextNumber(fh, &sphere->s_blue);
            csFound++;
        }
        else if(!strcmp("position:", c))
        {
            getNextNumber(fh, &sphere->x);
            getNextNumber(fh, &sphere->y);
            getNextNumber(fh, &sphere->z);
            pFound++;
        }
        else if(!strcmp("radius:", c))
        {
            getNextNumber(fh, &sphere->radius);
            r_found++;
        }
        else if(!strcmp("reflectivity:", c))
        {
            getNextNumber(fh, &sphere->reflectivity);
            reflectivityFound++;
        }
        else if(!strcmp("refractivity:", c))
        {
            getNextNumber(fh, &sphere->refractivity);
            refractivityFound++;
        }
        else if(!strcmp("ior:", c))
        {
            getNextNumber(fh, &sphere->ior);
            iorFound++;
        }

        if(checkEndLine(fh))
        {
            end = 1;
        }
        else
        {
            fscanf(fh, "%s", c);
        }
    }

    if(!pFound && !r_found)
    {
        fprintf(stderr, "[Sphere] Did not find all required properties before new line or end of file.\n");
        exit(-1);
    }
    else if(cdFound > 1 || csFound > 1 || pFound > 1 || r_found > 1 || reflectivityFound > 1 || refractivityFound > 1 || iorFound > 1)
    {
        fprintf(stderr, "[Sphere] Incorrect file formatting - duplicate values found!\n");
        exit(-1);
    }

    if(!reflectivityFound)
    {
        sphere->reflectivity = 0;
    }

    if((refractivityFound || iorFound) &&(!refractivityFound || !iorFound))
    {
        fprintf(stderr, "Unpaired refraction and IOR fields.\n");
        exit(-1);
    }

    if(!refractivityFound)
    {
        sphere->refractivity = 0;
    }

    if(!iorFound)
    {
        sphere->ior = 0;
    }

    if(!csFound)
    {
        sphere->s_red = 0;
        sphere->s_green = 0;
        sphere->s_blue = 0;
    }

    if(!cdFound)
    {
        sphere->d_red = 0;
        sphere->d_green = 0;
        sphere->d_blue = 0;
    }

    if(!reflectivityFound) sphere->reflectivity = 0;
}
void readPlaneFields(FILE *fh, Object *plane)
{
    char c[100] = "";
    int cdFound = 0, csFound = 0, pFound = 0, nFound = 0, reflectivityFound = 0, end = 0;

    while(!end)
    {
        if(!strcmp("diffuse_color:", c))
        {
            getNextNumber(fh, &plane->d_red);
            getNextNumber(fh, &plane->d_green);
            getNextNumber(fh, &plane->d_blue);
            cdFound++;
        }
        if(!strcmp("specular_color:", c))
        {
            getNextNumber(fh, &plane->s_red);
            getNextNumber(fh, &plane->s_green);
            getNextNumber(fh, &plane->s_blue);
            csFound++;
        }
        else if(!strcmp("position:", c))
        {
            getNextNumber(fh, &plane->x);
            getNextNumber(fh, &plane->y);
            getNextNumber(fh, &plane->z);
            pFound++;
        }
        else if(!strcmp("normal:", c))
        {
            getNextNumber(fh, &plane->n0);
            getNextNumber(fh, &plane->n1);
            getNextNumber(fh, &plane->n2);
            nFound++;
        }
        else if(!strcmp("reflectivity:", c))
        {
            getNextNumber(fh, &plane->reflectivity);
            reflectivityFound++;
        }

        if(checkEndLine(fh))
        {
            end = 1;
        }
        else
        {
            fscanf(fh, "%s", c);
        }
    }

    if(!pFound && !nFound)
    {
        fprintf(stderr, "[Plane] Did not find all required properties before new line or end of file.\n");
        exit(-1);
    }
    else if(cdFound > 1 || csFound > 1 || pFound > 1 || nFound > 1 || reflectivityFound > 1)
    {
        fprintf(stderr, "Incorrect file formatting - duplicative values found!\n");
        exit(-1);
    }

    if(!reflectivityFound)
    {
        plane->reflectivity = 0;
    }

    if(!csFound)
    {
        plane->s_red = 0;
        plane->s_green = 0;
        plane->s_blue = 0;
    }

    if(!cdFound)
    {
        plane->d_red = 0;
        plane->d_green = 0;
        plane->d_blue = 0;
    }
}

void readQuadricFields(FILE *fh, Object *quadric)
{
    char c[100] = "";
    fscanf(fh, "%s", c);
    int cdFound = 0, csFound = 0, pFound = 0, constantFound = 0, reflectivityFound = 0, end = 0;

    while(!end)
    {
        if(!strcmp("diffuse_color:", c))
        {
            getNextNumber(fh, &quadric->d_red);
            getNextNumber(fh, &quadric->d_green);
            getNextNumber(fh, &quadric->d_blue);
            cdFound++;
        }
        else if(!strcmp("specular_color:", c))
        {
            getNextNumber(fh, &quadric->s_red);
            getNextNumber(fh, &quadric->s_green);
            getNextNumber(fh, &quadric->s_blue);
            csFound++;
        }
        else if(!strcmp("position:", c))
        {
            getNextNumber(fh, &quadric->x);
            getNextNumber(fh, &quadric->y);
            getNextNumber(fh, &quadric->z);
            pFound++;
        }
        else if(!strcmp("constants:", c))
        {
            getNextNumber(fh, &quadric->A);
            getNextNumber(fh, &quadric->B);
            getNextNumber(fh, &quadric->C);
            getNextNumber(fh, &quadric->D);
            getNextNumber(fh, &quadric->E);
            getNextNumber(fh, &quadric->F);
            getNextNumber(fh, &quadric->G);
            getNextNumber(fh, &quadric->H);
            getNextNumber(fh, &quadric->I);
            getNextNumber(fh, &quadric->J);
            constantFound++;
        }
        else if(!strcmp("reflectivity:", c))
        {
            getNextNumber(fh, &quadric->reflectivity);
            reflectivityFound++;
        }

        if(checkEndLine(fh))
        {
            end = 1;
        }
        else
        {
            fscanf(fh, "%s", c);
        }
    }

    if(!pFound && !constantFound)
    {
        fprintf(stderr, "[Quadric] Did not find all required properties before new line or end of file.\n");
        exit(-1);
    }
    else if(cdFound > 1 || csFound > 1 || pFound > 1 || constantFound > 1 || reflectivityFound > 1)
    {
        fprintf(stderr, "Incorrect file formatting - duplicative values found!\n");
        exit(-1);
    }
    if(!reflectivityFound)
    {
        quadric->reflectivity = 0;
    }

    if(!csFound)
    {
        quadric->s_red = 0;
        quadric->s_green = 0;
        quadric->s_blue = 0;
    }

    if(!cdFound)
    {
        quadric->d_red = 0;
        quadric->d_green = 0;
        quadric->d_blue = 0;
    }
}
