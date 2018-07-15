#include <gtest/gtest.h>

#include <iostream>
using namespace std;

#include <Helpers/LinkedList.h>
using namespace LinkedList;

struct Node {
    Node(int value) : value(value) {}
    int value;
    Node *previous = nullptr;
    Node *next = nullptr;
};

TEST(LinkedList, insertNode) {
    Node *first = nullptr;
    Node *last = nullptr;
    Node a(1);
    append(&a, first, last);
    EXPECT_EQ(first, &a);
    EXPECT_EQ(last, &a);
    EXPECT_EQ(a.previous, nullptr);
    EXPECT_EQ(a.next, nullptr);
    Node b(2);
    append(&b, first, last);
    EXPECT_EQ(first, &a);
    EXPECT_EQ(last, &b);
    EXPECT_EQ(a.previous, nullptr);
    EXPECT_EQ(a.next, &b);
    EXPECT_EQ(b.previous, &a);
    EXPECT_EQ(b.next, nullptr);
    Node c(3);
    append(&c, first, last);
    EXPECT_EQ(first, &a);
    EXPECT_EQ(last, &c);
    EXPECT_EQ(a.previous, nullptr);
    EXPECT_EQ(a.next, &b);
    EXPECT_EQ(b.previous, &a);
    EXPECT_EQ(b.next, &c);
    EXPECT_EQ(c.previous, &b);
    EXPECT_EQ(c.next, nullptr);
}

TEST(LinkedList, loopForward) {
    Node *first = nullptr;
    Node *last = nullptr;
    Node *nodes[16];
    for (int i = 0; i < 16; i++) {
        nodes[i] = new Node(i);
        append(nodes[i], first, last);
    }

    int i = 0;
    for (Node *el = first; el != nullptr; el = el->next) {
        EXPECT_EQ(el->value, i);
        i++;
    }
    for (Node *&node : nodes) {
        delete node;
    }
}

TEST(LinkedList, loopReversed) {
    Node *first = nullptr;
    Node *last = nullptr;
    Node *nodes[16];
    for (int i = 0; i < 16; i++) {
        nodes[i] = new Node(i);
        append(nodes[i], first, last);
    }
    int i = 15;
    for (Node *el = last; el != nullptr; el = el->previous) {
        EXPECT_EQ(el->value, i);
        i--;
    }
    for (Node *&node : nodes) {
        delete node;
    }
}

TEST(LinkedList, deleteNode) {
    Node *first = nullptr;
    Node *last = nullptr;
    Node *nodes[5];
    for (int i = 0; i < 5; i++) {
        nodes[i] = new Node(i);
        append(nodes[i], first, last);
    }
    remove(nodes[2], first, last);
    EXPECT_EQ(nodes[1]->next, nodes[3]);
    EXPECT_EQ(nodes[3]->previous, nodes[1]);
    for (Node *&node : nodes) {
        delete node;
    }
}

TEST(LinkedList, deleteFirstNode) {
    Node *first = nullptr;
    Node *last = nullptr;
    Node *nodes[5];
    for (int i = 0; i < 5; i++) {
        nodes[i] = new Node(i);
        append(nodes[i], first, last);
    }
    remove(nodes[0], first, last);
    EXPECT_EQ(nodes[1]->previous, nullptr);
    EXPECT_EQ(first, nodes[1]);
    for (Node *&node : nodes)
        delete node;
}

TEST(LinkedList, deleteLastNode) {
    Node *first = nullptr;
    Node *last = nullptr;
    Node *nodes[5];
    for (int i = 0; i < 5; i++) {
        nodes[i] = new Node(i);
        append(nodes[i], first, last);
    }
    remove(nodes[4], first, last);
    EXPECT_EQ(nodes[3]->next, nullptr);
    EXPECT_EQ(last, nodes[3]);
    for (Node *&node : nodes)
        delete node;
}

TEST(LinkedList, moveDownNode) {
    Node *first = nullptr;
    Node *last = nullptr;
    Node *nodes[5];
    for (int i = 0; i < 5; i++) {
        nodes[i] = new Node(i);
        append(nodes[i], first, last);
    }
    moveDown(nodes[3], first, last);
    EXPECT_EQ(nodes[2]->next, nodes[4]);
    EXPECT_EQ(nodes[4]->previous, nodes[2]);
    EXPECT_EQ(nodes[3]->next, nodes[2]);
    EXPECT_EQ(nodes[2]->previous, nodes[3]);
    EXPECT_EQ(nodes[3]->previous, nodes[1]);
    EXPECT_EQ(nodes[1]->next, nodes[3]);
    // for (Node *el = first; el != nullptr; el = el->next)
    //    cout << el->value << endl;
    for (Node *&node : nodes)
        delete node;
}

TEST(LinkedList, moveDownFirstNode) {
    Node *first = nullptr;
    Node *last = nullptr;
    Node *nodes[5];
    for (int i = 0; i < 5; i++) {
        nodes[i] = new Node(i);
        append(nodes[i], first, last);
    }
    moveDown(nodes[0], first, last);
    EXPECT_EQ(first, nodes[0]);
    EXPECT_EQ(nodes[0]->previous, nullptr);
    EXPECT_EQ(nodes[0]->next, nodes[1]);
    EXPECT_EQ(nodes[1]->previous, nodes[0]);
    // for (Node *el = first; el != nullptr; el = el->next)
    //     cout << el->value << endl;
    for (Node *&node : nodes)
        delete node;
}

TEST(LinkedList, moveDownSecondNode) {
    Node *first = nullptr;
    Node *last = nullptr;
    Node *nodes[5];
    for (int i = 0; i < 5; i++) {
        nodes[i] = new Node(i);
        append(nodes[i], first, last);
    }
    moveDown(nodes[1], first, last);
    EXPECT_EQ(first, nodes[1]);
    EXPECT_EQ(nodes[0]->previous, nodes[1]);
    EXPECT_EQ(nodes[0]->next, nodes[2]);
    EXPECT_EQ(nodes[1]->previous, nullptr);
    EXPECT_EQ(nodes[1]->next, nodes[0]);
    // for (Node *el = first; el != nullptr; el = el->next)
    //     cout << el->value << endl;
    for (Node *&node : nodes)
        delete node;
}

TEST(LinkedList, moveDownLastNode) {
    Node *first = nullptr;
    Node *last = nullptr;
    Node *nodes[5];
    for (int i = 0; i < 5; i++) {
        nodes[i] = new Node(i);
        append(nodes[i], first, last);
    }
    moveDown(nodes[4], first, last);
    EXPECT_EQ(last, nodes[3]);
    EXPECT_EQ(nodes[3]->previous, nodes[4]);
    EXPECT_EQ(nodes[3]->next, nullptr);
    EXPECT_EQ(nodes[4]->previous, nodes[2]);
    EXPECT_EQ(nodes[4]->next, nodes[3]);
    // for (Node *el = first; el != nullptr; el = el->next)
    //     cout << el->value << endl;
    for (Node *&node : nodes)
        delete node;
}

TEST(LinkedList, moveDownOnlyElement) {
    Node *first = nullptr;
    Node *last = nullptr;
    Node node(0);
    append(&node, first, last);
    moveDown(&node, first, last);
    EXPECT_EQ(first, &node);
    EXPECT_EQ(last, &node);
    EXPECT_EQ(node.next, nullptr);
    EXPECT_EQ(node.previous, nullptr);
}