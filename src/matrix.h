#ifndef MATRIX_H
#define MATRIX_H

#include "./vector.h"

typedef struct {
    float m[4][4];
} matrix4x4;

Vector3 Matrix4x4_mul(Vector3 i, matrix4x4 m);

#endif