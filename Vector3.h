#ifndef VECTOR3_H
#define VECTOR3_H

#include <math.h>

class Vector3
{
public:
    float x;
    float y;
    float z;

    Vector3() 
    {
        x = 0;
        y = 0;
        z = 0;
    }

    Vector3(float x, float y, float z) 
    {
        this->x = x;
        this->y = y;
        this->z = z;
    }

    /// <summary>
    /// Calculates the Euclidean distance between point a and b.
    /// </summary>
    static float Distance(Vector3 a, Vector3 b)
    {
        float dx = a.x - b.x;
        float dy = a.y - b.y;
        float dz = a.z - b.z;

        dx = (dx * dx);
        dy = (dy * dy);
        dz = (dz * dz);

        return sqrt((float)((double)dx + (double)dy + (double)dz));
    }

    /// <summary>
    /// Calculates the Manhattan distance between point a and b.
    /// </summary>
    static float ManhattanDistance(Vector3 a, Vector3 b)
    {
        //return (a.x - b.x) + (a.y - b.y) + (a.z - b.z);
        return abs(a.x - b.x) + abs(a.y - b.y) + abs(a.z - b.z);
    }
};

#endif