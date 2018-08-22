#pragma once

template <class Node>
class Linkable {
  protected:
    Node *next = nullptr;
    Node *previous = nullptr;

    /**
     * @brief   Append a node to a linked list.
     * 
     * @param   node
     *          A pointer to the node to be appended.
     * @param   first
     *          The pointer to the first node of the linked list.
     * @param   last
     *          The pointer to the last node of the linked list.
     */
    static void append(Node *node, Node *&first, Node *&last) {
        if (first == nullptr)
            first = node;
        node->previous = last;
        if (node->previous != nullptr)
            node->previous->next = node;
        last = node;
        node->next = nullptr;
    }

    /**
     * @brief   Remove a node from a linked list.
     * 
     * @param   node
     *          A pointer to the node to be removed.
     * @param   first
     *          The pointer to the first node of the linked list.
     * @param   last
     *          The pointer to the last node of the linked list.
     */
    static void remove(Node *node, Node *&first, Node *&last) {
        if (node->previous != nullptr)
            node->previous->next = node->next;
        if (node == last)
            last = node->previous;
        if (node->next != nullptr)
            node->next->previous = node->previous;
        if (node == first)
            first = node->next;
        node->previous = nullptr;
        node->next = nullptr;
    }

    /**
     * @brief   Move down the given node in a linked list.
     * 
     * For example: moving down node `C`:
     * ```
     * Before:  ... → A → B → C → D → ...
     * After:   ... → A → C → B → D → ...
     * ```
     * @param   nodeC
     *          A pointer to the node to be moved down.
     * @param   first
     *          The pointer to the first node of the linked list.
     * @param   last
     *          The pointer to the last node of the linked list.
     */
    static void moveDown(Node *nodeC, Node *&first, Node *&last) {
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

    static bool contains(Node *node, Node *&first, Node *&last) {
        return node == first || node->next != nullptr ||
               node->previous != nullptr;
    }
};

/** 
 * @brief   A namespace with linked list utility functions.
 */
namespace LinkedList {

/**
 * @brief   Append a node to a linked list.
 * 
 * @param   node
 *          A pointer to the node to be appended.
 * @param   first
 *          The pointer to the first node of the linked list.
 * @param   last
 *          The pointer to the last node of the linked list.
 */
template <class Node>
void append(Node *node, Node *&first, Node *&last) {
    if (first == nullptr)
        first = node;
    node->previous = last;
    if (node->previous != nullptr)
        node->previous->next = node;
    last = node;
    node->next = nullptr;
}

/**
 * @brief   Remove a node from a linked list.
 * 
 * @param   node
 *          A pointer to the node to be removed.
 * @param   first
 *          The pointer to the first node of the linked list.
 * @param   last
 *          The pointer to the last node of the linked list.
 */
template <class Node>
void remove(Node *node, Node *&first, Node *&last) {
    if (node->previous != nullptr)
        node->previous->next = node->next;
    if (node == last)
        last = node->previous;
    if (node->next != nullptr)
        node->next->previous = node->previous;
    if (node == first)
        first = node->next;
    node->previous = nullptr;
    node->next = nullptr;
}

/**
 * @brief   Move down the given node in a linked list.
 * 
 * For example: moving down node `C`:
 * ```
 * Before:  ... → A → B → C → D → ...
 * After:   ... → A → C → B → D → ...
 * ```
 * @param   nodeC
 *          A pointer to the node to be moved down.
 * @param   first
 *          The pointer to the first node of the linked list.
 * @param   last
 *          The pointer to the last node of the linked list.
 */
template <class Node>
void moveDown(Node *nodeC, Node *&first, Node *&last) {
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

template <class Node>
bool contains(Node *node, Node *&first, __attribute__((unused)) Node *&last) {
    return node == first || node->next != nullptr || node->previous != nullptr;
}

}; // namespace LinkedList