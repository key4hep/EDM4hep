#ifndef EDM4HEP_DETAIL_ENUM_ACCESS_H
#define EDM4HEP_DETAIL_ENUM_ACCESS_H

#include <algorithm>
#include <array>
#include <cstdint>
#include <type_traits>

namespace edm4hep {

// Use 16 bits to encode the dimension
// Could go to 8 bits, but would need a fix in podio first
using DimType = std::uint16_t;

namespace detail {
// From c++23 this is functionality offerd by the STL
#if __cpp_lib_to_underlying
using to_index = std::to_underlying;
#else
// Otherwise it is simple enough to roll our own
template <typename E> constexpr auto to_index(E e) {
  return static_cast<std::underlying_type_t<E>>(e);
}
#endif

/// Cast an index to an enum value
template <typename DimEnum> constexpr DimEnum to_enum(DimType index) {
  return static_cast<DimEnum>(index);
}

/// Decode which index in the array corresponds to the passed enum value.
/// Assuming that they are unique!
template <typename DimEnum, std::size_t N>
constexpr int decode_index(const std::array<DimType, N> &dimIndices,
                           DimEnum dim) {
  for (auto i = 0u; i < N; ++i) {
    if (dimIndices[i] == detail::to_index(dim)) {
      return i;
    }
  }

  return -1;
}

/// Get the value corresponding to the passed enum value from the array
template <typename DimEnum, typename Scalar, std::size_t N>
constexpr Scalar get_value(const std::array<Scalar, N> &values, DimEnum dim,
                           const std::array<DimType, N> &dimIndices) {
  const auto index = decode_index(dimIndices, dim);
  if (index < 0) {
    // TODO: error handling
  }

  return values[index];
}

template <typename DimEnum, typename Scalar, std::size_t N>
constexpr void set_value(Scalar value, std::array<Scalar, N> &values,
                         DimEnum dim,
                         const std::array<DimType, N> &dimIndices) {
  const auto index = decode_index(dimIndices, dim);
  if (index < 0) {
    // TODO: error handling
  }

  values[index] = value;
}

/// Convert from 2D matrix indices to 1d lower triangle indices
constexpr int to_lower_tri(int i, int j, size_t N) {
  if (j < i) {
    std::swap(i, j);
  }
  return i * (2 * N - i - 1) / 2 + j;
}

/// Get the covariance matrix value corresponding to the passed enum values from
/// the array
template <typename DimEnum, typename Scalar, std::size_t N, std::size_t NDims>
constexpr Scalar get_cov(const std::array<Scalar, N> &cov, DimEnum dimI,
                         DimEnum dimJ,
                         const std::array<DimType, NDims> &dimIndices) {
  auto i = decode_index(dimIndices, dimI);
  auto j = decode_index(dimIndices, dimJ);

  if (i < 0 || j < 0) {
    // TODO: error handling
  }

  return cov[to_lower_tri(i, j, NDims)];
}

template <typename DimEnum, typename Scalar, std::size_t N, std::size_t NDims>
constexpr void set_cov(Scalar value, std::array<Scalar, N> &cov, DimEnum dimI,
                       DimEnum dimJ,
                       const std::array<DimType, NDims> &dimIndices) {
  auto i = decode_index(dimIndices, dimI);
  auto j = decode_index(dimIndices, dimJ);

  if (i < 0 || j < 0) {
    // TODO: error handling
  }

  // Covariance is in lower triangle this brings us from 2D indices to 1D
  cov[to_lower_tri(i, j, NDims)] = value;
}

} // namespace detail

} // namespace edm4hep

#endif // EDM4HEP_DETAIL_ENUM_ACCESS_H
