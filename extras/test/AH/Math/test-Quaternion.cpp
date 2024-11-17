#include <AH/Math/Degrees.hpp>
#include <AH/Math/Quaternion.hpp>
#include <gtest/gtest.h>

#include <random>

static constexpr float eps = 1e2f * std::numeric_limits<float>::epsilon();

// -------------------------------------------------------------------------- //
//                                 QUATERNION                                 //
// -------------------------------------------------------------------------- //

using AH::EulerAngles;
using AH::Quaternion;
using AH::Vec3f;
using AH::operator""_deg;

TEST(Quaternion, constructorDefault) {
    Quaternion def = {};
    ASSERT_EQ(def.w, 1.0f);
    ASSERT_EQ(def.x, 0.0f);
    ASSERT_EQ(def.y, 0.0f);
    ASSERT_EQ(def.z, 0.0f);
}

TEST(Quaternion, constructorParameters) {
    Quaternion q = {1, 2, 5, 7};
    ASSERT_EQ(q.w, 1.0f);
    ASSERT_EQ(q.x, 2.0f);
    ASSERT_EQ(q.y, 5.0f);
    ASSERT_EQ(q.z, 7.0f);
}

TEST(Quaternion, identity) {
    Quaternion q = Quaternion::identity();
    ASSERT_EQ(q.w, 1.0f);
    ASSERT_EQ(q.x, 0.0f);
    ASSERT_EQ(q.y, 0.0f);
    ASSERT_EQ(q.z, 0.0f);
}

TEST(Quaternion, hamiltonionProduct) {
    Quaternion q = {1, 2, 5, 7};
    Quaternion r = {11, 13, 17, 19};
    Quaternion result = Quaternion::hamiltonianProduct(q, r);
    Quaternion expected = {-233, 11, 125, 65};
    ASSERT_EQ(result, expected);
}

TEST(Quaternion, add) {
    Quaternion q = {1, 2, 5, 7};
    Quaternion r = {11, 13, 17, 19};
    Quaternion result = q + r;
    Quaternion expected = {-233, 11, 125, 65};
    ASSERT_EQ(result, expected);
}

TEST(Quaternion, addAssign) {
    Quaternion q = {1, 2, 5, 7};
    Quaternion r = {11, 13, 17, 19};
    Quaternion result = (q += r);
    Quaternion expected = {-233, 11, 125, 65};
    ASSERT_EQ(q, expected);
    ASSERT_EQ(result, expected);
}

TEST(Quaternion, conjugated) {
    Quaternion q = {1, 2, 5, 7};
    Quaternion result = q.conjugated();
    Quaternion expected = {1, -2, -5, -7};
    ASSERT_EQ(result, expected);
}

TEST(Quaternion, negate) {
    Quaternion q = {1, 2, 5, 7};
    Quaternion result = -q;
    Quaternion expected = {1, -2, -5, -7};
    ASSERT_EQ(result, expected);
}

TEST(Quaternion, subtract) {
    Quaternion q = {1, 2, 5, 7};
    Quaternion r = {11, 13, 17, 19};
    Quaternion result = r - q;
    Quaternion expected = {255, -33, 15, -89};
    ASSERT_EQ(result, expected);
}

TEST(Quaternion, subtractAssign) {
    Quaternion q = {1, 2, 5, 7};
    Quaternion r = {11, 13, 17, 19};
    Quaternion result = (r -= q);
    Quaternion expected = {255, -33, 15, -89};
    ASSERT_EQ(r, expected);
    ASSERT_EQ(result, expected);
}

TEST(Quaternion, multiplyRight) {
    Quaternion q = {1, 2, 5, 7};
    float factor = 3;
    Quaternion result = q * factor;
    Quaternion expected = {3, 6, 15, 21};
    ASSERT_EQ(result, expected);
}

TEST(Quaternion, multiplyLeft) {
    Quaternion q = {1, 2, 5, 7};
    float factor = 3;
    Quaternion result = factor * q;
    Quaternion expected = {3, 6, 15, 21};
    ASSERT_EQ(result, expected);
}

