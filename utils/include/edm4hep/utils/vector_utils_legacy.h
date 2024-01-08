#ifndef EDM4HEP_UTILS_VECTOR_LEGACY_HH
#define EDM4HEP_UTILS_VECTOR_LEGACY_HH

// This is the legacy implementation of vector_utils. If possible, use
// the better vector_utils.h instead (if concepts are available).
#if __cpp_concepts
#include <edm4hep/utils/vector_utils.h>
#else

#include <edm4hep/Vector2f.h>
#include <edm4hep/Vector2i.h>
#include <edm4hep/Vector3d.h>
#include <edm4hep/Vector3f.h>
#include <edm4hep/Vector4f.h>

#include <cmath>
#include <tuple>
#include <type_traits>

namespace edm4hep {

namespace utils {

  inline double etaToAngle(const double eta) {
    return std::atan(std::exp(-eta)) * 2.;
  }

  inline double angleToEta(const double theta) {
    return -std::log(std::tan(0.5 * theta));
  }

  // Utility getters to accomodate different vector types
  template <class V>
  constexpr auto vector_x(const V& v) {
    return v.x;
  }

  template <class V>
  constexpr auto vector_y(const V& v) {
    return v.y;
  }

  template <class V>
  constexpr auto vector_z(const V& v) {
    return v.z;
  }

  template <class V>
  constexpr auto vector_t(const V& v) {
    return v.t;
  }

  // 2D vector uses a,b instead of x,y
  template <>
  inline constexpr auto vector_x<edm4hep::Vector2i>(const edm4hep::Vector2i& v) {
    return v.a;
  }

  template <>
  inline constexpr auto vector_y<edm4hep::Vector2i>(const edm4hep::Vector2i& v) {
    return v.b;
  }

  template <>
  inline constexpr auto vector_x<edm4hep::Vector2f>(const edm4hep::Vector2f& v) {
    return v.a;
  }

  template <>
  inline constexpr auto vector_y<edm4hep::Vector2f>(const edm4hep::Vector2f& v) {
    return v.b;
  }
  // no z-component for 2D vectors
  template <>
  inline constexpr auto vector_z<edm4hep::Vector2f>(const edm4hep::Vector2f&) {
    return 0;
  }

  template <>
  inline constexpr auto vector_z<edm4hep::Vector2i>(const edm4hep::Vector2i&) {
    return 0;
  }

  namespace detail {
    /// Helper struct to determine the underlying type of edm4hep vector types
    template <typename V>
    struct ValueTypeHelper {
      using type = decltype(vector_x(std::declval<V>()));
    };

    /// Helper struct to determine whether a type is in a tuple of other types
    template <typename T, typename>
    struct TypeInTupleHelper : std::false_type {};

    template <typename T, typename... Ts>
    struct TypeInTupleHelper<T, std::tuple<Ts...>> : std::bool_constant<(std::is_same_v<T, Ts> || ...)> {};

    template <typename T, typename Tuple>
    constexpr static bool isInTuple = TypeInTupleHelper<T, Tuple>::value;
  } // namespace detail

  /// Type alias that returns the underlying type of edm4hep
  template <typename V>
  using ValueType = typename detail::ValueTypeHelper<V>::type;

  using EDM4hepVectorTypes = std::tuple<edm4hep::Vector3f, edm4hep::Vector3d, edm4hep::Vector2i, edm4hep::Vector2f>;
  using EDM4hepVector4DTypes = std::tuple<edm4hep::Vector4f>;
  using EDM4hepVector3DTypes = std::tuple<edm4hep::Vector3f, edm4hep::Vector3d>;
  using EDM4hepVector2DTypes = std::tuple<edm4hep::Vector2f, edm4hep::Vector2i>;
  using EDM4hepFloatVectorTypes = std::tuple<edm4hep::Vector2f, edm4hep::Vector3f, edm4hep::Vector3d>;

  template <typename V>
  using EnableIfEDM4hepVectorType = std::enable_if_t<detail::isInTuple<V, EDM4hepVectorTypes>, bool>;

