#include "utils/math.hpp"

namespace Math {
    float Distance(const Vector3& a, const Vector3& b) {
        float dx = a.x - b.x;
        float dy = a.y - b.y;
        float dz = a.z - b.z;
        return std::sqrt(dx * dx + dy * dy + dz * dz);
    }

    float Dot(const Vector3& a, const Vector3& b) {
        return a.x * b.x + a.y * b.y + a.z * b.z;
    }

    float Magnitude(const Vector3& v) {
        return std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
    }

    Vector3 Normalize(const Vector3& v) {
        float mag = Magnitude(v);
        if (mag == 0.0f) return Vector3(0.0f, 0.0f, 0.0f);
        return Vector3(v.x / mag, v.y / mag, v.z / mag);
    }

    bool WorldToScreen(const Vector3& worldPos, float* screenX, float* screenY, const float* viewMatrix) {
        // Assuming viewMatrix is a 4x4 matrix (16 floats) for world-to-screen projection
        float w = viewMatrix[12] * worldPos.x + viewMatrix[13] * worldPos.y + viewMatrix[14] * worldPos.z + viewMatrix[15];
        if (w < 0.01f) return false; // Behind camera

        float x = viewMatrix[0] * worldPos.x + viewMatrix[1] * worldPos.y + viewMatrix[2] * worldPos.z + viewMatrix[3];
        float y = viewMatrix[4] * worldPos.x + viewMatrix[5] * worldPos.y + viewMatrix[6] * worldPos.z + viewMatrix[7];

        // Assuming screen resolution (hardcoded for simplicity, adjust as needed)
        const float screenWidth = 1920.0f;
        const float screenHeight = 1080.0f;

        *screenX = (screenWidth / 2.0f) * (1.0f + (x / w));
        *screenY = (screenHeight / 2.0f) * (1.0f - (y / w));

        return (*screenX >= 0 && *screenX <= screenWidth && *screenY >= 0 && *screenY <= screenHeight);
    }

    Vector3 CalcAngle(const Vector3& src, const Vector3& dst) {
        Vector3 delta = dst - src;
        float hyp = Magnitude(delta);
        if (hyp == 0.0f) return Vector3(0.0f, 0.0f, 0.0f);

        float pitch = -std::asin(delta.z / hyp) * (180.0f / 3.14159f);
        float yaw = std::atan2(delta.y, delta.x) * (180.0f / 3.14159f);
        return Vector3(pitch, yaw, 0.0f); // Roll is typically unused in FPS games
    }
}