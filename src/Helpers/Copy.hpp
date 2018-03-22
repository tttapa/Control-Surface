template <class T, size_t N>
void copy(T *dest, const T (&src)[N])
{
    for (size_t i = 0; i < N; i++)
        dest[i] = src[i];
}