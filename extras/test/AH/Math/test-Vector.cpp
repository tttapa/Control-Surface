#include <gtest/gtest.h>
#include <AH/Math/Vector.hpp>

using AH::Vec3f;
using AH::Vec2f;

TEST(Vec2f, constructorDefault) {
    Vec2f o        = {};
    ASSERT_EQ(o.x, 0.0f);
    ASSERT_EQ(o.y, 0.0f);
}

TEST(Vec2f, constructorParameters) {
    Vec2f a        = {1, 2};
    ASSERT_EQ(a.x, 1.0f);
    ASSERT_EQ(a.y, 2.0f);
}

TEST(Vec2f, add) {
    Vec2f a        = {1, 2};
    Vec2f b        = {10, 20};
    Vec2f result   = a + b;
    Vec2f expected = {11, 22};
    ASSERT_EQ(result, expected);
}

TEST(Vec2f, addAssign) {
    Vec2f a        = {1, 2};
    Vec2f b        = {10, 20};
    Vec2f result   = (a += b);
    Vec2f expected = {11, 22};
    ASSERT_EQ(a, expected);
    ASSERT_EQ(result, expected);
}

TEST(Vec2f, negate) {
    Vec2f a        = {1, 2};
    Vec2f result   = -a;
    Vec2f expected = {-1, -2};
    ASSERT_EQ(result, expected);
}

TEST(Vec2f, subtract) {
    Vec2f a        = {1, 2};
    Vec2f b        = {10, 20};
    Vec2f result   = b - a;
    Vec2f expected = {9, 18};
    ASSERT_EQ(result, expected);
}

TEST(Vec2f, subtractAssign) {
    Vec2f a        = {1, 2};
    Vec2f b        = {10, 20};
    Vec2f result   = (b -= a);
    Vec2f expected = {9, 18};
    ASSERT_EQ(b, expected);
    ASSERT_EQ(result, expected);
}

TEST(Vec2f, multiplyRight) {
    Vec2f a        = {1, 2};
    float factor   = 3;
    Vec2f result   = a * factor;
    Vec2f expected = {3, 6};
    ASSERT_EQ(result, expected);
}

TEST(Vec2f, multiplyLeft) {
    Vec2f a        = {1, 2};
    float factor   = 3;
    Vec2f result   = factor * a;
    Vec2f expected = {3, 6};
    ASSERT_EQ(result, expected);
}

TEST(Vec2f, multiplyAssign) {
    Vec2f a        = {1, 2};
    float factor   = 3;
    Vec2f result   = (a *= factor);
    Vec2f expected = {3, 6};
    ASSERT_EQ(a, expected);
    ASSERT_EQ(result, expected);
}

TEST(Vec2f, divide) {
    Vec2f b        = {10, 20};
    float factor   = 5;
    Vec2f result   = b / factor;
    Vec2f expected = {2, 4};
    ASSERT_EQ(result, expected);
}

TEST(Vec2f, divideAssign) {
    Vec2f b        = {10, 20};
    float factor   = 5;
    Vec2f result   = (b /= factor);
    Vec2f expected = {2, 4};
    ASSERT_EQ(b, expected);
    ASSERT_EQ(result, expected);
}

TEST(Vec2f, innerProduct) {
    Vec2f a        = {1, 2};
    Vec2f b        = {10, 20};
    float result   = a * b;
    float expected = 50;
    ASSERT_EQ(result, expected);
}

TEST(Vec2f, normSquared) {
    Vec2f a        = {1, 2};
    float result   = a.normSquared();
    float expected = 5;
    ASSERT_EQ(result, expected);
}

TEST(Vec2f, norm) {
    Vec2f a        = {1, 2};
    float result   = a.norm();
    float expected = std::sqrt(5.0f);
    ASSERT_EQ(result, expected);
}

TEST(Vec2f, normalize) {
    Vec2f a        = {1, 2};
    Vec2f result   = a.normalize();
    Vec2f expected = {1 / std::sqrt(5.0f), 2 / std::sqrt(5.0f)};
    ASSERT_EQ(a, expected);
    ASSERT_EQ(result, expected);
}

TEST(Vec2f, normalized) {
    Vec2f a        = {1, 2};
    Vec2f result   = a.normalized();
    Vec2f expected = {1 / std::sqrt(5.0f), 2 / std::sqrt(5.0f)};
    ASSERT_EQ(result, expected);
}

TEST(Vec2f, equality) {
    Vec2f a1 = {1, 2};
    Vec2f a2 = {1, 2};
    Vec2f b = {10, 20};
    bool result1 = (a1 == a2);
    bool expected1 = true;
    bool result2 = (a1 == b);
    bool expected2 = false;
    ASSERT_EQ(result1, expected1);
    ASSERT_EQ(result2, expected2);
}

TEST(Vec2f, inequality) {
    Vec2f a1 = {1, 2};
    Vec2f a2 = {1, 2};
    Vec2f b = {10, 20};
    bool result1 = (a1 != a2);
    bool expected1 = false;
    bool result2 = (a1 != b);
    bool expected2 = true;
    ASSERT_EQ(result1, expected1);
    ASSERT_EQ(result2, expected2);
}


