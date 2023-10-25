#ifndef VECTOR_H
#define VECTOR_H

// -- Vector2 --
typedef struct {
    float x;
    float y;
} Vector2;

extern const Vector2 VEC2_ZERO;
extern const Vector2 VEC2_ONE;

// constructor
Vector2 Vector2_new(float x, float y);
#define Vector2(x, y) (Vector2_new(x, y))

// math
Vector2 Vector2_add(Vector2 a, Vector2 b);
Vector2 Vector2_sub(Vector2 a, Vector2 b);
Vector2 Vector2_mul(Vector2 a, Vector2 b);
Vector2 Vector2_div(Vector2 a, Vector2 b);
Vector2 Vector2_scale(Vector2 v, float b);
Vector2 Vector2_rotate(Vector2 point, Vector2 pivot, float angle);
Vector2 Vector2_unit(Vector2 v);

float Vector2_magnitude(Vector2 v);

// general
void Vector2_print(Vector2 v);

// -- Vector3 --
typedef struct {
    float x;
    float y;
    float z;
} Vector3;

extern const Vector3 VEC3_ZERO;
extern const Vector3 VEC3_ONE;

// constructor
Vector3 Vector3_new(float x, float y, float z);
#define Vector3(x, y, z) (Vector3_new(x, y, z))

// math
Vector3 Vector3_sub(Vector3 a, Vector3 b);

// general
void Vector3_print(Vector3 v);

#endif