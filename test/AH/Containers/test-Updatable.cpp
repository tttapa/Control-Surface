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
    TestUpdatable::disable(v);
    auto &l = TestUpdatable::getList();
    auto len = std::distance(l.begin(), l.end());
    EXPECT_EQ(len, 0);
    TestUpdatable::enable(v);
    len = std::distance(l.begin(), l.end());
    EXPECT_EQ(len, 16);
}