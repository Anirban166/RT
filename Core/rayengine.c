#include "raycast.h"

// "Aw shoot" - a friend, '22
float shoot(struct Object currentObject, float *RdNorm, struct Object origin)
{
    float t = -1;
    if(currentObject.klass == SPHERE)
    {
        float b, c; // Constant a = 1 for sphere

        // B = 2 * (Xd * (X0 - Xc) + Yd * (Y0 - Yc) + Zd * (Z0 - Zc))
        b = 2 * ((RdNorm[0] * (origin.x - currentObject.x)) + (RdNorm[1] * (origin.y - currentObject.y)) + (RdNorm[2] * (origin.z - currentObject.z)));

        // C = (X0 - Xc) ^ 2 + (Y0 - Yc) ^ 2 + (Z0 - Zc) ^ 2 - Sr ^ 2
        c = pow((origin.x - currentObject.x), 2) + pow((origin.y - currentObject.y), 2) + pow((origin.z - currentObject.z), 2) - pow(currentObject.radius, 2);
        float d = (b * b - 4 * c);
        float t0, t1;
        if(d > 0)
        {
            t0 = (-b - sqrt(d)) / 2;
            t1 = (-b + sqrt(d)) / 2;
        }
        else
        {
            t0 = -1;
            t1 = -1;
        }
        // printf("t0: %f, t1: %f\n", t0, t1);
        t = (t0 < t1 && t0 > 0) ? t0 : t1;
        // printf("Found sphere t = %f, x = %d, y = %d\n", t, x, y);
    }
    else if(currentObject.klass == PLANE)
    {
        float originVec[3] = {origin.x, origin.y, origin.z};
        float planePosition[3] = {currentObject.x, currentObject.y, currentObject.z};
        float A = currentObject.n0, B = currentObject.n1, C = currentObject.n2, D = distance(planePosition, originVec);
        t = -(A * origin.x + B * origin.y + C * origin.z + D) / (A * RdNorm[0] + B * RdNorm[1] + C * RdNorm[2]);
    }
    else if(currentObject.klass == QUADRIC)
    {
        float t0, t1;
        float scaledX = origin.x - currentObject.x;
        float scaledY = origin.y - currentObject.y;
        float scaledZ = origin.z - currentObject.z;
        float Aq = currentObject.A * (pow(RdNorm[0], 2)) + currentObject.B * (pow(RdNorm[1], 2)) + currentObject.C * (pow(RdNorm[2], 2)) + (currentObject.D * RdNorm[0] * RdNorm[1]) + (currentObject.E * RdNorm[0] * RdNorm[2]) + (currentObject.F * RdNorm[1] * RdNorm[2]);
        float Bq = (2 * currentObject.A * scaledX * RdNorm[0]) + (2 * currentObject.B * scaledY * RdNorm[1]) + (2 * currentObject.C * scaledZ * RdNorm[2]) + currentObject.D * (scaledX * RdNorm[1] + scaledY * RdNorm[0]) + currentObject.E * (scaledX * RdNorm[2] + scaledZ + RdNorm[0]) + currentObject.F * (scaledY * RdNorm[2] + RdNorm[1] * scaledZ) + currentObject.G * RdNorm[0] + currentObject.H * RdNorm[1] + currentObject.I * RdNorm[2];
        float Cq = (currentObject.A * pow(scaledX, 2)) + (currentObject.B * pow(scaledY, 2)) + (currentObject.C * pow(scaledZ, 2)) + (currentObject.D * scaledX * scaledY) + (currentObject.E * scaledX * scaledZ) + (currentObject.F * scaledY * scaledZ) + (currentObject.G * scaledX) + (currentObject.H * scaledY) + (currentObject.I * scaledZ) + (currentObject.J);
        // printf("Aq: %f, Bq: %f, Cq: %f\n", Aq, Bq, Cq);
        float d = (Bq * Bq - 4 * Aq * Cq);
        // t0, t1 = (- B +/- (B ^ 2 - 4 * C) ^ 1/2) / 2
        t0 = -1;
        t1 = -1;
        if(d > 0)
        {
            t0 = (-Bq - sqrt(pow(Bq, 2) - 4 * Aq * Cq)) / (2 * Aq);
            t1 = (-Bq + sqrt(pow(Bq, 2) - 4 * Aq * Cq)) / (2 * Aq);
        }
        // printf("Quadric: t0: %f, t1: %f\n", t0, t1);
        t = (t0 < t1 && t0 > 0) ? t0 : t1;
    }

    // printf("Engine T: %f\n", t);
    return t;
}

void getPoint(float *object, float *origin, float t, float *newPoint)
{
    newPoint[0] = origin[0] + object[0] * t;
    newPoint[1] = origin[1] + object[1] * t;
    newPoint[2] = origin[2] + object[2] * t;
}

