// g++ -Wall -std=c++11 -o tests -I/home/pieter/lib/Catch2/single_include *.cpp && ./tests

#if !defined(ARDUINO) && !defined(GOOGLETEST)

#include <catch.hpp>
using Catch::Matchers::Equals;

#include <iostream>
using namespace std;

#include "../LinkedList.h"
using namespace LinkedList;

struct Node {
    Node(int value) : value(value) {}
    int value;
    Node *previous = nullptr;
    Node *next = nullptr;
};

TEST_CASE("Insert some elements", "[INSERT]") {
    Node *first = nullptr;
    Node *last = nullptr;
    Node a(1);
    append(&a, first, last);
    REQUIRE(first == &a);
    REQUIRE(last == &a);
    REQUIRE(a.previous == nullptr);
    REQUIRE(a.next == nullptr);
    Node b(2);
    append(&b, first, last);
    REQUIRE(first == &a);
    REQUIRE(last == &b);
    REQUIRE(a.previous == nullptr);
    REQUIRE(a.next == &b);
    REQUIRE(b.previous == &a);
    REQUIRE(b.next == nullptr);
    Node c(3);
    append(&c, first, last);
    REQUIRE(first == &a);
    REQUIRE(last == &c);
    REQUIRE(a.previous == nullptr);
    REQUIRE(a.next == &b);
    REQUIRE(b.previous == &a);
    REQUIRE(b.next == &c);
    REQUIRE(c.previous == &b);
    REQUIRE(c.next == nullptr);
}

TEST_CASE("Insert some elements and read them back forwards",
          "[INSERT][LOOP]") {
    Node *first = nullptr;
    Node *last = nullptr;
    Node *nodes[16];
    for (int i = 0; i < 16; i++) {
        nodes[i] = new Node(i);
        append(nodes[i], first, last);
    }

    int i = 0;
    for (Node *el = first; el != nullptr; el = el->next) {
        REQUIRE(el->value == i);
        i++;
    }
    for (Node *&node : nodes) {
        delete node;
    }
}

TEST_CASE("Insert some elements and read them back backwards",
          "[INSERT][LOOP]") {
    Node *first = nullptr;
    Node *last = nullptr;
    Node *nodes[16];
    for (int i = 0; i < 16; i++) {
        nodes[i] = new Node(i);
        append(nodes[i], first, last);
    }
    int i = 15;
    for (Node *el = last; el != nullptr; el = el->previous) {
        REQUIRE(el->value == i);
        i--;
    }
    for (Node *&node : nodes) {
        delete node;
    }
}

TEST_CASE("Delete an element from the list", "[DELETE]") {
    Node *first = nullptr;
    Node *last = nullptr;
    Node *nodes[5];
    for (int i = 0; i < 5; i++) {
        nodes[i] = new Node(i);
        append(nodes[i], first, last);
    }
    remove(nodes[2], first, last);
    REQUIRE(nodes[1]->next == nodes[3]);
    REQUIRE(nodes[3]->previous == nodes[1]);
    for (Node *&node : nodes) {
        delete node;
    }
}

TEST_CASE("Delete the first element from the list", "[DELETE]") {
    Node *first = nullptr;
    Node *last = nullptr;
    Node *nodes[5];
    for (int i = 0; i < 5; i++) {
        nodes[i] = new Node(i);
        append(nodes[i], first, last);
    }
    remove(nodes[0], first, last);
    REQUIRE(nodes[1]->previous == nullptr);
    REQUIRE(first == nodes[1]);
    for (Node *&node : nodes)
        delete node;
}

TEST_CASE("Delete the last element from the list", "[DELETE]") {
    Node *first = nullptr;
    Node *last = nullptr;
    Node *nodes[5];
    for (int i = 0; i < 5; i++) {
        nodes[i] = new Node(i);
        append(nodes[i], first, last);
    }
    remove(nodes[4], first, last);
    REQUIRE(nodes[3]->next == nullptr);
    REQUIRE(last == nodes[3]);
    for (Node *&node : nodes)
        delete node;
}

TEST_CASE("Move down an element", "[MOVE]") {
    Node *first = nullptr;
    Node *last = nullptr;
    Node *nodes[5];
    for (int i = 0; i < 5; i++) {
        nodes[i] = new Node(i);
        append(nodes[i], first, last);
    }
    moveDown(nodes[3], first, last);
    REQUIRE(nodes[2]->next == nodes[4]);
    REQUIRE(nodes[4]->previous == nodes[2]);
    REQUIRE(nodes[3]->next == nodes[2]);
    REQUIRE(nodes[2]->previous == nodes[3]);
    REQUIRE(nodes[3]->previous == nodes[1]);
    REQUIRE(nodes[1]->next == nodes[3]);
    // for (Node *el = first; el != nullptr; el = el->next)
    //    cout << el->value << endl;
    for (Node *&node : nodes)
        delete node;
}

TEST_CASE("Move down the first element", "[MOVE]") {
    Node *first = nullptr;
    Node *last = nullptr;
    Node *nodes[5];
    for (int i = 0; i < 5; i++) {
        nodes[i] = new Node(i);
        append(nodes[i], first, last);
    }
    moveDown(nodes[0], first, last);
    REQUIRE(first == nodes[0]);
    REQUIRE(nodes[0]->previous == nullptr);
    REQUIRE(nodes[0]->next == nodes[1]);
    REQUIRE(nodes[1]->previous == nodes[0]);
    // for (Node *el = first; el != nullptr; el = el->next)
    //     cout << el->value << endl;
    for (Node *&node : nodes)
        delete node;
}

TEST_CASE("Move down the second element", "[MOVE]") {
    Node *first = nullptr;
    Node *last = nullptr;
    Node *nodes[5];
    for (int i = 0; i < 5; i++) {
        nodes[i] = new Node(i);
        append(nodes[i], first, last);
    }
    moveDown(nodes[1], first, last);
    REQUIRE(first == nodes[1]);
    REQUIRE(nodes[0]->previous == nodes[1]);
    REQUIRE(nodes[0]->next == nodes[2]);
    REQUIRE(nodes[1]->previous == nullptr);
    REQUIRE(nodes[1]->next == nodes[0]);
    // for (Node *el = first; el != nullptr; el = el->next)
    //     cout << el->value << endl;
    for (Node *&node : nodes)
        delete node;
}

TEST_CASE("Move down the last element", "[MOVE]") {
    Node *first = nullptr;
    Node *last = nullptr;
    Node *nodes[5];
    for (int i = 0; i < 5; i++) {
        nodes[i] = new Node(i);
        append(nodes[i], first, last);
    }
    moveDown(nodes[4], first, last);
    REQUIRE(last == nodes[3]);
    REQUIRE(nodes[3]->previous == nodes[4]);
    REQUIRE(nodes[3]->next == nullptr);
    REQUIRE(nodes[4]->previous == nodes[2]);
    REQUIRE(nodes[4]->next == nodes[3]);
    // for (Node *el = first; el != nullptr; el = el->next)
    //     cout << el->value << endl;
    for (Node *&node : nodes)
        delete node;
}

TEST_CASE("Move down the only element", "[MOVE]") {
    Node *first = nullptr;
    Node *last = nullptr;
    Node node(0);
    append(&node, first, last);
    moveDown(&node, first, last);
    REQUIRE(first == &node);
    REQUIRE(last == &node);
    REQUIRE(node.next == nullptr);
    REQUIRE(node.previous == nullptr);
}

#endif // ifndef ARDUINO