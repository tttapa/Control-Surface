#include <AH/Containers/ArrayHelpers.hpp>
#include <gtest/gtest.h>

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

TEST(Array, sliceEquality) {
    Array<int, 5> aa = {1, 2, 3, 4, 5};
    auto a = aa.slice();
    Array<int, 5> ba = {1, 2, 3, 4, 5};
    auto b = ba.slice();
    Array<int, 5> ca = {1, 2, 3, 4, 6};
    auto c = ca.slice();
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

TEST(Array, sliceOutOfBounds) {
    Array<int, 5> a = {};
    auto sa = a.slice<0, 1>();
    EXPECT_THROW(sa[2], AH::ErrorException);
    const Array<int, 5> b = {};
    auto sb = b.slice<3, 4>();
    EXPECT_THROW(sb[2], AH::ErrorException);
}

// Addition

TEST(Array, sliceAdd) {
    Array<int, 5> a = {1, 2, 3, 4, 5};
    Array<int, 5> b = {1, 2, 3, 4, 5};
    Array<int, 3> c = {4, 6, 8};
    auto s1 = a.slice<0, 2>();
    auto s2 = b.slice<2, 4>();
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
    auto s1 = a.slice<0, 2>();
    auto s2 = b.slice<2, 4>();
    auto s3 = c.slice<0, 2>();
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
    auto s1 = a.slice<0, 2>();
    auto s2 = b.slice<2, 4>();
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
    auto s1 = a.slice<1, 3>();
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
    auto s1 = a.slice<1, 3>();
    auto c = s1 *= 2;
    auto s3 = b.slice<1, 3>();
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
    auto s1 = a.slice<1, 3>();
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
    auto s1 = a.slice<1, 3>();
    auto c = s1 /= 2;
    auto s3 = b.slice<1, 3>();
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

// Negation

TEST(Array, sliceNegate) {
    Array<int, 5> a = {2, -4, -6, 8, 0};
    Array<int, 4> b = {4, 6, -8, 0};
    auto s1 = -a.slice<1, 4>();
    EXPECT_EQ(s1, b);
}

TEST(Array, negate) {
    Array<int, 5> a = {2, -4, -6, 8, 0};
    Array<int, 5> b = {-2, 4, 6, -8, 0};
    EXPECT_EQ(-a, b);
}

// Convert slice to array

TEST(Array, sliceToArray) {
    Array<int, 5> a = {2, 4, 6, 8, 10};
    Array<int, 3> b = a.slice<1, 3>().asArray();
    Array<int, 3> c = {4, 6, 8};
    EXPECT_EQ(b, c);
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

TEST(Array, apply) {
    Array<int, 5> a = {-1, 2, -3, 4, 0};
    Array<int, 5> b = {1, -2, 3, -4, 0};
    auto c = apply(a, std::negate<>());
    EXPECT_EQ(c, b);
}

// -----------------------------------------------------------------------------

TEST(Array, sliceDistributeSlice) {
    Array<int, 9> a = {3, 1, -4, 2, 10, 12, 13, 14, 15};
    Array<int, 20> b = {11, 3, 7, 11, 13};
    Array<int, 6> c = {-12, -22, 0, 40, 136, 130};
    auto s1 = a.slice<2, 4>();
    auto s2 = b.slice<1, 4>();
    auto s3 = distribute(s1, s2);
    EXPECT_EQ(s3, c);
}

TEST(Array, sliceDistribute1) {
    Array<int, 9> a = {3, 1, -4, 2, 10, 12, 13, 14, 15};
    Array<int, 4> b = {3, 7, 11, 13};
    Array<int, 6> c = {-12, -22, 0, 40, 136, 130};
    auto s1 = a.slice<2, 4>();
    auto s2 = distribute(s1, b);
    EXPECT_EQ(s2, c);
}

TEST(Array, sliceDistribute2) {
    Array<int, 9> a = {3, 1, 3, 7, 11, 13, 13, 14, 15};
    Array<int, 3> b = {-4, 2, 10};
    Array<int, 6> c = {-12, -22, 0, 40, 136, 130};
    auto s1 = a.slice<2, 5>();
    auto s2 = distribute(s1, b);
    EXPECT_EQ(s2, c);
}

TEST(Array, distribute1) {
    Array<int, 3> a = {-4, 2, 10};
    Array<int, 4> b = {3, 7, 11, 13};
    Array<int, 6> c = {-12, -22, 0, 40, 136, 130};
    auto s2 = distribute(a, b);
    EXPECT_EQ(s2, c);
}

TEST(Array, distribute2) {
    Array<int, 4> a = {3, 7, 11, 13};
    Array<int, 3> b = {-4, 2, 10};
    Array<int, 6> c = {-12, -22, 0, 40, 136, 130};
    auto s2 = distribute(a, b);
    EXPECT_EQ(s2, c);
}

TEST(Array, reverseSlice) {
    Array<int, 5> a = {1, 2, 3, 4, 5};
    Array<int, 5> b = {5, 4, 3, 2, 1};
    auto s1 = a.slice<4, 0>();
    auto s2 = s1.slice<4, 0>();
    EXPECT_EQ(s1[0], 5);
    EXPECT_EQ(s1[1], 4);
    EXPECT_EQ(s1[2], 3);
    EXPECT_EQ(s1[3], 2);
    EXPECT_EQ(s1[4], 1);
    EXPECT_EQ(s1, b);
    EXPECT_EQ(s2, a);
}

TEST(Array, reverseSliceSlice) {
    Array<int, 5> a = {1, 2, 3, 4, 5};
    auto s1 = a.slice<4, 0>();
    auto s2 = s1.slice<3, 1>();
    auto s3 = a.slice<1, 3>();
    EXPECT_EQ(s2, s3);
}

TEST(Array, reverseSliceIterator) {
    Array<int, 5> a = {1, 2, 3, 4, 5};
    Array<int, 5> b = a.slice<4, 0>();
    auto s1 = b.slice();
    std::reverse(std::begin(s1), std::end(s1));
    EXPECT_EQ(s1, a);
}

TEST(Array, reverseSliceSort) {
    Array<int, 5> a = {1, 5, 4, 3, 2};
    Array<int, 5> b = {5, 4, 3, 2, 1};
    auto s1 = a.slice<4, 0>();               // reverse view of a
    std::sort(std::begin(s1), std::end(s1)); // sort reverse view
    EXPECT_EQ(a, b); // a should be sorted in reverse order
}

TEST(Array, reverseSubSliceSort) {
    Array<int, 7> a = {10, 1, 5, 4, 3, 2, 0};
    Array<int, 7> b = {10, 5, 4, 3, 2, 1, 0};
    auto s1 = a.slice<5, 1>();               // reverse subview of a
    std::sort(std::begin(s1), std::end(s1)); // sort reverse subview
    EXPECT_EQ(a, b); // middle of a should be sorted in reverse order
}

TEST(Array, reverseSliceIteratorSubtract) {
    Array<int, 5> a = {1, 2, 3, 4, 5};
    auto s = a.slice<4, 0>();
    EXPECT_EQ(*(s.end() - 1), 1);
    EXPECT_EQ(*(s.end() - 2), 2);
}

TEST(Array, sliceIteratorSubtract) {
    Array<int, 5> a = {1, 2, 3, 4, 5};
    auto s = a.slice<0, 4>();
    EXPECT_EQ(*(s.end() - 1), 5);
    EXPECT_EQ(*(s.end() - 2), 4);
}