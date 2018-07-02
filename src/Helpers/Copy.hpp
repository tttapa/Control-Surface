#ifndef COPY_HPP
#define COPY_HPP

template <class T, class S, size_t N> void copy(T *dest, const S (&src)[N]) {
  for (size_t i = 0; i < N; i++)
    dest[i] = (T)src[i];
}

#endif // COPY_HPP