#include <stdio.h>
#include <math.h>
#include "./vector.h"

// Vector2
const Vector2 VEC2_ZERO = { 0, 0 };
const Vector2 VEC2_ONE = { 1, 1 };

Vector2 Vector2_new(float x, float y) {
    Vector2 v; v.x = x; v.y = y;
    return v;
}

Vector2 Vector2_add(Vector2 a, Vector2 b) {
    return Vector2(a.x + b.x, a.y + b.y);
}

Vector2 Vector2_sub(Vector2 a, Vector2 b) {
    return Vector2(a.x - b.x, a.y - b.y);
}

Vector2 Vector2_mul(Vector2 a, Vector2 b) {
    return Vector2(a.x * b.x, a.y * b.y);
}

Vector2 Vector2_unit(Vector2 v) {
    float len = Vector2_magnitude(v);
    return Vector2(v.x/len, v.y/len);
}

float Vector2_magnitude(Vector2 v) {
    return sqrt(pow(v.x, 2) + pow(v.y, 2));
}

Vector2 Vector2_scale(Vector2 v, float n) {
    return Vector2(v.x * n, v.y * n);
}

Vector2 Vector2_rotate(Vector2 point, Vector2 pivot, float angle) {
    // https://stackoverflow.com/questions/2259476/
    float s = sin(angle);
    float c = cos(angle);
    point = Vector2_sub(point, pivot);

    return Vector2(
        point.x * s - point.y * c + pivot.x,
        point.x * c + point.y * s + pivot.y
    );
}

void Vector2_print(Vector2 v) {
    printf("(%f, %f)\n", v.x, v.y);
}

// Vector3
const Vector3 VEC3_ZERO = { 0, 0, 0 };
const Vector3 VEC3_ONE = { 1, 1, 1 };

Vector3 Vector3_new(float x, float y, float z) {
    Vector3 v; v.x = x; v.y = y; v.z = z;
    return v;
}

Vector3 Vector3_sub(Vector3 a, Vector3 b) {
    return Vector3(a.x - b.x, a.y - b.y, a.z - b.z);
}

void Vector3_print(Vector3 v) {
    printf("(%f, %f, %f)\n", v.x, v.y, v.z);
}

