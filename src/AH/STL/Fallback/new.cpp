#ifdef __AVR__

#include "new"

#if __cpp_sized_deallocation

void operator delete(void* ptr, std::size_t) _GLIBCXX_USE_NOEXCEPT {
    delete ptr;
}

void operator delete[](void* ptr, std::size_t) _GLIBCXX_USE_NOEXCEPT {
    delete[] ptr;
}

#endif

#endif