  template <typename V>
  using EnableIfEDM4hepVector2DType = std::enable_if_t<detail::isInTuple<V, EDM4hepVector2DTypes>, bool>;

  template <typename V>
  using EnableIfEDM4hepVector3DType = std::enable_if_t<detail::isInTuple<V, EDM4hepVector3DTypes>, bool>;

  template <typename V>
  using EnableIfEdm4hepVector4DType = std::enable_if_t<detail::isInTuple<V, EDM4hepVector4DTypes>, bool>;
  template <typename V>
  using EnableIfEDM4hepFloatVectorType = std::enable_if_t<detail::isInTuple<V, EDM4hepFloatVectorTypes>, bool>;

  // inline edm4hep::Vector2f VectorFromPolar(const double r, const double theta)
  // {
  //  return {r * sin(theta), r * cos(theta)};
  //}

  template <class V = edm4hep::Vector3f, typename = EnableIfEDM4hepVector3DType<V>>
  V sphericalToVector(const double r, const double theta, const double phi) {
    using FloatType = ValueType<V>;
    const double sth = sin(theta);
    const double cth = cos(theta);
    const double sph = sin(phi);
    const double cph = cos(phi);
    const FloatType x = r * sth * cph;
    const FloatType y = r * sth * sph;
    const FloatType z = r * cth;
    return {x, y, z};
  }

  template <class V, typename = EnableIfEDM4hepVectorType<V>>
  double anglePolar(const V& v) {
    return std::atan2(std::hypot(vector_x(v), vector_y(v)), vector_z(v));
  }

  template <class V, typename = EnableIfEDM4hepVectorType<V>>
  double angleAzimuthal(const V& v) {
    return std::atan2(vector_y(v), vector_x(v));
  }

  template <class V, typename = EnableIfEDM4hepVectorType<V>>
  double eta(const V& v) {
    return angleToEta(anglePolar(v));
  }

  template <class V, typename = EnableIfEDM4hepVectorType<V>>
  double magnitude(const V& v) {
    return std::hypot(vector_x(v), vector_y(v), vector_z(v));
  }

  template <class V, typename = EnableIfEDM4hepVector3DType<V>>
  double magnitudeTransverse(const V& v) {
    return std::hypot(vector_x(v), vector_y(v));
  }

  template <class V, typename = EnableIfEDM4hepVector3DType<V>>
  double magnitudeLongitudinal(const V& v) {
    return vector_z(v);
  }

  template <class V, typename = EnableIfEDM4hepFloatVectorType<V>>
  V normalizeVector(const V& v, double norm = 1.) {
    const double old = magnitude(v);
    if (old == 0) {
      return v;
    }
    return (norm > 0) ? v * norm / old : v * 0;
  }

  template <class V, typename = EnableIfEDM4hepVector3DType<V>>
  constexpr V vectorTransverse(const V& v) {
    return {vector_x(v), vector_y(v), 0};
  }

  template <class V, typename = EnableIfEDM4hepVector3DType<V>>
  constexpr V vectorLongitudinal(const V& v) {
    return {0, 0, vector_z(v)};
  }

  // Two vector functions
  template <class V, typename = EnableIfEDM4hepVectorType<V>>
  double angleBetween(const V& v1, const V& v2) {
    const double dot = v1 * v2;
    if (dot == 0) {
      return 0.;
    }
    return acos(dot / (magnitude(v1) * magnitude(v2)));
  }