TEST(Quaternion, multiplyAssign) {
    Quaternion q = {1, 2, 5, 7};
    float factor = 3;
    Quaternion result = (q *= factor);
    Quaternion expected = {3, 6, 15, 21};
    ASSERT_EQ(q, expected);
    ASSERT_EQ(result, expected);
}

TEST(Quaternion, divide) {
    Quaternion q = {1, 2, 5, 7};
    float factor = 2;
    Quaternion result = q / factor;
    Quaternion expected = {0.5, 1, 2.5, 3.5};
    ASSERT_EQ(result, expected);
}

TEST(Quaternion, divideAssign) {
    Quaternion q = {1, 2, 5, 7};
    float factor = 2;
    Quaternion result = (q /= factor);
    Quaternion expected = {0.5, 1, 2.5, 3.5};
    ASSERT_EQ(q, expected);
    ASSERT_EQ(result, expected);
}

TEST(Quaternion, normSquared) {
    Quaternion q = {1, 2, 5, 7};
    float result = q.normSquared();
    float expected = 79;
    ASSERT_EQ(result, expected);
}

TEST(Quaternion, norm) {
    Quaternion q = {1, 2, 5, 7};
    float result = q.norm();
    float expected = std::sqrt(79.0f);
    ASSERT_EQ(result, expected);
}

TEST(Quaternion, normalize) {
    Quaternion q = {1, 2, 5, 7};
    Quaternion result = q.normalize();
    Quaternion expected = {1 / std::sqrt(79.0f), 2 / std::sqrt(79.0f),
                           5 / std::sqrt(79.0f), 7 / std::sqrt(79.0f)};
    ASSERT_EQ(q, expected);
    ASSERT_EQ(result, expected);
}

TEST(Quaternion, normalized) {
    Quaternion q = {1, 2, 5, 7};
    Quaternion result = q.normalized();
    Quaternion expected = {1 / std::sqrt(79.0f), 2 / std::sqrt(79.0f),
                           5 / std::sqrt(79.0f), 7 / std::sqrt(79.0f)};
    ASSERT_EQ(result, expected);
}

TEST(Quaternion, rotate) {
    Quaternion q = {1, 2, 5, 7};
    Vec3f v1 = {1, 3, 5};
    Vec3f v2 = {2, 4, 6};
    Vec3f result1 = q.rotate(v1);
    Vec3f expected1 = {1.759493670886076, 3.582278481012658, 4.367088607594937};
    Vec3f result2 = q.rotate(v2);
    Vec3f expected2 = {1.443037974683544, 4.506329113924051, 5.797468354430380};

    // Expected is calculated using MATLAB (double precision), so compare its
    // components
    ASSERT_FLOAT_EQ(result1.x, expected1.x);
    ASSERT_FLOAT_EQ(result1.y, expected1.y);
    ASSERT_FLOAT_EQ(result1.z, expected1.z);
    ASSERT_FLOAT_EQ(result2.x, expected2.x);
    ASSERT_FLOAT_EQ(result2.y, expected2.y);
    ASSERT_FLOAT_EQ(result2.z, expected2.z);
}

TEST(Quaternion, equality) {
    Quaternion q1 = {1, 2, 5, 7};
    Quaternion q2 = {1, 2, 5, 7};
    Quaternion r = {11, 13, 17, 19};
    bool result1 = (q1 == q2);
    bool expected1 = true;
    bool result2 = (q1 == r);
    bool expected2 = false;
    ASSERT_EQ(result1, expected1);
    ASSERT_EQ(result2, expected2);
}

TEST(Quaternion, inequality) {
    Quaternion q1 = {1, 2, 5, 7};
    Quaternion q2 = {1, 2, 5, 7};
    Quaternion r = {11, 13, 17, 19};
    bool result1 = (q1 != q2);
    bool expected1 = false;
    bool result2 = (q1 != r);
    bool expected2 = true;
    ASSERT_EQ(result1, expected1);
    ASSERT_EQ(result2, expected2);
}

