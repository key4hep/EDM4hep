#ifndef EDM4HEP_UTILS_VECTOR_HH
#define EDM4HEP_UTILS_VECTOR_HH

// These ultilies require concepts. If not available, use the fallback
// vector_utils_legacy.h instead to capture most functionality.
#if !__cpp_concepts
#include <edm4hep/utils/vector_utils_legacy.h>
#else
#include <cmath>

#include <edm4hep/Vector3f.h>

namespace edm4hep {

template <class V>
concept VectorHasX = requires(V v) {
  v.x;
};

template <class V>
concept VectorHasY = requires(V v) {
  v.y;
};

template <class V>
concept VectorHasZ = requires(V v) {
  v.z;
};

template <class V>
concept VectorHasA = requires(V v) {
  v.a;
};

template <class V>
concept VectorHasB = requires(V v) {
  v.b;
};

template <class V>
concept ClassVector = requires(V v) {
  v.x();
};

template <class V>
concept Vector2D_XY = VectorHasX<V>&& VectorHasY<V> && !VectorHasZ<V> && !ClassVector<V>;

template <class V>
concept Vector2D_AB = VectorHasA<V>&& VectorHasB<V> && !VectorHasZ<V> && !ClassVector<V>;

template <class V>
concept Vector2D = Vector2D_XY<V> || Vector2D_AB<V>;

template <class V>
concept Vector3D = VectorHasX<V>&& VectorHasY<V>&& VectorHasZ<V> && !ClassVector<V>;

template <class V>
concept VectorND = Vector2D<V> || Vector3D<V>;

template <class V>
concept VectorND_XYZ = Vector2D_XY<V> || Vector3D<V>;

inline double etaToAngle(const double eta) {
  return std::atan(std::exp(-eta)) * 2.;
}

inline double angleToEta(const double theta) {
  return -std::log(std::tan(0.5 * theta));
}

// Utility getters to accomodate different vector types
template <VectorND_XYZ V>
constexpr auto vector_x(const V& v) {
  return v.x;
}

template <VectorND_XYZ V>
constexpr auto vector_y(const V& v) {
  return v.y;
}

template <Vector3D V>
constexpr auto vector_z(const V& v) {
  return v.z;
}

template <Vector2D_AB V>
constexpr auto vector_x(const V& v) {
  return v.a;
}

template <Vector2D_AB V>
constexpr auto vector_y(const V& v) {
  return v.b;
}

// inline edm4hep::Vector2f VectorFromPolar(const double r, const double theta)
// {
//  return {r * sin(theta), r * cos(theta)};
//}

template <Vector3D V = edm4hep::Vector3f>
V sphericalToVector(const double r, const double theta, const double phi) {
  using FloatType = decltype(vector_x(V()));
  const double sth = sin(theta);
  const double cth = cos(theta);
  const double sph = sin(phi);
  const double cph = cos(phi);
  const FloatType x = r * sth * cph;
  const FloatType y = r * sth * sph;
  const FloatType z = r * cth;
  return {x, y, z};
}

template <Vector3D V>
double anglePolar(const V& v) {
  return std::atan2(std::hypot(vector_x(v), vector_y(v)), vector_z(v));
}

template <VectorND V>
double angleAzimuthal(const V& v) {
  return std::atan2(vector_y(v), vector_x(v));
}

template <Vector3D V>
double eta(const V& v) {
  return angleToEta(anglePolar(v));
}

template <Vector2D V>
double magnitude(const V& v) {
  return std::hypot(vector_x(v), vector_y(v));
}

template <Vector3D V>
double magnitude(const V& v) {
  return std::hypot(vector_x(v), vector_y(v), vector_z(v));
}

template <Vector3D V>
double magnitudeTransverse(const V& v) {
  return std::hypot(vector_x(v), vector_y(v));
}

template <Vector3D V>
double magnitudeLongitudinal(const V& v) {
  return vector_z(v);
}

template <VectorND V>
V normalizeVector(const V& v, double norm = 1.) {
  const double old = magnitude(v);
  if (old == 0) {
    return v;
  }
  return (norm > 0) ? v * norm / old : v * 0;
}

template <Vector3D V>
constexpr V vectorTransverse(const V& v) {
  return {vector_x(v), vector_y(v), 0};
}

template <Vector3D V>
constexpr V vectorLongitudinal(const V& v) {
  return {0, 0, vector_z(v)};
}

// Two vector functions
template <VectorND V>
double angleBetween(const V& v1, const V& v2) {
  const double dot = v1 * v2;
  if (dot == 0) {
    return 0.;
  }
  return acos(dot / (magnitude(v1) * magnitude(v2)));
}

// Project v onto v1
template <Vector3D V>
double projection(const V& v, const V& v1) {
  const double norm = magnitude(v1);
  if (norm == 0) {
    return magnitude(v);
  }
  return v * v1 / norm;
}

} // namespace edm4hep

template <edm4hep::Vector2D V>
constexpr V operator+(const V& v1, const V& v2) {
  return {edm4hep::vector_x(v1) + edm4hep::vector_x(v2), edm4hep::vector_y(v1) + edm4hep::vector_y(v2)};
}

template <edm4hep::Vector3D V>
constexpr V operator+(const V& v1, const V& v2) {
  return {edm4hep::vector_x(v1) + edm4hep::vector_x(v2), edm4hep::vector_y(v1) + edm4hep::vector_y(v2),
          edm4hep::vector_z(v1) + edm4hep::vector_z(v2)};
}

template <edm4hep::Vector2D V>
constexpr double operator*(const V& v1, const V& v2) {
  return edm4hep::vector_x(v1) * edm4hep::vector_x(v2) + edm4hep::vector_y(v1) * edm4hep::vector_y(v2);
}

template <edm4hep::Vector3D V>
constexpr double operator*(const V& v1, const V& v2) {
  return edm4hep::vector_x(v1) * edm4hep::vector_x(v2) + edm4hep::vector_y(v1) * edm4hep::vector_y(v2) +
      edm4hep::vector_z(v1) * edm4hep::vector_z(v2);
}

template <edm4hep::Vector2D V>
constexpr V operator*(const double d, const V& v) {
  return {d * edm4hep::vector_x(v), d * edm4hep::vector_y(v)};
}

template <edm4hep::Vector3D V>
constexpr V operator*(const double d, const V& v) {
  return {d * edm4hep::vector_x(v), d * edm4hep::vector_y(v), d * edm4hep::vector_z(v)};
}

template <edm4hep::Vector2D V>
constexpr V operator*(const V& v, const double d) {
  return {d * edm4hep::vector_x(v), d * edm4hep::vector_y(v)};
}

template <edm4hep::Vector3D V>
constexpr V operator*(const V& v, const double d) {
  return {d * edm4hep::vector_x(v), d * edm4hep::vector_y(v), d * edm4hep::vector_z(v)};
}

template <edm4hep::VectorND V>
constexpr V operator-(const V& v1, const V& v2) {
  return v1 + (-1. * v2);
}

template <edm4hep::VectorND V>
constexpr V operator/(const V& v, const double d) {
  return (1. / d) * v;
}

#endif
#endif