  // Project v onto v1
  template <class V, typename = EnableIfEDM4hepVectorType<V>>
  double projection(const V& v, const V& v1) {
    const double norm = magnitude(v1);
    if (norm == 0) {
      return magnitude(v);
    }
    return v * v1 / norm;
  }

} // namespace utils

template <typename V, edm4hep::utils::EnableIfEDM4hepVector2DType<V> = false>
inline constexpr V operator+(const V& v1, const V& v2) {
  const auto x = edm4hep::utils::vector_x(v1) + edm4hep::utils::vector_x(v2);
  const auto y = edm4hep::utils::vector_y(v1) + edm4hep::utils::vector_y(v2);
  return {x, y};
}

template <typename V, edm4hep::utils::EnableIfEDM4hepVector3DType<V> = false>
inline constexpr V operator+(const V& v1, const V& v2) {
  const auto x = edm4hep::utils::vector_x(v1) + edm4hep::utils::vector_x(v2);
  const auto y = edm4hep::utils::vector_y(v1) + edm4hep::utils::vector_y(v2);
  const auto z = edm4hep::utils::vector_z(v1) + edm4hep::utils::vector_z(v2);
  return {x, y, z};
}

template <typename V, edm4hep::utils::EnableIfEdm4hepVector4DType<V> = false>
inline constexpr V operator+(const V& v1, const V& v2) {
  const auto x = edm4hep::utils::vector_x(v1) + edm4hep::utils::vector_x(v2);
  const auto y = edm4hep::utils::vector_y(v1) + edm4hep::utils::vector_y(v2);
  const auto z = edm4hep::utils::vector_z(v1) + edm4hep::utils::vector_z(v2);
  const auto t = edm4hep::utils::vector_t(v1) + edm4hep::utils::vector_t(v2);
  return {x, y, z, t};
}

template <typename V, edm4hep::utils::EnableIfEDM4hepVector2DType<V> = false>
inline constexpr double operator*(const V& v1, const V& v2) {
  return edm4hep::utils::vector_x(v1) * edm4hep::utils::vector_x(v2) +
      edm4hep::utils::vector_y(v1) * edm4hep::utils::vector_y(v2);
}

template <typename V, edm4hep::utils::EnableIfEDM4hepVector3DType<V> = false>
inline constexpr double operator*(const V& v1, const V& v2) {
  return edm4hep::utils::vector_x(v1) * edm4hep::utils::vector_x(v2) +
      edm4hep::utils::vector_y(v1) * edm4hep::utils::vector_y(v2) +
      edm4hep::utils::vector_z(v1) * edm4hep::utils::vector_z(v2);
}

template <typename V, edm4hep::utils::EnableIfEdm4hepVector4DType<V> = false>
inline constexpr double operator*(const V& v1, const V& v2) {
  return (edm4hep::utils::vector_x(v1) * edm4hep::utils::vector_x(v2) +
          edm4hep::utils::vector_y(v1) * edm4hep::utils::vector_y(v2) +
          edm4hep::utils::vector_z(v1) * edm4hep::utils::vector_z(v2)) -
      edm4hep::utils::vector_t(v1) * edm4hep::utils::vector_t(v2);
}

template <typename V, edm4hep::utils::EnableIfEDM4hepVector2DType<V> = false>
inline constexpr V operator*(const double d, const V& v) {
  using VT = edm4hep::utils::ValueType<V>;
  const VT x = d * edm4hep::utils::vector_x(v);
  const VT y = d * edm4hep::utils::vector_y(v);
  return {x, y};
}

template <typename V, edm4hep::utils::EnableIfEDM4hepVector3DType<V> = false>
inline constexpr V operator*(const double d, const V& v) {
  using VT = edm4hep::utils::ValueType<V>;
  const VT x = d * edm4hep::utils::vector_x(v);
  const VT y = d * edm4hep::utils::vector_y(v);
  const VT z = d * edm4hep::utils::vector_z(v);
  return {x, y, z};
}

template <typename V, edm4hep::utils::EnableIfEdm4hepVector4DType<V> = false>
inline constexpr V operator*(const double d, const V& v) {
  using VT = edm4hep::utils::ValueType<V>;
  const VT x = d * edm4hep::utils::vector_x(v);
  const VT y = d * edm4hep::utils::vector_y(v);
  const VT z = d * edm4hep::utils::vector_z(v);
  const VT t = d * edm4hep::utils::vector_t(v);
  return {x, y, z, t};
}

template <typename V>
inline constexpr V operator*(const V& v, const double d) {
  return d * v;
}

template <typename V>
inline constexpr V operator-(const V& v1, const V& v2) {
  return v1 + (-1. * v2);
}

template <typename V>
inline constexpr V operator/(const V& v, const double d) {
  return (1. / d) * v;
}

} // namespace edm4hep

#endif
#endif // EDM4HEP_UTILS_VECTOR_LEGACY_HH
