#include "../Core/raycast.h"

void printObjectProperties(struct Object *objects, int objectCount)
{
    for(int i = 0; i < objectCount; i++)
    {
        struct Object *object = &objects[i];
        if(object->klass == CAMERA)
        {
            printf("camera, position: [%f, %f, %f], width: %f, height: %f\n", object->x, object->y, object->z, object->width, object->height);
        }
        else if(object->klass == LIGHT)
        {
            printf("light, position: [%f, %f, %f], color: [%f, %f, %f], a0: %f, a1: %f, a2: %f\n", object->x, object->y, object->z, object->red, object->green, object->blue, object->a0, object->a1, object->a2);

            if(object->theta > 0)
            {
                printf("direction: [%f, %f, %f], theta: %f\n", object->dir_x, object->dir_y, object->dir_z, object->theta);
            }
        }
        else if(object->klass == SPHERE)
        {
            printf("sphere, position: [%f, %f, %f], color_s: [%f, %f, %f], color_d: [%f, %f, %f], radius: %f, reflectivity: %f, refractivity: %f, ior: %f\n", object->x, object->y, object->z, object->s_red, object->s_green, object->s_blue, object->d_red, object->d_green, object->d_blue, object->radius, object->reflectivity, object->refractivity, object->ior);
        }
        else if(object->klass == PLANE)
        {
            printf("plane, position: [%f, %f, %f], color_s: [%f, %f, %f], color_d: [%f, %f, %f], normal: [%f, %f, %f], reflectivity: %f\n", object->x, object->y, object->z, object->s_red, object->s_green, object->s_blue, object->d_red, object->d_green, object->d_blue, object->n0, object->n1, object->n2, object->reflectivity);
        }
        else if(object->klass == QUADRIC)
        {
            printf("quadrics, position: [%f, %f, %f], color_s: [%f, %f, %f], color_d: [%f, %f, %f], constant: [%f, %f, %f, %f, %f, %f, %f, %f, %f, %f], reflectivity: %f\n", object->x, object->y, object->z, object->s_red, object->s_green, object->s_blue, object->d_red, object->d_green, object->d_blue, object->A, object->B, object->C, object->D, object->E, object->F, object->G, object->H, object->I, object->J, object->reflectivity);
        }
        else
        {
            printf("Invalid object type: %d. Cannot print.\n", object->klass);
        }
    }
}

int checkForResize(int capacity, int objectCount)
{
    return capacity == objectCount;
}

void resizeArray(struct Object **objects, int newCapacity)
{
    printf("Resizing array! Newcapacity: %d\n", newCapacity);
}
