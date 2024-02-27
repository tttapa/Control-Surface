/**
 * @file
 * @brief   Definition of Vec2f and Vec3f.
 * 
 * Vec2fs can be added, subtracted, multiplied (dot product) and normalized.
 * It also has an implementation of the following operators:
 *      "+", "+=", "-", "-=", "*" (vector-vector);
 *      "*", "*=", "/", "/=" (vector-scalar);
 *      "==", "!=" (equality);
 *      "*" (scalar-vector);
 *      "<<" (printing).
 * The same applies for Vec3fs.
 */
#pragma once

#include <AH/Arduino-Wrapper.h> // Print
#include <AH/STL/cmath>         // std::sqrt

#ifndef ARDUINO
#include <iosfwd> // std::ostream
#endif

BEGIN_AH_NAMESPACE

/// @addtogroup  math-types
/// @{

/**
 * @brief   Type for 2D vectors of floating point numbers.
 *
 * Vec2fs can be added, subtracted, multiplied (dot product) and normalized.
 * It also has an implementation of the following operators:
 *      "+", "+=", "-", "-=", "*" (vector-vector);
 *      "*", "*=", "/", "/=" (vector-scalar);
 *      "==", "!=" (equality);
 *      "*" (scalar-vector);
 *      "<<" (printing).
 */
struct Vec2f {
    float x = 0.0; ///< The x component of the vector.
    float y = 0.0; ///< The y component of the vector.

    /// Create a vector that is initialized to the zero vector (0,0).
    Vec2f() = default;
    /// Create a vector with the given x and y coordinates.
    Vec2f(float x, float y) : x(x), y(y) {}

    /// Addition.
    Vec2f &operator+=(Vec2f rhs) {
        x += rhs.x;
        y += rhs.y;
        return *this;
    }
    /// Addition.
    Vec2f operator+(Vec2f rhs) const {
        Vec2f result = *this;
        result += rhs;
        return result;
    }

    /// Negation.
    Vec2f operator-() const { return {-x, -y}; }
    /// Subtraction.
    Vec2f &operator-=(Vec2f rhs) { return *this += -rhs; }
    /// Subtraction.
    Vec2f operator-(Vec2f rhs) const {
        Vec2f result = *this;
        result -= rhs;
        return result;
    }

    /// Scalar multiplication.
    Vec2f &operator*=(float rhs) {
        x *= rhs;
        y *= rhs;
        return *this;
    }
    /// Scalar multiplication.
    Vec2f operator*(float rhs) const {
        Vec2f result = *this;
        result *= rhs;
        return result;
    }

    /// Scalar division.
    Vec2f &operator/=(float rhs) {
        x /= rhs;
        y /= rhs;
        return *this;
    }
    /// Scalar division.
    Vec2f operator/(float rhs) const {
        Vec2f result = *this;
        result /= rhs;
        return result;
    }

    /// Inner product.
    float operator*(Vec2f rhs) const {
        return this->x * rhs.x + this->y * rhs.y;
    }

    /// Norm squared.
    float normSquared() const { return (*this) * (*this); }
    /// Norm.
    float norm() const {
        // return std::sqrt(normSquared()); // faster but less accurate
        return std::hypot(x, y);
    }
    /// Normalize this vector.
    Vec2f &normalize() { return *this /= norm(); }
    /// Normalize a copy of this vector (doesn't change the original vector).
    Vec2f normalized() const { return *this / norm(); }

    /// Equality check.
    bool operator==(Vec2f rhs) const {
        return this->x == rhs.x && this->y == rhs.y;
    }
    /// Inequality check.
    bool operator!=(Vec2f rhs) const { return !(*this == rhs); }
};

/// Scalar multiplication.
/// @related  Vec2f
inline Vec2f operator*(float lhs, Vec2f rhs) {
    return {lhs * rhs.x, lhs * rhs.y};
}

