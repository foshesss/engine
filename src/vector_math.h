#ifndef VECTOR_MATH_H
#define VECTOR_MATH_H

#include <stdarg.h>
#include <stdio.h>

typedef struct {
    float x;
    float y;
} Vector2;

extern const Vector2 VEC2_ZERO;
extern const Vector2 VEC2_ONE;

// constructor
Vector2 Vector2_new(float x, float y);

// math
Vector2 Vector2_add(Vector2 a, Vector2 b);
Vector2 Vector2_sub(Vector2 a, Vector2 b);
Vector2 vec_multiply(Vector2 a, Vector2 b);
Vector2 num_multiply(Vector2 a, float b);
Vector2 divide(Vector2 a, Vector2 b);
Vector2 Vector2_rotate(Vector2 point, Vector2 pivot, float angle);
void Vector2_Unit(Vector2* original);

void Vector2_print(Vector2 v);

#define Vector2(x, y) (Vector2_new(x, y))

#endif