TEST(Vec3f, constructorDefault) {
    Vec3f o        = {};
    ASSERT_EQ(o.x, 0.0f);
    ASSERT_EQ(o.y, 0.0f);
    ASSERT_EQ(o.z, 0.0f);
}

TEST(Vec3f, constructorParameters) {
    Vec3f a        = {1, 2, 3};
    ASSERT_EQ(a.x, 1.0f);
    ASSERT_EQ(a.y, 2.0f);
    ASSERT_EQ(a.z, 3.0f);
}


TEST(Vec3f, add) {
    Vec3f a        = {1, 2, 3};
    Vec3f b        = {10, 20, 30};
    Vec3f result   = a + b;
    Vec3f expected = {11, 22, 33};
    ASSERT_EQ(result, expected);
}

TEST(Vec3f, addAssign) {
    Vec3f a        = {1, 2, 3};
    Vec3f b        = {10, 20, 30};
    Vec3f result   = (a += b);
    Vec3f expected = {11, 22, 33};
    ASSERT_EQ(a, expected);
    ASSERT_EQ(result, expected);
}

TEST(Vec3f, negate) {
    Vec3f a        = {1, 2, 3};
    Vec3f result   = -a;
    Vec3f expected = {-1, -2, -3};
    ASSERT_EQ(result, expected);
}

TEST(Vec3f, subtract) {
    Vec3f a        = {1, 2, 3};
    Vec3f b        = {10, 20, 30};
    Vec3f result   = b - a;
    Vec3f expected = {9, 18, 27};
    ASSERT_EQ(result, expected);
}

TEST(Vec3f, subtractAssign) {
    Vec3f a        = {1, 2, 3};
    Vec3f b        = {10, 20, 30};
    Vec3f result   = (b -= a);
    Vec3f expected = {9, 18, 27};
    ASSERT_EQ(b, expected);
    ASSERT_EQ(result, expected);
}

TEST(Vec3f, multiplyRight) {
    Vec3f a        = {1, 2, 3};
    float factor   = 3;
    Vec3f result   = a * factor;
    Vec3f expected = {3, 6, 9};
    ASSERT_EQ(result, expected);
}

TEST(Vec3f, multiplyLeft) {
    Vec3f a        = {1, 2, 3};
    float factor   = 3;
    Vec3f result   = factor * a;
    Vec3f expected = {3, 6, 9};
    ASSERT_EQ(result, expected);
}

TEST(Vec3f, multiplyAssign) {
    Vec3f a        = {1, 2, 3};
    float factor   = 3;
    Vec3f result   = (a *= factor);
    Vec3f expected = {3, 6, 9};
    ASSERT_EQ(a, expected);
    ASSERT_EQ(result, expected);
}

TEST(Vec3f, divide) {
    Vec3f b        = {10, 20, 30};
    float factor   = 5;
    Vec3f result   = b / factor;
    Vec3f expected = {2, 4, 6};
    ASSERT_EQ(result, expected);
}

TEST(Vec3f, divideAssign) {
    Vec3f b        = {10, 20, 30};
    float factor   = 5;
    Vec3f result   = (b /= factor);
    Vec3f expected = {2, 4, 6};
    ASSERT_EQ(b, expected);
    ASSERT_EQ(result, expected);
}

TEST(Vec3f, innerProduct) {
    Vec3f a        = {1, 2, 3};
    Vec3f b        = {10, 20, 30};
    float result   = a * b;
    float expected = 140;
    ASSERT_EQ(result, expected);
}

TEST(Vec3f, normSquared) {
    Vec3f a        = {1, 2, 3};
    float result   = a.normSquared();
    float expected = 14;
    ASSERT_EQ(result, expected);
}

TEST(Vec3f, norm) {
    Vec3f a        = {1, 2, 3};
    float result   = a.norm();
    float expected = std::sqrt(14.0f);
    ASSERT_EQ(result, expected);
}

TEST(Vec3f, normalize) {
    Vec3f a        = {1, 2, 3};
    Vec3f result   = a.normalize();
    Vec3f expected = {1 / std::sqrt(14.0f), 2 / std::sqrt(14.0f), 3 / std::sqrt(14.0f)};
    ASSERT_EQ(a, expected);
    ASSERT_EQ(result, expected);
}

TEST(Vec3f, normalized) {
    Vec3f a        = {1, 2, 3};
    Vec3f result   = a.normalized();
    Vec3f expected = {1 / std::sqrt(14.0f), 2 / std::sqrt(14.0f), 3 / std::sqrt(14.0f)};
    ASSERT_EQ(result, expected);
}

TEST(Vec3f, equality) {
    Vec3f a1 = {1, 2, 3};
    Vec3f a2 = {1, 2, 3};
    Vec3f b = {10, 20, 30};
    bool result1 = (a1 == a2);
    bool expected1 = true;
    bool result2 = (a1 == b);
    bool expected2 = false;
    ASSERT_EQ(result1, expected1);
    ASSERT_EQ(result2, expected2);
}

TEST(Vec3f, inequality) {
    Vec3f a1 = {1, 2, 3};
    Vec3f a2 = {1, 2, 3};
    Vec3f b = {10, 20, 30};
    bool result1 = (a1 != a2);
    bool expected1 = false;
    bool result2 = (a1 != b);
    bool expected2 = true;
    ASSERT_EQ(result1, expected1);
    ASSERT_EQ(result2, expected2);
}