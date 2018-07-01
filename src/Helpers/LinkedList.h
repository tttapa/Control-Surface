#ifndef LINKED_LIST_H
#define LINKED_LIST_H

namespace LinkedList
{

template <class Node>
void append(Node *node, Node *&first, Node *&last)
{
    if (first == nullptr)
        first = node;
    node->previous = last;
    if (node->previous != nullptr)
        node->previous->next = node;
    last = node;
    node->next = nullptr;
}

template <class Node>
void remove(Node *node, Node *&first, Node *&last)
{
    if (node->previous != nullptr)
        node->previous->next = node->next;
    if (node == last)
        last = node->previous;
    if (node->next != nullptr)
        node->next->previous = node->previous;
    if (node == first)
        first = node->next;
}

template <class Node>
void moveDown(Node *nodeC, Node *&first, Node *&last)
{
    /*
        Before: ... → A → B → C → D → ...
        After:  ... → A → C → B → D → ...
    */
    Node *nodeB = nodeC->previous;
    if (nodeB == nullptr) // Can't move first node further down
        return;
    Node *nodeA = nodeB->previous;
    Node *nodeD = nodeC->next;

    if (nodeA != nullptr)
        nodeA->next = nodeC;
    else
        first = nodeC;
    nodeB->next = nodeD;
    nodeB->previous = nodeC;
    nodeC->next = nodeB;
    nodeC->previous = nodeA;
    if (nodeD != nullptr)
        nodeD->previous = nodeB;
    else
        last = nodeB;
}
 
};
#endif // LINKED_LIST_H