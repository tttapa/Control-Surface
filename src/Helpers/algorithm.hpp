#pragma once

#ifndef __AVR__

#include <algorithm>

#else

template <class InputIterator, class UnaryPredicate>
InputIterator find_if(InputIterator first, InputIterator last,
                      UnaryPredicate pred) {
    while (first != last) {
        if (pred(*first))
            return first;
        ++first;
    }
    return last;
}

#endif // __AVR__