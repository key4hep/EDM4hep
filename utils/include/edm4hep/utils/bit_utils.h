#ifndef EDM4HEP_UTILS_BIT_UTILS_HH
#define EDM4HEP_UTILS_BIT_UTILS_HH

namespace edm4hep::utils {
template <typename T>
constexpr T setBit(T bits, int bitNum, bool value) {
  return (bits & ~(1 << bitNum)) | (value << bitNum);
}

template <typename T>
constexpr bool checkBit(T bits, int bitNum) {
  return bits & (0x1 << bitNum);
}
} // namespace edm4hep::utils

#endif // EDM4HEP_UTILS_BIT_UTILS_HH
