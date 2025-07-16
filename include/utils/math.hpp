#pragma once

#include <cmath>
#include <array>

namespace Math {
    // Vector structure for 3D coordinates
    struct Vector3 {
        float x, y, z;

        Vector3() : x(0.0f), y(0.0f), z(0.0f) {}
        Vector3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}

        // Vector addition
        Vector3 operator+(const Vector3& other) const {
            return Vector3(x + other.x, y + other.y, z + other.z);
        }

        // Vector subtraction
        Vector3 operator-(const Vector3& other) const {
            return Vector3(x - other.x, y - other.y, z - other.z);
        }

        // Scalar multiplication
        Vector3 operator*(float scalar) const {
            return Vector3(x * scalar, y * scalar, z * scalar);
        }
    };

    // Calculate distance between two 3D points
    float Distance(const Vector3& a, const Vector3& b);

    // Calculate dot product of two vectors
    float Dot(const Vector3& a, const Vector3& b);

    // Calculate magnitude (length) of a vector
    float Magnitude(const Vector3& v);

    // Normalize a vector
    Vector3 Normalize(const Vector3& v);

    // Convert world coordinates to screen coordinates (simplified)
    bool WorldToScreen(const Vector3& worldPos, float* screenX, float* screenY, const float* viewMatrix);

    // Calculate angles between two points (for aimbot)
    Vector3 CalcAngle(const Vector3& src, const Vector3& dst);
}