#pragma once

template <class Node>
class LinkedListForwardIterator;

template <class Node>
class LinkedListReverseIterator;

template <class Node>
class DoublyLinkedList {
  public:
    /**
     * @brief   Append a node to a linked list.
     * 
     * @param   node
     *          A pointer to the node to be appended.
     */
    void append(Node *node) {
        if (first == nullptr)
            first = node;
        node->previous = last;
        if (node->previous != nullptr)
            node->previous->next = node;
        last = node;
        node->next = nullptr;
    }

    void append(Node &node) { append(&node); }

    /**
     * @brief   Remove a node from a linked list.
     * 
     * @param   node
     *          A pointer to the node to be removed.
     */
    void remove(Node *node) {
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

    void remove(Node &node) { remove(&node); }

    /**
     * @brief   Move down the given node in a linked list.
     * 
     * For example: moving down node `C`:
     * ```
     * Before:  ... → A → B → C → D → ...
     * After:   ... → A → C → B → D → ...
     * ```
     * @param   node
     *          A pointer to the node to be moved down.
     */
    void moveDown(Node *node) {
        Node *nodeB = node->previous;
        if (nodeB == nullptr) // Can't move first node further down
            return;
        Node *nodeA = nodeB->previous;
        Node *nodeD = node->next;

        if (nodeA != nullptr)
            nodeA->next = node;
        else
            first = node;
        nodeB->next = nodeD;
        nodeB->previous = node;
        node->next = nodeB;
        node->previous = nodeA;
        if (nodeD != nullptr)
            nodeD->previous = nodeB;
        else
            last = nodeB;
    }

    /** 
     * @brief   Check if the linked list could contain the given node.
     * 
     * @return  true
     *          The given node is part of some linked list or it is the first
     *          node of the given linked list.  
     *          It could be that the node is part of a different linked list
     *          if it was ever added to a different list.
     *          However, **if this function returns true and the node was never
     *          added to another linked list, it means that this linked list 
     *          contains the given node**.
     * @return  false
     *          The given node is not part of any linked list, or it is the 
     *          only element of a different linked list.
     */
    bool couldContain(Node *node) {
        return node && (node == first || node->next != nullptr ||
                        node->previous != nullptr);
    }

    LinkedListForwardIterator<Node> begin() const { return {first}; }
    LinkedListForwardIterator<Node> end() const { return {nullptr}; }

    LinkedListReverseIterator<Node> rbegin() const { return {last}; }
    LinkedListReverseIterator<Node> rend() const { return {nullptr}; }

  private:
    Node *first = nullptr;
    Node *last = nullptr;
};

template <class Node>
class DoublyLinkable {
  protected:
    friend class DoublyLinkedList<Node>;
    friend class LinkedListForwardIterator<Node>;
    friend class LinkedListReverseIterator<Node>;
    Node *next = nullptr;
    Node *previous = nullptr;
};

template <class Node>
class LinkedListIterator {
  public:
    LinkedListIterator(Node *node) : node(node) {}

    bool operator!=(const LinkedListIterator<Node> &rhs) const {
        return node != rhs.node;
    }

    Node &operator*() const {
        // TODO: check node != nullptr
        return *node;
    }

  protected:
    Node *node;
};

template <class Node>
class LinkedListForwardIterator : public LinkedListIterator<Node> {
  public:
    LinkedListForwardIterator(Node *node) : LinkedListIterator<Node>(node) {}

    /** Prefix increment operator */
    LinkedListForwardIterator<Node> &operator++() {
        // TODO: check node != nullptr
        this->node = this->node->next;
        return *this;
    }
};

template <class Node>
class LinkedListReverseIterator : public LinkedListIterator<Node> {
  public:
    LinkedListReverseIterator(Node *node) : LinkedListIterator<Node>(node) {}

    /** Prefix increment operator */
    LinkedListReverseIterator<Node> &operator++() {
        // TODO: check node != nullptr
        this->node = this->node->previous;
        return *this;
    }
};