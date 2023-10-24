#include <math.h>
#include "./vector_math.h"

const Vector2 VEC2_ZERO = { 0, 0 };
const Vector2 VEC2_ONE = { 1, 1 };

Vector2 Vector2_new(float x, float y) {
    Vector2 v;
    v.x = x;
    v.y = y;
    return v;
}

Vector2 Vector2_add(Vector2 a, Vector2 b) {
    return Vector2(a.x + b.x, a.y + b.y);
}

Vector2 Vector2_sub(Vector2 a, Vector2 b) {
    return Vector2(a.x - b.x, a.y - b.y);
}

Vector2 vec_multiply(Vector2 a, Vector2 b) {
    return Vector2(a.x * b.x, a.y * b.y);
}

Vector2 num_multiply(Vector2 a, float n) {
    return Vector2(a.x * n, a.y * n);
}

// https://stackoverflow.com/questions/2259476/rotating-a-point-about-another-point-2d
Vector2 Vector2_rotate(Vector2 point, Vector2 pivot, float angle) {
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

