#pragma once

#include <cmath>

namespace game_ref
{
    struct Vector2
    {
        float x = 0.0f;
        float y = 0.0f;

        constexpr Vector2() = default;
        constexpr Vector2(float initialX, float initialY) : x(initialX), y(initialY) {}

        constexpr Vector2 operator+(const Vector2& other) const
        {
            return { x + other.x, y + other.y };
        }

        constexpr Vector2 operator-(const Vector2& other) const
        {
            return { x - other.x, y - other.y };
        }

        constexpr Vector2 operator*(float scalar) const
        {
            return { x * scalar, y * scalar };
        }

        Vector2& operator+=(const Vector2& other)
        {
            x += other.x;
            y += other.y;
            return *this;
        }

        [[nodiscard]] float LengthSquared() const
        {
            return x * x + y * y;
        }

        [[nodiscard]] float Length() const
        {
            return std::sqrt(LengthSquared());
        }

        [[nodiscard]] Vector2 Normalized() const
        {
            const float length = Length();
            if (length <= 0.0001f)
            {
                return {};
            }

            return { x / length, y / length };
        }
    };

    inline float Distance(const Vector2& a, const Vector2& b)
    {
        return (a - b).Length();
    }
}
