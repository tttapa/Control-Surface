#include <gtest/gtest.h>

#include <AH/Containers/LinkedList.hpp>
#include <vector>

using std::vector;

struct TestNode : DoublyLinkable<TestNode> {
    TestNode(int value) : value(value) {}
    const int value;
    TestNode *getNext() { return next; }
    TestNode *getPrevious() { return previous; }
    bool operator>(const TestNode &rhs) const {
        return this->value > rhs.value;
    }
    operator int() const { return value; }
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

// --------------------------------- Lists ---------------------------------- //

TEST(DoublyLinkedList, appendNode) {
    DoublyLinkedList<TestNode> list;
    TestNode a(1);
    list.append(a);
    EXPECT_EQ(list.getFirst(), &a);
    EXPECT_EQ(list.getLast(), &a);
    EXPECT_EQ(a.getPrevious(), nullptr);
    EXPECT_EQ(a.getNext(), nullptr);
    EXPECT_EQ(list.getFirst()->value, 1);
    TestNode b(2);
    list.append(b);
    EXPECT_EQ(list.getFirst(), &a);
    EXPECT_EQ(list.getLast(), &b);
    EXPECT_EQ(a.getPrevious(), nullptr);
    EXPECT_EQ(a.getNext(), &b);
    EXPECT_EQ(b.getPrevious(), &a);
    EXPECT_EQ(b.getNext(), nullptr);
    TestNode c(3);
    list.append(c);
    EXPECT_EQ(list.getFirst(), &a);
    EXPECT_EQ(list.getLast(), &c);
    EXPECT_EQ(a.getPrevious(), nullptr);
    EXPECT_EQ(a.getNext(), &b);
    EXPECT_EQ(b.getPrevious(), &a);
    EXPECT_EQ(b.getNext(), &c);
    EXPECT_EQ(c.getPrevious(), &b);
    EXPECT_EQ(c.getNext(), nullptr);
}

TEST(DoublyLinkedList, insertNodeBeforeFirst) {
    DoublyLinkedList<TestNode> list;
    TestNode a(1);
    list.append(a);
    EXPECT_EQ(list.getFirst(), &a);
    EXPECT_EQ(list.getLast(), &a);
    EXPECT_EQ(a.getPrevious(), nullptr);
    EXPECT_EQ(a.getNext(), nullptr);
    TestNode b(2);
    list.append(b);
    EXPECT_EQ(list.getFirst(), &a);
    EXPECT_EQ(list.getLast(), &b);
    EXPECT_EQ(a.getPrevious(), nullptr);
    EXPECT_EQ(a.getNext(), &b);
    EXPECT_EQ(b.getPrevious(), &a);
    EXPECT_EQ(b.getNext(), nullptr);
    TestNode c(3);
    list.insertBefore(c, a);
    EXPECT_EQ(list.getFirst(), &c);
    EXPECT_EQ(list.getLast(), &b);
    EXPECT_EQ(a.getPrevious(), &c);
    EXPECT_EQ(a.getNext(), &b);
    EXPECT_EQ(b.getPrevious(), &a);
    EXPECT_EQ(b.getNext(), nullptr);
    EXPECT_EQ(c.getPrevious(), nullptr);
    EXPECT_EQ(c.getNext(), &a);
}

TEST(DoublyLinkedList, insertNodeBeforeMiddle) {
    DoublyLinkedList<TestNode> list;
    TestNode a(1);
    list.append(a);
    EXPECT_EQ(list.getFirst(), &a);
    EXPECT_EQ(list.getLast(), &a);
    EXPECT_EQ(a.getPrevious(), nullptr);
    EXPECT_EQ(a.getNext(), nullptr);
    TestNode b(2);
    list.append(b);
    EXPECT_EQ(list.getFirst(), &a);
    EXPECT_EQ(list.getLast(), &b);
    EXPECT_EQ(a.getPrevious(), nullptr);
    EXPECT_EQ(a.getNext(), &b);
    EXPECT_EQ(b.getPrevious(), &a);
    EXPECT_EQ(b.getNext(), nullptr);
    TestNode c(3);
    list.insertBefore(c, b);
    EXPECT_EQ(list.getFirst(), &a);
    EXPECT_EQ(list.getLast(), &b);
    EXPECT_EQ(a.getPrevious(), nullptr);
    EXPECT_EQ(a.getNext(), &c);
    EXPECT_EQ(b.getPrevious(), &c);
    EXPECT_EQ(b.getNext(), nullptr);
    EXPECT_EQ(c.getPrevious(), &a);
    EXPECT_EQ(c.getNext(), &b);
}

TEST(DoublyLinkedList, insertSorted) {
    DoublyLinkedList<TestNode> list;
    TestNode nodes[] = {5, 9, 6, 2, 8, 1, 3, 4, 7};
    for (auto &node : nodes)
        list.insertSorted(&node);
    vector<int> expected = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    vector<int> result(expected.size());
    std::copy(list.begin(), list.end(), result.begin());
    EXPECT_EQ(result, expected);
}

TEST(DoublyLinkedList, insertSortedDoubles) {
    DoublyLinkedList<TestNode> list;
    TestNode nodes[] = {2, 5, 6, 9, 2, 2, 8, 1, 3, 2, 4, 7, 2};
    for (auto &node : nodes)
        list.insertSorted(&node);
    vector<int> expected = {1, 2, 2, 2, 2, 2, 3, 4, 5, 6, 7, 8, 9};
    vector<int> result(expected.size());
    std::copy(list.begin(), list.end(), result.begin());
    EXPECT_EQ(result, expected);
}

TEST(DoublyLinkedList, insertSortedGreaterThan) {
    DoublyLinkedList<TestNode> list;
    TestNode nodes[] = {2, 5, 6, 9, 2, 2, 8, 1, 3, 2, 4, 7, 2};
    for (auto &node : nodes)
        list.insertSorted(
            &node, [](TestNode &lhs, TestNode &rhs) { return lhs > rhs; });
    vector<int> expected = {9, 8, 7, 6, 5, 4, 3, 2, 2, 2, 2, 2, 1};
    vector<int> result(expected.size());
    std::copy(list.begin(), list.end(), result.begin());
    EXPECT_EQ(result, expected);
}

TEST(DoublyLinkedList, deleteNode) {
    DoublyLinkedList<TestNode> list;

    TestNode *nodes[5];
    for (int i = 0; i < 5; i++) {
        nodes[i] = new TestNode(i);
        list.append(nodes[i]);
    }
    list.remove(nodes[2]);
    EXPECT_EQ(nodes[1]->getNext(), nodes[3]);
    EXPECT_EQ(nodes[3]->getPrevious(), nodes[1]);
    for (TestNode *&node : nodes) {
        delete node;
    }
}

TEST(DoublyLinkedList, deleteFirstNode) {
    DoublyLinkedList<TestNode> list;

    TestNode *nodes[5];
    for (int i = 0; i < 5; i++) {
        nodes[i] = new TestNode(i);
        list.append(nodes[i]);
    }
    list.remove(nodes[0]);
    EXPECT_EQ(nodes[1]->getPrevious(), nullptr);
    EXPECT_EQ(list.getFirst(), nodes[1]);
    for (TestNode *&node : nodes)
        delete node;
}

TEST(DoublyLinkedList, deleteLastNode) {
    DoublyLinkedList<TestNode> list;

    TestNode *nodes[5];
    for (int i = 0; i < 5; i++) {
        nodes[i] = new TestNode(i);
        list.append(nodes[i]);
    }
    list.remove(nodes[4]);
    EXPECT_EQ(nodes[3]->getNext(), nullptr);
    EXPECT_EQ(list.getLast(), nodes[3]);
    for (TestNode *&node : nodes)
        delete node;
}

TEST(DoublyLinkedList, moveDownNode) {
    DoublyLinkedList<TestNode> list;

    TestNode *nodes[5];
    for (int i = 0; i < 5; i++) {
        nodes[i] = new TestNode(i);
        list.append(nodes[i]);
    }
    list.moveDown(nodes[3]);
    EXPECT_EQ(nodes[2]->getNext(), nodes[4]);
    EXPECT_EQ(nodes[4]->getPrevious(), nodes[2]);
    EXPECT_EQ(nodes[3]->getNext(), nodes[2]);
    EXPECT_EQ(nodes[2]->getPrevious(), nodes[3]);
    EXPECT_EQ(nodes[3]->getPrevious(), nodes[1]);
    EXPECT_EQ(nodes[1]->getNext(), nodes[3]);
    for (TestNode *&node : nodes)
        delete node;
}

TEST(DoublyLinkedList, moveDownFirstNode) {
    DoublyLinkedList<TestNode> list;

    TestNode *nodes[5];
    for (int i = 0; i < 5; i++) {
        nodes[i] = new TestNode(i);
        list.append(nodes[i]);
    }
    list.moveDown(nodes[0]);
    EXPECT_EQ(list.getFirst(), nodes[0]);
    EXPECT_EQ(nodes[0]->getPrevious(), nullptr);
    EXPECT_EQ(nodes[0]->getNext(), nodes[1]);
    EXPECT_EQ(nodes[1]->getPrevious(), nodes[0]);
    for (TestNode *&node : nodes)
        delete node;
}

TEST(DoublyLinkedList, moveDownSecondNode) {
    DoublyLinkedList<TestNode> list;

    TestNode *nodes[5];
    for (int i = 0; i < 5; i++) {
        nodes[i] = new TestNode(i);
        list.append(nodes[i]);
    }
    list.moveDown(nodes[1]);
    EXPECT_EQ(list.getFirst(), nodes[1]);
    EXPECT_EQ(nodes[0]->getPrevious(), nodes[1]);
    EXPECT_EQ(nodes[0]->getNext(), nodes[2]);
    EXPECT_EQ(nodes[1]->getPrevious(), nullptr);
    EXPECT_EQ(nodes[1]->getNext(), nodes[0]);
    for (TestNode *&node : nodes)
        delete node;
}

TEST(DoublyLinkedList, moveDownLastNode) {
    DoublyLinkedList<TestNode> list;

    TestNode *nodes[5];
    for (int i = 0; i < 5; i++) {
        nodes[i] = new TestNode(i);
        list.append(nodes[i]);
    }
    list.moveDown(nodes[4]);
    EXPECT_EQ(list.getLast(), nodes[3]);
    EXPECT_EQ(nodes[3]->getPrevious(), nodes[4]);
    EXPECT_EQ(nodes[3]->getNext(), nullptr);
    EXPECT_EQ(nodes[4]->getPrevious(), nodes[2]);
    EXPECT_EQ(nodes[4]->getNext(), nodes[3]);
    for (TestNode *&node : nodes)
        delete node;
}

TEST(DoublyLinkedList, moveDownOnlyElement) {
    DoublyLinkedList<TestNode> list;

    TestNode node(0);
    list.append(&node);
    list.moveDown(&node);
    EXPECT_EQ(list.getFirst(), &node);
    EXPECT_EQ(list.getLast(), &node);
    EXPECT_EQ(node.getNext(), nullptr);
    EXPECT_EQ(node.getPrevious(), nullptr);
}

TEST(DoublyLinkedList, couldContainInsert) {
    DoublyLinkedList<TestNode> list;

    TestNode a(1);
    EXPECT_FALSE(list.couldContain(&a));
    list.append(a);
    EXPECT_TRUE(list.couldContain(&a));
    TestNode b(2);
    EXPECT_FALSE(list.couldContain(&b));
    list.append(&b);
    EXPECT_TRUE(list.couldContain(&a));
    EXPECT_TRUE(list.couldContain(&b));
    TestNode c(3);
    EXPECT_FALSE(list.couldContain(&c));
    list.append(&c);
    EXPECT_TRUE(list.couldContain(&a));
    EXPECT_TRUE(list.couldContain(&b));
    EXPECT_TRUE(list.couldContain(&c));
}

TEST(DoublyLinkedList, couldContainDeleteFirst) {
    DoublyLinkedList<TestNode> list;

    TestNode a(1);
    list.append(&a);
    TestNode b(2);
    list.append(&b);
    TestNode c(3);
    list.append(&c);

    list.remove(&a);
    EXPECT_FALSE(list.couldContain(&a));

    list.append(&a);
    EXPECT_TRUE(list.couldContain(&a));
}

TEST(DoublyLinkedList, couldContainDeleteMiddle) {
    DoublyLinkedList<TestNode> list;

    TestNode a(1);
    list.append(&a);
    TestNode b(2);
    list.append(&b);
    TestNode c(3);
    list.append(&c);

    list.remove(&b);
    EXPECT_FALSE(list.couldContain(&b));

    list.append(&b);
    EXPECT_TRUE(list.couldContain(&b));
}

TEST(DoublyLinkedList, couldContainDeleteLast) {
    DoublyLinkedList<TestNode> list;

    TestNode a(1);
    list.append(&a);
    TestNode b(2);
    list.append(&b);
    TestNode c(3);
    list.append(&c);

    list.remove(&c);
    EXPECT_FALSE(list.couldContain(&c));

    list.append(&c);
    EXPECT_TRUE(list.couldContain(&c));
}

// -------------------------------- Iterator -------------------------------- //

TEST(DoublyLinkedList, iterator) {
    DoublyLinkedList<TestNode> ll;
    TestNode nodes[] = {1, 2, 3, 4, 5};
    for (TestNode &node : nodes)
        ll.append(node);
    vector<int> expected = {1, 2, 3, 4, 5};
    vector<int> result(expected.size());
    std::copy(ll.begin(), ll.end(), result.begin());
    EXPECT_EQ(result, expected);
}

TEST(DoublyLinkedList, const_iterator) {
    DoublyLinkedList<TestNode> ll;
    TestNode nodes[] = {1, 2, 3, 4, 5};
    for (TestNode &node : nodes)
        ll.append(node);
    vector<int> expected = {1, 2, 3, 4, 5};
    const DoublyLinkedList<TestNode> &lli = ll;
    vector<int> result(expected.size());
    std::copy(lli.begin(), lli.end(), result.begin());
    EXPECT_EQ(result, expected);
}

TEST(DoublyLinkedList, reverse_iterator) {
    DoublyLinkedList<TestNode> ll;
    TestNode nodes[] = {1, 2, 3, 4, 5};
    for (TestNode &node : nodes)
        ll.append(node);
    vector<int> expected = {5, 4, 3, 2, 1};
    vector<int> result(expected.size());
    std::copy(ll.rbegin(), ll.rend(), result.begin());
    EXPECT_EQ(result, expected);
}

TEST(DoublyLinkedList, const_reverse_iterator) {
    DoublyLinkedList<TestNode> ll;
    TestNode nodes[] = {1, 2, 3, 4, 5};
    for (TestNode &node : nodes)
        ll.append(node);
    vector<int> expected = {5, 4, 3, 2, 1};
    const DoublyLinkedList<TestNode> &lli = ll;
    vector<int> result(expected.size());
    std::copy(lli.rbegin(), lli.rend(), result.begin());
    EXPECT_EQ(result, expected);
}