#include <gtest-wrapper.h>

#include <AH/Containers/Updatable.hpp>
#include <random>
#include <thread>
#include <vector>

using std::vector;
using namespace AH;

struct T {};
struct TestUpdatable : Updatable<T, true> {
    TestUpdatable *getNext() { return dynamic_cast<TestUpdatable *>(next); }
    TestUpdatable *getPrevious() {
        return dynamic_cast<TestUpdatable *>(previous);
    }
    static auto getList() -> decltype(updatables) & { return updatables; }

    void begin() override {}
    void update() override {}
};

TEST(Updatable, threadSafety) {
    vector<TestUpdatable> v(16);
    // Select a random element of the list, move it down one place in the list,
    // and repeat many times to try causing a data race.
    auto fun = [&v] {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> distrib(0, 15);
        for (size_t i = 0; i < 65536 * 2; ++i) {
            size_t idx = distrib(gen);
            v[idx].moveDown();
        }
    };

    std::thread ts[] = {
        std::thread(fun), std::thread(fun), std::thread(fun), std::thread(fun),
        std::thread(fun), std::thread(fun), std::thread(fun), std::thread(fun),
    };
    for (auto &t : ts)
        t.join();

    auto &l = TestUpdatable::getList();
    // Walk through the list in both directions, counting the elements to make
    // sure the list wasn't corrupted by data races:
    auto dist_fwd = std::distance(l.begin(), l.end());
    auto dist_bwd = std::distance(l.rbegin(), l.rend());
    EXPECT_EQ(dist_fwd, v.size());
    EXPECT_EQ(dist_bwd, v.size());
}

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