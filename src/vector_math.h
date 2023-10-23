#ifndef VECTOR_MATH_H
#define VECTOR_MATH_H

#include <stdarg.h>
#include <stdio.h>

#define PI 3.1415

typedef struct {
    float x;
    float y;
} Vector2;

extern const Vector2 VEC2_ZERO;
extern const Vector2 VEC2_ONE;

// constructor
Vector2 new(float x, float y);

// math
Vector2 add(Vector2 a, Vector2 b);
Vector2 subtract(Vector2 a, Vector2 b);
Vector2 vec_multiply(Vector2 a, Vector2 b);
Vector2 num_multiply(Vector2 a, float b);
Vector2 divide(Vector2 a, Vector2 b);
Vector2 rotate_vector(Vector2 point, Vector2 pivot, float angle);

void print(Vector2 v);

#define Vector2(x, y) (new(x, y))

#endif