/**
 * @brief   Type for 3D vectors of floating point numbers.
 *
 * Vec3fs can be added, subtracted, multiplied (dot product) and normalized.
 * It also has an implementation of the following operators:
 *      "+", "+=", "-", "-=", "*" (vector-vector);
 *      "*", "*=", "/", "/=" (vector-scalar);
 *      "==", "!=" (equality);
 *      "*" (scalar-vector);
 *      "<<" (printing).
 */
struct Vec3f {
    float x = 0.0; ///< The x component of the vector.
    float y = 0.0; ///< The y component of the vector.
    float z = 0.0; ///< The z component of the vector.

    /// Create a vector that is initialized to the zero vector (0,0,0).
    Vec3f() = default;
    /// Create a vector with the given x, y and z coordinates.
    Vec3f(float x, float y, float z) : x(x), y(y), z(z) {}
    Vec3f(const Vec3f &) = default;
    Vec3f(const volatile Vec3f &other) : x(other.x), y(other.y), z(other.z) {}
    void operator=(const Vec3f &other) volatile {
        this->x = other.x;
        this->y = other.y;
        this->z = other.z;
    }

    /// Addition.
    Vec3f &operator+=(Vec3f rhs) {
        x += rhs.x;
        y += rhs.y;
        z += rhs.z;
        return *this;
    }
    /// Addition.
    Vec3f operator+(Vec3f rhs) const {
        Vec3f result = *this;
        result += rhs;
        return result;
    }

    /// Negation.
    Vec3f operator-() const { return {-x, -y, -z}; }
    /// Subtraction.
    Vec3f &operator-=(Vec3f rhs) { return *this += -rhs; }
    /// Subtraction.
    Vec3f operator-(Vec3f rhs) const {
        Vec3f result = *this;
        result -= rhs;
        return result;
    }

    /// Scalar multiplication.
    Vec3f &operator*=(float rhs) {
        x *= rhs;
        y *= rhs;
        z *= rhs;
        return *this;
    }
    /// Scalar multiplication.
    Vec3f operator*(float rhs) const {
        Vec3f result = *this;
        result *= rhs;
        return result;
    }

    /// Scalar division.
    Vec3f &operator/=(float rhs) {
        x /= rhs;
        y /= rhs;
        z /= rhs;
        return *this;
    }
    /// Scalar division.
    Vec3f operator/(float rhs) const {
        Vec3f result = *this;
        result /= rhs;
        return result;
    }

    /// Inner product.
    float operator*(Vec3f rhs) const {
        return this->x * rhs.x + this->y * rhs.y + this->z * rhs.z;
    }

    /// Norm squared.
    float normSquared() const { return (*this) * (*this); }

    /// Norm.
    float norm() const {
        return std::sqrt(normSquared());
        // return std::hypot(x, y, z);  // C++17
    }

    /// Normalize this vector.
    Vec3f &normalize() { return *this /= norm(); }
    /// Normalize a copy of this vector (doesn't change the original vector).
    Vec3f normalized() const { return *this / norm(); }

    /// Equality check.
    bool operator==(Vec3f rhs) const {
        return this->x == rhs.x && this->y == rhs.y && this->z == rhs.z;
    }
    /// Inequality check.
    bool operator!=(Vec3f rhs) const { return !(*this == rhs); }
};

/// Scalar multiplication.
/// @related  Vec3f
inline Vec3f operator*(float lhs, Vec3f rhs) {
    return {lhs * rhs.x, lhs * rhs.y, lhs * rhs.z};
}

#ifndef ARDUINO

/// Printing.
/// @related  Vec2f
std::ostream &operator<<(std::ostream &os, Vec2f v);

/// Printing.
/// @related  Vec3f
std::ostream &operator<<(std::ostream &os, Vec3f v);

#endif

/// Printing.
/// @related  Vec2f
Print &operator<<(Print &os, Vec2f v);

/// Printing.
/// @related  Vec3f
Print &operator<<(Print &os, Vec3f v);

/// @}

END_AH_NAMESPACE
