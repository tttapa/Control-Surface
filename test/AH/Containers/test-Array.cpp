#include <AH/Containers/ArrayHelpers.hpp>
#include <gtest-wrapper.h>

USING_AH_NAMESPACE;

TEST(Array, initializeAndRetrieve) {
    Array<int, 6> arr = {0, 1, 2, 3, 4, 5};
    for (int i = 0; i < 6; i++)
        EXPECT_EQ(arr[i], i);
}

TEST(Array, initializeAndRetrieveConst) {
    const Array<int, 6> arr = {0, 1, 2, 3, 4, 5};
    for (int i = 0; i < 6; i++)
        EXPECT_EQ(arr[i], i);
}

TEST(Array, write) {
    Array<int, 6> arr = {42, 1, 2, 3, 4, 5};
    arr[0] = 0;
    for (int i = 0; i < 6; i++)
        EXPECT_EQ(arr[i], i);
}

TEST(Array, rangeFor) {
    Array<int, 6> arr = {0, 1, 2, 3, 4, 5};
    int i = 0;
    for (int &el : arr)
        EXPECT_EQ(el, i++);
}

TEST(Array, rangeForConst) {
    const Array<int, 6> arr = {0, 1, 2, 3, 4, 5};
    int i = 0;
    for (const int &el : arr)
        EXPECT_EQ(el, i++);
}

TEST(Array, equality) {
    Array<int, 5> a = {1, 2, 3, 4, 5};
    Array<int, 5> b = {1, 2, 3, 4, 5};
    Array<int, 5> c = {1, 2, 3, 4, 6};
    EXPECT_EQ(a, a);
    EXPECT_TRUE(a == a);
    EXPECT_FALSE(a != a);
    EXPECT_EQ(a, b);
    EXPECT_TRUE(a == b);
    EXPECT_FALSE(a != b);
    EXPECT_NE(a, c);
    EXPECT_FALSE(a == c);
    EXPECT_TRUE(a != c);
}

TEST(Array, outOfBounds) {
    Array<int, 5> a = {};
    EXPECT_THROW(a[5], AH::ErrorException);
    const Array<int, 5> b = {};
    EXPECT_THROW(b[5], AH::ErrorException);
}

// Addition

TEST(Array, sliceAdd) {
    Array<int, 5> a = {1, 2, 3, 4, 5};
    Array<int, 5> b = {1, 2, 3, 4, 5};
    Array<int, 3> c = {4, 6, 8};
    auto s1 = a.slice<0, 3>();
    auto s2 = b.slice<2, 5>();
    EXPECT_EQ(s1 + s2, c);
}

TEST(Array, add) {
    Array<int, 3> a = {3, 4, 5};
    Array<int, 3> b = {1, 2, 3};
    Array<int, 3> c = {4, 6, 8};
    EXPECT_EQ(a + b, c);
}

TEST(Array, sliceCompAdd) {
    Array<int, 5> a = {1, 2, 3, 4, 5};
    Array<int, 5> b = {1, 2, 3, 4, 5};
    Array<int, 5> c = {4, 6, 8, 4, 5};
    auto s1 = a.slice<0, 3>();
    auto s2 = b.slice<2, 5>();
    auto s3 = c.slice<0, 3>();
    auto s4 = s1 += s2;
    EXPECT_EQ(s4, s3);
    EXPECT_EQ(a, c);
}

TEST(Array, compAdd) {
    Array<int, 3> a = {3, 4, 5};
    Array<int, 3> b = {1, 2, 3};
    Array<int, 3> c = {4, 6, 8};
    auto d = a += b;
    EXPECT_EQ(d, c);
    EXPECT_EQ(a, c);
}

// Subtraction

TEST(Array, sliceSub) {
    Array<int, 5> a = {1, 2, 3, 4, 5};
    Array<int, 5> b = {1, 2, 4, 8, 16};
    Array<int, 3> c = {-3, -6, -13};
    auto s1 = a.slice<0, 3>();
    auto s2 = b.slice<2, 5>();
    EXPECT_EQ(s1 - s2, c);
}

TEST(Array, sub) {
    Array<int, 3> a = {1, 2, 3};
    Array<int, 3> b = {4, 8, 16};
    Array<int, 3> c = {-3, -6, -13};
    EXPECT_EQ(a - b, c);
}

