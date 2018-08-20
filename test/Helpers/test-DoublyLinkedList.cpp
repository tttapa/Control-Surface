#include <gtest/gtest.h>

#include <Helpers/LinkedList.hpp>
#include <vector>

using std::vector;

struct TestNode : DoublyLinkable<TestNode> {
    TestNode(int value) : value(value) {}
    const int value;
    TestNode *getNext() { return next; }
    TestNode *getPrevious() { return previous; }
};

template <class T>
struct ReversionWrapper {
    T &iterable;
    auto begin() -> decltype(iterable.rbegin()) { return iterable.rbegin(); }
    auto end() -> decltype(iterable.rend()) { return iterable.rend(); }
};

template <class T>
ReversionWrapper<T> reverse(T &&iterable) {
    return {iterable};
}

// -------------------------------------------------------------------------- //

TEST(DoublyLinkedList, iterator) {
    DoublyLinkedList<TestNode> ll;
    TestNode nodes[] = {1, 2, 3, 4, 5};
    for (TestNode &node : nodes)
        ll.append(node);
    vector<int> result = {};
    vector<int> expected = {1, 2, 3, 4, 5};
    for (TestNode &node : ll)
        result.push_back(node.value);
    EXPECT_EQ(result, expected);
}

TEST(DoublyLinkedList, const_iterator) {
    DoublyLinkedList<TestNode> ll;
    TestNode nodes[] = {1, 2, 3, 4, 5};
    for (TestNode &node : nodes)
        ll.append(node);
    vector<int> result = {};
    vector<int> expected = {1, 2, 3, 4, 5};
    const DoublyLinkedList<TestNode> &lli = ll;
    for (const TestNode &node : lli)
        result.push_back(node.value);
    EXPECT_EQ(result, expected);
}

TEST(DoublyLinkedList, reverse_iterator) {
    DoublyLinkedList<TestNode> ll;
    TestNode nodes[] = {1, 2, 3, 4, 5};
    for (TestNode &node : nodes)
        ll.append(node);
    vector<int> result = {};
    vector<int> expected = {5, 4, 3, 2, 1};
    for (TestNode &node : reverse(ll))
        result.push_back(node.value);
    EXPECT_EQ(result, expected);
}

TEST(DoublyLinkedList, const_reverse_iterator) {
    DoublyLinkedList<TestNode> ll;
    TestNode nodes[] = {1, 2, 3, 4, 5};
    for (TestNode &node : nodes)
        ll.append(node);
    vector<int> result = {};
    vector<int> expected = {5, 4, 3, 2, 1};
    const DoublyLinkedList<TestNode> &lli = ll;
    for (const TestNode &node : reverse(lli))
        result.push_back(node.value);
    EXPECT_EQ(result, expected);
}