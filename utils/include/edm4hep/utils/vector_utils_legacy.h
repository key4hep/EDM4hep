#ifndef EDM4HEP_UTILS_VECTOR_LEGACY_HH
#define EDM4HEP_UTILS_VECTOR_LEGACY_HH

// This is the legacy implementation of vector_utils. If possible, use
// the better vector_utils.h instead (if concepts are available).
#if __cpp_concepts
#include <edm4hep/utils/vector_utils.h>
#else
#include <cmath>

#include <edm4hep/Vector2f.h>
#include <edm4hep/Vector3f.h>

namespace edm4hep {

inline double etaToAngle(const double eta) {
  return std::atan(std::exp(-eta)) * 2.;
}
inline double angleToEta(const double theta) {
  return -std::log(std::tan(0.5 * theta));
}

// Utility getters to accomodate different vector types
template <class V>
auto vector_x(const V& v) {
  return v.x;
}
template <class V>
auto vector_y(const V& v) {
  return v.y;
}
template <class V>
auto vector_z(const V& v) {
  return v.z;
}
// Vector2f uses a,b instead of x,y
template <>
inline auto vector_x<edm4hep::Vector2f>(const edm4hep::Vector2f& v) {
  return v.a;
}
template <>
inline auto vector_y<edm4hep::Vector2f>(const edm4hep::Vector2f& v) {
  return v.b;
}
// no z-component for 2D vectors
template <>
inline auto vector_z<edm4hep::Vector2f>(const edm4hep::Vector2f& v) {
  return 0;
}

// inline edm4hep::Vector2f VectorFromPolar(const double r, const double theta)
// {
//  return {r * sin(theta), r * cos(theta)};
//}
template <class V = edm4hep::Vector3f>
V sphericalToVector(const double r, const double theta, const double phi) {
  using FloatType = decltype(edm4hep::vector_x(V()));
  const double sth = sin(theta);
  const double cth = cos(theta);
  const double sph = sin(phi);
  const double cph = cos(phi);
  const FloatType x = r * sth * cph;
  const FloatType y = r * sth * sph;
  const FloatType z = r * cth;
  return {x, y, z};
}

template <class V>
double anglePolar(const V& v) {
  return std::atan2(std::hypot(edm4hep::vector_x(v), edm4hep::vector_y(v)), edm4hep::vector_z(v));
}
template <class V>
double angleAzimuthal(const V& v) {
  return std::atan2(edm4hep::vector_y(v), edm4hep::vector_x(v));
}
template <class V>
double eta(const V& v) {
  return angleToEta(anglePolar(v));
}
template <class V>
double magnitude(const V& v) {
  return std::hypot(edm4hep::vector_x(v), edm4hep::vector_y(v), edm4hep::vector_z(v));
}
template <class V>
double magnitudeTransverse(const V& v) {
  return std::hypot(edm4hep::vector_x(v), edm4hep::vector_y(v));
}
template <class V>
double magnitudeLongitudinal(const V& v) {
  return edm4hep::vector_z(v);
}
template <class V>
V normalizeVector(const V& v, double norm = 1.) {
  const double old = magnitude(v);
  if (old == 0) {
    return v;
  }
  return (norm > 0) ? v * norm / old : v * 0;
}
template <class V>
V vectorTransverse(const V& v) {
  return {edm4hep::vector_x(v), edm4hep::vector_y(v), 0};
}
template <class V>
V vectorLongitudinal(const V& v) {
  return {0, 0, edm4hep::vector_z(v)};
}
// Two vector functions
template <class V>
double angleBetween(const V& v1, const V& v2) {
  const double dot = v1 * v2;
  if (dot == 0) {
    return 0.;
  }
  return acos(dot / (magnitude(v1) * magnitude(v2)));
}
// Project v onto v1
template <class V>
double projection(const V& v, const V& v1) {
  const double norm = magnitude(v1);
  if (norm == 0) {
    return magnitude(v);
  }
  return v * v1 / norm;
}

} // namespace edm4hep

inline edm4hep::Vector2f operator+(const edm4hep::Vector2f& v1, const edm4hep::Vector2f& v2) {
  using ValueType = decltype(edm4hep::vector_x(edm4hep::Vector2f()));
  const ValueType x = edm4hep::vector_x(v1) + edm4hep::vector_x(v2);
  const ValueType y = edm4hep::vector_y(v1) + edm4hep::vector_y(v2);
  return {x, y};
}
inline edm4hep::Vector3f operator+(const edm4hep::Vector3f& v1, const edm4hep::Vector3f& v2) {
  using ValueType = decltype(edm4hep::vector_x(edm4hep::Vector3f()));
  const ValueType x = edm4hep::vector_x(v1) + edm4hep::vector_x(v2);
  const ValueType y = edm4hep::vector_y(v1) + edm4hep::vector_y(v2);
  const ValueType z = edm4hep::vector_z(v1) + edm4hep::vector_z(v2);
  return {x, y, z};
}
inline double operator*(const edm4hep::Vector2f& v1, const edm4hep::Vector2f& v2) {
  return edm4hep::vector_x(v1) * edm4hep::vector_x(v2) + edm4hep::vector_y(v1) * edm4hep::vector_y(v2);
}
inline double operator*(const edm4hep::Vector3f& v1, const edm4hep::Vector3f& v2) {
  return edm4hep::vector_x(v1) * edm4hep::vector_x(v2) + edm4hep::vector_y(v1) * edm4hep::vector_y(v2) +
      edm4hep::vector_z(v1) * edm4hep::vector_z(v2);
}
inline edm4hep::Vector2f operator*(const double d, const edm4hep::Vector2f& v) {
  using ValueType = decltype(edm4hep::vector_x(edm4hep::Vector2f()));
  const ValueType x = d * edm4hep::vector_x(v);
  const ValueType y = d * edm4hep::vector_y(v);
  return {x, y};
}
inline edm4hep::Vector3f operator*(const double d, const edm4hep::Vector3f& v) {
  using ValueType = decltype(edm4hep::vector_x(edm4hep::Vector3f()));
  const ValueType x = d * edm4hep::vector_x(v);
  const ValueType y = d * edm4hep::vector_y(v);
  const ValueType z = d * edm4hep::vector_z(v);
  return {x, y, z};
}
inline edm4hep::Vector2f operator*(const edm4hep::Vector2f& v, const double d) {
  return d * v;
}
inline edm4hep::Vector3f operator*(const edm4hep::Vector3f& v, const double d) {
  return d * v;
}
inline edm4hep::Vector2f operator-(const edm4hep::Vector2f& v1, const edm4hep::Vector2f& v2) {
  return v1 + (-1. * v2);
}
inline edm4hep::Vector3f operator-(const edm4hep::Vector3f& v1, const edm4hep::Vector3f& v2) {
  return v1 + (-1. * v2);
}
inline edm4hep::Vector2f operator/(const edm4hep::Vector2f& v, const double d) {
  return (1. / d) * v;
}
inline edm4hep::Vector3f operator/(const edm4hep::Vector3f& v, const double d) {
  return (1. / d) * v;
}
#endif
#endif
