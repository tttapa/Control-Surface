#pragma once

#include <stdlib.h>

template <class Node>
class DoublyLinkedList {
  public:
    DoublyLinkedList() = default;

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

    template <class INode>
    class node_iterator_base {
      public:
        node_iterator_base(INode *node) : node(node) {}

        bool operator!=(const node_iterator_base &rhs) const {
            return node != rhs.node;
        }

        INode &operator*() const {
            // TODO: check node != nullptr
            return *node;
        }

      protected:
        INode *node;
    };

    template <class INode>
    class node_iterator : public node_iterator_base<INode> {
      public:
        node_iterator(INode *node) : node_iterator_base<INode>(node) {}

        /** Prefix increment operator */
        node_iterator &operator++() {
            // TODO: check node != nullptr
            this->node = this->node->next;
            return *this;
        }
    };

    template <class INode>
    class reverse_node_iterator : public node_iterator_base<INode> {
      public:
        reverse_node_iterator(INode *node) : node_iterator_base<INode>(node) {}

        /** Prefix increment operator */
        reverse_node_iterator &operator++() {
            // TODO: check node != nullptr
            this->node = this->node->previous;
            return *this;
        }
    };

    using iterator = node_iterator<Node>;
    using const_iterator = node_iterator<const Node>;
    using reverse_iterator = reverse_node_iterator<Node>;
    using const_reverse_iterator = reverse_node_iterator<const Node>;

    iterator begin() { return {first}; }
    iterator end() { return {nullptr}; }

    const_iterator begin() const { return {first}; }
    const_iterator end() const { return {nullptr}; }

    reverse_iterator rbegin() { return {last}; }
    reverse_iterator rend() { return {nullptr}; }

    const_reverse_iterator rbegin() const { return {last}; }
    const_reverse_iterator rend() const { return {nullptr}; }

  private:
    Node *first = nullptr;
    Node *last = nullptr;
};

template <class Node>
class DoublyLinkable {
  protected:
    friend class DoublyLinkedList<Node>;
    Node *next = nullptr;
    Node *previous = nullptr;
};