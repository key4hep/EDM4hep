#ifndef EDM4HEP_UTILS_BIT_UTILS_HH
#define EDM4HEP_UTILS_BIT_UTILS_HH

#include <type_traits>

namespace edm4hep::utils {

/// Set a bit in the passed bitfield to the desired value
///
/// @tparam T any integer type that can be used as a bitfield, typically an
///           unsigned integer type
///
/// @param bitfield The bitfield for which the bit should be set
/// @param bit      The bit (number) that should be set
/// @param value    The value to which the bit should be set
///
/// @returns The new value of the bitfield after setting bits
template <typename T>
constexpr T setBit(T bitfield, int bit, bool value) {
  return (bitfield & ~(T(1) << bit)) | (T(value) << bit);
}

/// Set multiple bits to one desired value in the passed bitfield
///
/// @tparam T any integer type that can be used as a bitfield, typically an
///           unsigned integer type
/// @tparam Bits A variable number of bits (numbers) that should be set
///
/// @param bitfield The bitfield for which the bit should be set
/// @param value    The value to which the bit should be set
/// @param bits     The bits that should be set
template <typename T, typename... Bits>
constexpr T setBits(T bitfield, bool value, Bits... bits) {
  static_assert((std::is_same_v<Bits, int> && ...), "All bit numbers to set must be integers");
  static_assert(sizeof...(bits) > 0, "Need at least one bit to set");

  for (auto n : {bits...}) {
    bitfield = setBit(bitfield, n, value);
  }
  return bitfield;
}

/// Check if a bit is set in the bitfield
///
/// @tparam T any integer type that can be used as a bitfield, typically an
///           unsigned integer type
///
/// @param bitfield The bitfield that should be checked
/// @param bit      The bit (number) that should be checked
///
/// @returns true if the passed bit is set in the bitfield and false otherwise
template <typename T>
constexpr bool checkBit(T bitfield, int bit) {
  return bitfield & (T(1) << bit);
}

/// Check if all the passed bits are set in the bitfield
///
/// @tparam T any integer type that can be used as a bitfield, typically an
///           unsigned integer type
/// @tparam Bits A variable number of bits (numbers) that should be checked
///
/// @param bitfield The bitfield that should be checked
/// @param bits     The bits that should be checked
///
/// @returns true if all the passed bits are set in the bitfield and false
///          otherwise
template <typename T, typename... Bits>
constexpr bool checkAllBits(T bitfield, Bits... bits) {
  static_assert((std::is_same_v<Bits, int> && ...), "All bit numbers to set must be integers");
  static_assert(sizeof...(bits) > 0, "Need at least one bit to check");
  return (true && ... && checkBit(bitfield, bits));
}

/// Check if any of the passed bits is set in the bitfield
///
/// @tparam T any integer type that can be used as a bitfield, typically an
///           unsigned integer type
/// @tparam Bits A variable number of bits (numbers) that should be checked
///
/// @param bitfield The bitfield that should be checked
/// @param bits     The bits that should be checked
///
/// @returns true if any of the passed bits is set in the bitfield and false
///          otherwise
template <typename T, typename... Bits>
constexpr bool checkAnyBits(T bitfield, Bits... bits) {
  static_assert((std::is_same_v<Bits, int> && ...), "All bit numbers to set must be integers");
  static_assert(sizeof...(bits) > 0, "Need at least one bit to check");
  return (false || ... || checkBit(bitfield, bits));
}

} // namespace edm4hep::utils

#endif // EDM4HEP_UTILS_BIT_UTILS_HH
