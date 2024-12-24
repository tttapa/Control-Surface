#include <gtest/gtest.h>

#include <AH/Containers/Updatable.hpp>
#include <random>
#include <thread>
#include <vector>

using std::vector;
using namespace AH;

struct T {};
struct TestUpdatable : Updatable<T> {
    TestUpdatable *getNext() { return dynamic_cast<TestUpdatable *>(next); }
    TestUpdatable *getPrevious() {
        return dynamic_cast<TestUpdatable *>(previous);
    }
    static auto getList() -> decltype(updatables) & { return updatables; }

    void begin() override {}
    void update() override {}
};

TEST(Updatable, enableDisable) {
    TestUpdatable v[16];
    for (auto &vv : v)
        EXPECT_TRUE(vv.isEnabled());
    TestUpdatable::disable(v);
    for (auto &vv : v)
        EXPECT_FALSE(vv.isEnabled());
    auto &l = TestUpdatable::getList();
    auto len = std::distance(l.begin(), l.end());
    EXPECT_EQ(len, 0);
    TestUpdatable::enable(v);
    for (auto &vv : v)
        EXPECT_TRUE(vv.isEnabled());
    len = std::distance(l.begin(), l.end());
    EXPECT_EQ(len, 16);
}

TEST(Updatable, alreadyEnabled) {
    TestUpdatable v[16];
    try {
        v[0].enable();
        FAIL();
    } catch (ErrorException &e) {
        EXPECT_EQ(e.getErrorCode(), 0x1212);
    }
}

TEST(Updatable, alreadyDisabled) {
    TestUpdatable v[16];
    TestUpdatable::disable(v);
    try {
        v[0].disable();
        FAIL();
    } catch (ErrorException &e) {
        EXPECT_EQ(e.getErrorCode(), 0x1213);
    }
}