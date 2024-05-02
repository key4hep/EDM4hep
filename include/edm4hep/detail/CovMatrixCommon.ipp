// This file is meant to be included inside the declaration part of the
// ExtraCode for the CovMatrixNx components. They live in this file because they
// can be written very generically and reduce the clutter and code repetition in
// the edm4hep.yaml file
//
// NOTE: All of these functions are intended to be member functions, and the
// only member of a CovMatrixNx component is an appropriately sized std::array
// named values.
//
// NOTE: It is also assumed that the edm4hep/utils/cov_matrix_utils.h header is
// included via the corresponding ExtraCode: include directive

/// Get the i-th element of the underlying storage
///
/// \note The values are stored in a flat array assuming a lower
/// triangular matrix representation
constexpr float operator[](unsigned i) const {
  return values[i];
}

/// Get the i-th element of the underlying storage
///
/// \note The values are stored in a flat array assuming a lower
/// triangular matrix representation
constexpr float& operator[](unsigned i) {
  return values[i];
}

/// Get the begin iterator to the underlying storage
constexpr auto begin() const {
  return values.begin();
}

/// Get the begin iterator to the underlying storage
constexpr auto begin() {
  return values.begin();
}

/// Get the end iterator to the underlying storage
constexpr auto end() const {
  return values.end();
}

/// Get the end iterator to the underlying storage
constexpr auto end() {
  return values.end();
}

/// Get a pointer to the underlying storage data
auto* data() {
  return values.data();
}

/// Get a pointer to the underlying storage data
const auto* data() const {
  return values.data();
}

/// Get the value of the covariance matrix for the passed dimensions
///
/// @tparam DimEnum The enum (class) type that describes the dimensions of this
///                 covariance matrix. This will be deduced from the passed
///                 arguments!
///
/// @param dimI The first dimension for which the covariance matrix value should
///             be obtained
/// @param dimJ The second dimension for which the covariance matrix value
///             should be obtained
///
/// @returns The value of the covariance matrix for dimension dimI and dimJ
template <typename DimEnum>
constexpr float getValue(DimEnum dimI, DimEnum dimJ) const {
  return edm4hep::utils::get_cov_value(values, dimI, dimJ);
}

/// Set the value of the covariance matrix for the passed dimensions
///
/// @tparam DimEnum The enum (class) type that describes the dimensions of this
///                 covariance matrix. This will be deduced from the passed
///                 arguments!
///
/// @param value The value to be set
/// @param dimI  The first dimension for which the covariance matrix value
///              should be obtained
/// @param dimJ  The second dimension for which the covariance matrix value
///              should be obtained
template <typename DimEnum>
constexpr void setValue(float value, DimEnum dimI, DimEnum dimJ) {
  utils::set_cov_value(value, values, dimI, dimJ);
}