// "Illumination saved the nation" - a friend, '22
void illuminate(float *point, struct Object *lights, int lightCount, int currentObject, struct Object *objects, int objectCount, struct RGBPixel *outputImage, int outIndex, float *RdNorm, int recursionCount, POSITION_FLAG position)
{
    int lightHit;    
    float distanceValue, t = -1;
    float normal[3], sphereCenter[3], lightPos[3], L[3];
    float V[3] = {RdNorm[0], RdNorm[1], RdNorm[2]}, color[3] = {0, 0, 0};    
    float shine = 20, radialAttenuation = 0, angularAttenuation = 1, totalAttenuation = 0;

    struct Object currentPoint;
    currentPoint.x = point[0];
    currentPoint.y = point[1];
    currentPoint.z = point[2];

    if(objects[currentObject].klass == SPHERE)
    {
        sphereCenter[0] = objects[currentObject].x;
        sphereCenter[1] = objects[currentObject].y;
        sphereCenter[2] = objects[currentObject].z;
        subtraction(normal, point, sphereCenter);

        for(int i = 0; i < 3; i++)
        {
            normal[i] /= objects[currentObject].radius;
        }
        normalization(normal, normal);
    }

    else if(objects[currentObject].klass == PLANE)
    {
        normal[0] = objects[currentObject].n0;
        normal[1] = objects[currentObject].n1;
        normal[2] = objects[currentObject].n2;
        normalization(normal, normal);
    }

    else if(objects[currentObject].klass == QUADRIC)
    {
        normal[0] = (2 * objects[currentObject].A * currentPoint.x + objects[currentObject].D * currentPoint.y + objects[currentObject].E * currentPoint.z + objects[currentObject].G);
        normal[1] = (1 * objects[currentObject].B * currentPoint.y + objects[currentObject].D * currentPoint.x + objects[currentObject].F * currentPoint.z + objects[currentObject].H);
        normal[2] = (2 * objects[currentObject].C * currentPoint.z + objects[currentObject].E * currentPoint.x + objects[currentObject].F * currentPoint.y + objects[currentObject].I);

        float dotProd = dotProduct(RdNorm, normal);
        normalization(normal, normal);

        if(dotProd > 0)
        {
            normal[0] *= -1;
            normal[1] *= -1;
            normal[2] *= -1;
        }
        else
        {
            printf("DotProduct is NOT greater than 0! Value: %f\n", dotProd);
        }
    }

    for(int i = 0; i < lightCount; i++)
    {
        lightHit = 1;
        lightPos[0] = lights[i].x;
        lightPos[1] = lights[i].y;
        lightPos[2] = lights[i].z;

        subtraction(L, lightPos, point);
        normalization(L, L);
        distanceValue = distance(point, lightPos);

        for(int j = 1; j < objectCount; j++)
        {
            if(j != currentObject)
            {
                t = shoot(objects[j], L, currentPoint);
                if(t > 0 && t < distanceValue)
                {
                    lightHit = 0;
                    j = objectCount;
                }
            }
        }
        if(!lightHit)
        {
            continue;
        }

        radialAttenuation = 1 / (lights[i].a0 + lights[i].a1 * distanceValue + lights[i].a2 * pow(distanceValue, 2));
        
        float cosAlpha = 0;
        float vL[3] = {0}, v0[3] = {-1 * L[0], -1 * L[1], -1 * L[2]};

        if(lights[i].theta != 0)
        {
            vL[0] = lights[i].dir_x;
            vL[1] = lights[i].dir_y;
            vL[2] = lights[i].dir_z;
            cosAlpha = dotProduct(v0, vL);
        }

        if(lights[i].theta == 0)
        {
            angularAttenuation = 1;
        }

        else if(cosAlpha < cos(lights[i].theta))
        {
            angularAttenuation = 0;
        }

        else
        {
            angularAttenuation = pow(cosAlpha, lights[i].a1);
        }
        totalAttenuation = radialAttenuation * angularAttenuation;

        float nL = dotProduct(normal, L);
        float R[3] = {0};

        reflection(R, L, normal);
        normalization(R, R);
        float vR = dotProduct(V, R);

        if(nL <= 0)
        {
            nL = 0;
            vR = 0;
        }
        else if(vR < 0)
        {
            vR = 0;
        }

        color[0] += totalAttenuation * ((objects[currentObject].d_red * lights[i].red * nL) + (objects[currentObject].s_red * lights[i].red * pow(vR, shine)));
        color[1] += totalAttenuation * ((objects[currentObject].d_green * lights[i].green * nL) + (objects[currentObject].s_green * lights[i].green * pow(vR, shine)));
        color[2] += totalAttenuation * ((objects[currentObject].d_blue * lights[i].blue * nL) + (objects[currentObject].s_blue * lights[i].blue * pow(vR, shine)));
    }

    color[0] *= 255;
    color[1] *= 255;
    color[2] *= 255;

    for(int i = 0; i < 3; i++)
    {
        if(color[i] > 255)
        {
            color[i] = 255;
        }
    }
    reflection(V, V, normal);

    int nearestObj = -1;
    float nearestT = -1, objectRefraction = 0;
    float reflectedPoint[3], reflection[3], refraction[3];

    for(int i = 1; i < objectCount; i++)
    {
        if(position == OUTSIDE && i != currentObject)
        {
            t = shoot(objects[i], V, currentPoint);
            if((t > 0) && (t < nearestT || nearestObj == -1))
            {
                nearestObj = i;
                nearestT = t;
            }
        }
        else if(position == INSIDE && i == currentObject)
        {
            t = shoot(objects[i], V, currentPoint);
            nearestObj = i;
            nearestT = t;
        }
    }

    // Reflection:
    if(nearestObj == -1 || recursionCount == 0 || objects[currentObject].reflectivity == 0)
    {
        reflection[0] = 0;
        reflection[1] = 0;
        reflection[2] = 0;
    }
    else
    {
        getPoint(V, point, nearestT, reflectedPoint);
        illuminate(reflectedPoint, lights, lightCount, nearestObj, objects, objectCount, outputImage, outIndex, V, recursionCount - 1, position);

        reflection[0] = objects[currentObject].reflectivity * outputImage[outIndex].red;
        reflection[1] = objects[currentObject].reflectivity * outputImage[outIndex].green;
        reflection[2] = objects[currentObject].reflectivity * outputImage[outIndex].blue;
    }

    // Refraction:
    if (objects[currentObject].klass != SPHERE)
    {
        refraction[0] = 0;
        refraction[1] = 0;
        refraction[2] = 0;
    }
    else if (objects[currentObject].refractivity == 0)
    {
        refraction[0] = 0;
        refraction[1] = 0;
        refraction[2] = 0;
    }
    else
    {
        nearestT = -1;
        nearestObj = -1;  
        float iorAir = 1.0003, a[3], b[3], Ut[3], cosPhi, sinPhi, rhoExternal, rhoInternal, refractedPoint[3];              

        if(position == OUTSIDE)
        {
            rhoExternal = iorAir;
            rhoInternal = objects[currentObject].ior;
        }
        else
        {
            rhoExternal = objects[currentObject].ior;
            rhoInternal = iorAir;
        }
        objectRefraction = objects[currentObject].refractivity;

        crossProduct(a, normal, RdNorm);
        crossProduct(b, a, normal);

        sinPhi = (rhoExternal / rhoInternal) * dotProduct(RdNorm, b);
        cosPhi = sqrt(1 - pow(sinPhi, 2));

        for(int i = 0; i < 3; i++)
        {
            Ut[i] = (-normal[i] * cosPhi) + (b[i] * sinPhi);
        }    

        for(int i = 1; i < objectCount; i++)
        {
            if(position == OUTSIDE && i != currentObject)
            {
                t = shoot(objects[i], Ut, currentPoint);
                if((t > 0) && (t < nearestT || nearestObj == -1))
                {
                    nearestObj = i;
                    nearestT = t;
                }
            }
            else if(position == INSIDE && i == currentObject)
            {
                t = shoot(objects[i], Ut, currentPoint);
                nearestObj = i;
                nearestT = t;
            }
        }

        if(nearestObj == -1 || recursionCount == 0)
        {
            refraction[0] = 0;
            refraction[1] = 0;
            refraction[2] = 0;
        }
        else
        {
            if(position == OUTSIDE)
            {
                position = INSIDE;
            }
            else
            {
                position = OUTSIDE;
            }

            getPoint(Ut, point, nearestT, refractedPoint);
            illuminate(refractedPoint, lights, lightCount, nearestObj, objects, objectCount, outputImage, outIndex, Ut, recursionCount - 1, position);

            refraction[0] = objects[currentObject].refractivity * outputImage[outIndex].red;
            refraction[1] = objects[currentObject].refractivity * outputImage[outIndex].green;
            refraction[2] = objects[currentObject].refractivity * outputImage[outIndex].blue;
        }
    }

    outputImage[outIndex].red = (1 - objects[currentObject].reflectivity - objectRefraction) * color[0] + reflection[0] + refraction[0];
    outputImage[outIndex].green = (1 - objects[currentObject].reflectivity - objectRefraction) * color[1] + reflection[1] + refraction[1];
    outputImage[outIndex].blue = (1 - objects[currentObject].reflectivity - objectRefraction) * color[2] + reflection[2] + refraction[2];

    if(outputImage[outIndex].red > 255) outputImage[outIndex].red = 255;
    if(outputImage[outIndex].green > 255) outputImage[outIndex].green = 255;
    if(outputImage[outIndex].blue > 255) outputImage[outIndex].blue = 255;
}