// Scalar multiplication

TEST(Array, sliceMult) {
    Array<int, 5> a = {1, 2, 3, 4, 5};
    Array<int, 3> b = {4, 6, 8};
    auto s1 = a.slice<1, 4>();
    EXPECT_EQ(s1 * 2, b);
    EXPECT_EQ(2 * s1, b);
}

TEST(Array, mult) {
    Array<int, 5> a = {1, 2, 3, 4, 5};
    Array<int, 5> b = {2, 4, 6, 8, 10};
    EXPECT_EQ(a * 2, b);
    EXPECT_EQ(2 * a, b);
}

TEST(Array, sliceCompMult) {
    Array<int, 5> a = {1, 2, 3, 4, 5};
    Array<int, 5> b = {1, 4, 6, 8, 5};
    auto s1 = a.slice<1, 4>();
    auto c = s1 *= 2;
    auto s3 = b.slice<1, 4>();
    EXPECT_EQ(a, b);
    EXPECT_EQ(c, s3);
}

TEST(Array, compMult) {
    Array<int, 5> a = {1, 2, 3, 4, 5};
    Array<int, 5> b = {2, 4, 6, 8, 10};
    auto c = a *= 2;
    EXPECT_EQ(a, b);
    EXPECT_EQ(c, b);
}

// Scalar division

TEST(Array, sliceDiv) {
    Array<int, 5> a = {2, 4, 6, 8, 10};
    Array<int, 3> b = {2, 3, 4};
    auto s1 = a.slice<1, 4>();
    EXPECT_EQ(s1 / 2, b);
}

TEST(Array, div) {
    Array<int, 5> a = {2, 4, 6, 8, 10};
    Array<int, 5> b = {1, 2, 3, 4, 5};
    EXPECT_EQ(a / 2, b);
}

TEST(Array, sliceCompDiv) {
    Array<int, 5> a = {1, 4, 6, 8, 5};
    Array<int, 5> b = {1, 2, 3, 4, 5};
    auto s1 = a.slice<1, 4>();
    auto c = s1 /= 2;
    auto s3 = b.slice<1, 4>();
    EXPECT_EQ(a, b);
    EXPECT_EQ(c, s3);
}

TEST(Array, compDiv) {
    Array<int, 5> a = {2, 4, 6, 8, 10};
    Array<int, 5> b = {1, 2, 3, 4, 5};
    auto c = a /= 2;
    EXPECT_EQ(a, b);
    EXPECT_EQ(c, b);
}

// -------------------------------------------------------------------------- //

TEST(Array2D, initialize) {
    Array2D<int, 3, 2> arr = {{{0, 1}, {2, 3}, {4, 5}}};
    EXPECT_EQ(arr[0][0], 0);
    EXPECT_EQ(arr[0][1], 1);
    EXPECT_EQ(arr[1][0], 2);
    EXPECT_EQ(arr[1][1], 3);
    EXPECT_EQ(arr[2][0], 4);
    EXPECT_EQ(arr[2][1], 5);
}

// -------------------------------------------------------------------------- //

TEST(generateIncrementalArray, simple) {
    auto x = generateIncrementalArray<unsigned int, 4>(2, 3);
    Array<unsigned int, 4> y = {2, 5, 8, 11};
    EXPECT_EQ(x, y);
}

TEST(generateArray, simple) {
    auto x = generateArray<unsigned int, 4>([i = 0u]() mutable { return i++; });
    Array<unsigned int, 4> y = {0, 1, 2, 3};
    EXPECT_EQ(x, y);
}

TEST(generateArray, simpleNoType) {
    auto x = generateArray<4>([i = 0u]() mutable { return i++; });
    Array<unsigned int, 4> y = {0, 1, 2, 3};
    EXPECT_EQ(x, y);
}

TEST(fillArray, simple) {
    struct S {
        int i;
        float f;
        bool operator!=(S o) const { return this->i != o.i || this->f != o.f; }
    };
    auto x = fillArray<S, 4>(2, 3.14f);
    Array<S, 4> y = {{{2, 3.14f}, {2, 3.14f}, {2, 3.14f}, {2, 3.14f}}};
    EXPECT_EQ(x, y);
}