#ifndef INSERT_INTO_LINKED_LIST

#define INSERT_INTO_LINKED_LIST(el, first, last) \
    do                                           \
    {                                            \
        if (first == nullptr)                    \
            first = el;                          \
        el->previous = last;                     \
        if (el->previous != nullptr)             \
            el->previous->next = el;             \
        last = el;                               \
        el->next = nullptr;                      \
    } while (0)

#define DELETE_FROM_LINKED_LIST(el, first, last) \
    do                                           \
    {                                            \
        if (el->previous != nullptr)             \
            el->previous->next = el->next;       \
        if (el == last)                          \
            last = el->previous;                 \
        if (el->next != nullptr)                 \
            el->next->previous = el->previous;   \
        if (el == first)                         \
            first = el->next;                    \
    } while (0)

    #endif // INSERT_INTO_LINKED_LIST