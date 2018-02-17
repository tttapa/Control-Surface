#ifndef IS_INTEGRAL_H_
#define IS_INTEGRAL_H_

template<class T> constexpr bool is_integral_type() {
  return false;
}
template<> constexpr bool is_integral_type<uint8_t>() {
  return true;
}
template<> constexpr bool is_integral_type<uint16_t>() {
  return true;
}
template<> constexpr bool is_integral_type<uint32_t>() {
  return true;
}
template<> constexpr bool is_integral_type<uint64_t>() {
  return true;
}
template<> constexpr bool is_integral_type<int8_t>() {
  return true;
}
template<> constexpr bool is_integral_type<int16_t>() {
  return true;
}
template<> constexpr bool is_integral_type<int32_t>() {
  return true;
}
template<> constexpr bool is_integral_type<int64_t>() {
  return true;
}

#endif // IS_INTEGRAL_H_