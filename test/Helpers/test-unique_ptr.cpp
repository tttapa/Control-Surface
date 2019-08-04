#include <gtest-wrapper.h>

#include <Helpers/unique_ptr.hpp>

#include <iostream>
using std::cout;
using std::endl;

class TestClass {
  public:
    TestClass(int i) : i(i) { ++constructed; }
    ~TestClass() { ++destructed; }

    const int i;

    static void reset() {
        constructed = 0;
        destructed = 0;
    }

    static bool hasLeaks() { return constructed != destructed; }

  private:
    static size_t constructed;
    static size_t destructed;
};
size_t TestClass::constructed = 0;
size_t TestClass::destructed = 0;

TEST(uniquePtr, constructDestruct) {
    TestClass::reset();
    { unique_ptr<TestClass> u(new TestClass(1)); }
    ASSERT_FALSE(TestClass::hasLeaks());
}

TEST(uniquePtr, constructRelease) {
    TestClass::reset();
    unique_ptr<TestClass> u(new TestClass(1));
    delete u.release();
    ASSERT_FALSE(TestClass::hasLeaks());
}

TEST(uniquePtr, constructReset) {
    TestClass::reset();
    unique_ptr<TestClass> u(new TestClass(1));
    u.reset();
    ASSERT_FALSE(TestClass::hasLeaks());
}

TEST(uniquePtr, constructAssign) {
    TestClass::reset();
    unique_ptr<TestClass> u(new TestClass(1));
    u = make_unique<TestClass>(2);
    ASSERT_EQ(u->i, 2);
    u.reset();
    ASSERT_FALSE(TestClass::hasLeaks());
}

TEST(uniquePtr, constructAssignNew) {
    TestClass::reset();
    unique_ptr<TestClass> u(new TestClass(1));
    u = unique_ptr<TestClass>(new TestClass(2));
    ASSERT_EQ(u->i, 2);
    u.reset();
    ASSERT_FALSE(TestClass::hasLeaks());
}