TEST(Quaternion, fromDirectionSimple) {
    Vec3f vector = {0.0, 1.0, std::sqrt(3.0f)};
    Quaternion result = Quaternion::fromDirection(vector);
    Quaternion expected = {0.965925826289068, -0.258819045102521, 0, 0}; // 30°

    // Expected is calculated using MATLAB (double precision), so compare its
    // components.
    ASSERT_FLOAT_EQ(result.w, expected.w);
    ASSERT_FLOAT_EQ(result.x, expected.x);
    ASSERT_FLOAT_EQ(result.y, expected.y);
    ASSERT_FLOAT_EQ(result.z, expected.z);

    // Make sure it satisfies the definition of `fromDirection()`.
    float diff = (result.rotate({0, 0, 1}) - vector.normalized()).norm();
    ASSERT_LT(diff, eps);
}

TEST(Quaternion, fromDirectionVertical) {
    Vec3f vector = {1e-18, 1e-18, 1};
    Quaternion result = Quaternion::fromDirection(vector);
    Quaternion expected = Quaternion::identity();

    // Expected is calculated using MATLAB (double precision), so compare its
    // components.
    ASSERT_FLOAT_EQ(result.w, expected.w);
    ASSERT_FLOAT_EQ(result.x, expected.x);
    ASSERT_FLOAT_EQ(result.y, expected.y);
    ASSERT_FLOAT_EQ(result.z, expected.z);

    // Make sure it satisfies the definition of `fromDirection()`.
    float diff = (result.rotate({0, 0, 1}) - vector.normalized()).norm();
    ASSERT_LT(diff, eps);
}

TEST(Quaternion, fromDirectionHard) {
    Vec3f vector = {-0.414578098794425, 0.829156197588850, 0.375000000000000};
    Quaternion result = Quaternion::fromDirection(vector);
    Quaternion expected = {0.829156197588850, -0.5, -0.25, 0.0};

    // Expected is calculated using MATLAB (double precision), so compare its
    // components
    ASSERT_FLOAT_EQ(result.w, expected.w);
    ASSERT_FLOAT_EQ(result.x, expected.x);
    ASSERT_FLOAT_EQ(result.y, expected.y);
    ASSERT_FLOAT_EQ(result.z, expected.z);

    // Make sure it satisfies the definition of `fromDirection()`.
    float diff = (result.rotate({0, 0, 1}) - vector.normalized()).norm();
    ASSERT_LT(diff, eps);
}

TEST(Quaternion, fromDirectionInvalid) {
    Vec3f vector = {0, 0, 0};
    Quaternion result = Quaternion::fromDirection(vector);
    Quaternion expected = {0, 0, 0, 0};
    EXPECT_EQ(result, expected);
}

TEST(Quaternion, fromDirectionRandom) {
    std::mt19937_64 generator(0);
    std::uniform_real_distribution<float> distribution(0.0, M_PI);

    for (size_t i = 0; i < 1'000'000; ++i) {
        // Pick a random vector somewhere on the unit sphere
        // https://en.wikipedia.org/wiki/Spherical_coordinate_system
        float phi = 2 * distribution(generator); // [0, 2π]
        float theta = distribution(generator);   // [0, π]
        Vec3f vector = {
            std::sin(theta) * std::cos(phi),
            std::sin(theta) * std::sin(phi),
            std::cos(theta),
        };
        // Convert the vector to its orientation relative to (0 0 1)
        Quaternion result = Quaternion::fromDirection(vector);

        // Make sure it satisfies the definition of `fromDirection()`
        Vec3f diff = result.rotate({0, 0, 1}) - vector.normalized();
        ASSERT_LT(diff.norm(), eps * 40)
            << "i = " << i << ", phi = " << phi << ", theta = " << theta
            << ", vector = " << vector << ", quat = " << result << std::endl;
    }
}

TEST(Quaternion, fromXYAngle) {
    float xAngle = 10_deg;
    float yAngle = -20_deg;
    Quaternion q = Quaternion::fromXYAngle(xAngle, yAngle);
    Vec3f result = q.rotate(Vec3f{0, 0, 1});
    Vec3f expected = {std::tan(yAngle), -std::tan(xAngle), 1};
    expected.normalize();

    // Expected is calculated using MATLAB (double precision), so compare its
    // components
    ASSERT_FLOAT_EQ(result.x, expected.x);
    ASSERT_FLOAT_EQ(result.y, expected.y);
    ASSERT_FLOAT_EQ(result.z, expected.z);
}

// -------------------------------------------------------------------------- //
//                                EULER ANGLES                                //
// -------------------------------------------------------------------------- //

