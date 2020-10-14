#pragma once

typedef struct
{
    union
    {
        struct
        {
            float x;
            float y;
        };

        float coords[2];
    };
} Vec2;

typedef struct
{
    union
    {
        struct
        {
            float x;
            float y;
            float z;
        };

        float coords[3];
    };
} Vec3;

typedef struct
{
    union
    {
        struct
        {
            float x;
            float y;
            float z;
            float w;
        };

        float coords[4];
    };
} Vec4;