TEST(EulerAngles, constructorDefault) {
    EulerAngles def = {};
    ASSERT_EQ(def.yaw, 0.0f);
    ASSERT_EQ(def.pitch, 0.0f);
    ASSERT_EQ(def.roll, 0.0f);
}

TEST(EulerAngles, constructorParameters) {
    EulerAngles e = {2, 3, 5};
    ASSERT_EQ(e.yaw, 2.0f);
    ASSERT_EQ(e.pitch, 3.0f);
    ASSERT_EQ(e.roll, 5.0f);
}

TEST(EulerAngles, constructorQuaternion) {
    Quaternion q = {1, 2, 5, 7};
    EulerAngles result = q;
    EulerAngles expected = EulerAngles::quat2eul(q);
    ASSERT_EQ(result, expected);
}

TEST(EulerAngles, implicitConversionToQuat) {
    EulerAngles e = {2, 3, 5};
    Quaternion result = e;
    Quaternion expected = EulerAngles::eul2quat(e);
    ASSERT_EQ(result, expected);
}

TEST(EulerAngles, quat2eul) {
    Quaternion q = {1, 2, 5, 7};
    EulerAngles result = EulerAngles::quat2eul(q);
    EulerAngles expected = {2.68375893257083, -0.229867079467390,
                            1.29428238017742};

    // Expected is calculated using MATLAB (double precision), so compare its
    // components.
    ASSERT_FLOAT_EQ(result.yaw, expected.yaw);
    ASSERT_FLOAT_EQ(result.pitch, expected.pitch);
    ASSERT_FLOAT_EQ(result.roll, expected.roll);
}

TEST(EulerAngles, eul2quat) {
    EulerAngles e = {2, 3, 5};
    Quaternion result = EulerAngles::eul2quat(e);
    Quaternion expected = {0.471716140466081, 0.695323669708306,
                           -0.396152384736371, -0.370232582621850};

    // Expected is calculated using MATLAB (double precision), so compare its
    // components.
    ASSERT_FLOAT_EQ(result.w, expected.w);
    ASSERT_FLOAT_EQ(result.x, expected.x);
    ASSERT_FLOAT_EQ(result.y, expected.y);
    ASSERT_FLOAT_EQ(result.z, expected.z);
}

TEST(Quaternion, eul2quatAndBack) {
    EulerAngles e = {0.3, -0.7, 0.9};
    Quaternion q = e;
    Quaternion q_expected = {0.814068885161671, 0.450147392727527,
                             -0.244234643193019, 0.273877005417802};
    EulerAngles result = q;
    EulerAngles expected = e;

    // Expected is calculated using MATLAB (double precision), so compare its
    // components.
    ASSERT_FLOAT_EQ(q.w, q_expected.w);
    ASSERT_FLOAT_EQ(q.x, q_expected.x);
    ASSERT_FLOAT_EQ(q.y, q_expected.y);
    ASSERT_FLOAT_EQ(q.z, q_expected.z);
    ASSERT_FLOAT_EQ(result.yaw, expected.yaw);
    ASSERT_FLOAT_EQ(result.pitch, expected.pitch);
    ASSERT_FLOAT_EQ(result.roll, expected.roll);
}

TEST(Quaternion, quat2eulAndBack) {
    Quaternion q = {1, 2, 5, 11};
    q.normalize(); // Conversion to EulerAngles loses the length of q.
    EulerAngles e = q;
    EulerAngles e_expected = {2.85208898162841, -0.227112959382601,
                              0.886501535133747};
    Quaternion result = e;
    Quaternion expected = q;

    // Expected is calculated using MATLAB (double precision), so compare its
    // components.
    ASSERT_FLOAT_EQ(e.yaw, e_expected.yaw);
    ASSERT_FLOAT_EQ(e.pitch, e_expected.pitch);
    ASSERT_FLOAT_EQ(e.roll, e_expected.roll);
    ASSERT_FLOAT_EQ(result.w, expected.w);
    ASSERT_FLOAT_EQ(result.x, expected.x);
    ASSERT_FLOAT_EQ(result.y, expected.y);
    ASSERT_FLOAT_EQ(result.z, expected.z);
}