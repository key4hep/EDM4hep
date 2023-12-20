#ifndef EDM4HEP_UTILS_VECTOR_HH
#define EDM4HEP_UTILS_VECTOR_HH

// These ultilies require concepts. If not available, use the fallback
// vector_utils_legacy.h instead to capture most functionality.
#if !__cpp_concepts
#include <edm4hep/utils/vector_utils_legacy.h>
#else

#include <edm4hep/Vector3f.h>

#include <cmath>

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
concept VectorHasT = requires(V v) {
  v.t;
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
concept Vector2D_XY = VectorHasX<V> && VectorHasY<V> && !VectorHasZ<V> && !VectorHasT<V> && !ClassVector<V>;

template <class V>
concept Vector2D_AB = VectorHasA<V> && VectorHasB<V> && !VectorHasZ<V> && !VectorHasT<V> && !ClassVector<V>;

template <class V>
concept Vector2D = Vector2D_XY<V> || Vector2D_AB<V>;

template <class V>
concept Vector3D = VectorHasX<V> && VectorHasY<V> && VectorHasZ<V> && !VectorHasT<V> && !ClassVector<V>;

template <class V>
concept Vector4D = VectorHasX<V> && VectorHasY<V> && VectorHasZ<V> && VectorHasT<V> && !ClassVector<V>;

template <class V>
concept Vector2or3D = Vector2D<V> || Vector3D<V>;

template <class V>
concept VectorND = Vector2D<V> || Vector3D<V> || Vector4D<V>;

template <class V>
concept VectorND_XYZ = Vector2D_XY<V> || Vector3D<V> || Vector4D<V>;

namespace utils {
  // Utility getters to accomodate different vector types
  template <VectorND_XYZ V>
  constexpr auto vector_x(const V& v) {
    return v.x;
  }

  template <VectorND_XYZ V>
  constexpr auto vector_y(const V& v) {
    return v.y;
  }

  template <Vector2D V>
  constexpr auto vector_z(const V&) {
    return 0;
  }

  template <Vector3D V>
  constexpr auto vector_z(const V& v) {
    return v.z;
  }

  template <Vector4D V>
  constexpr auto vector_z(const V& v) {
    return v.z;
  }

  template <Vector4D V>
  constexpr auto vector_t(const V& v) {
    return v.t;
  }

  template <Vector2or3D V>
  constexpr auto vector_t(const V&) {
    return 0;
  }

  template <Vector2D_AB V>
  constexpr auto vector_x(const V& v) {
    return v.a;
  }

  template <Vector2D_AB V>
  constexpr auto vector_y(const V& v) {
    return v.b;
  }

  namespace detail {
    /// Helper struct to determine the underlying type of edm4hep vector types
    template <typename V>
    struct ValueTypeHelper {
      using type = decltype(vector_x(std::declval<V>()));
    };
  } // namespace detail

  /// Type alias that returns the underlying type of edm4hep
  template <typename V>
  using ValueType = typename detail::ValueTypeHelper<V>::type;

  // inline edm4hep::Vector2f VectorFromPolar(const double r, const double theta)
  // {
  //  return {r * sin(theta), r * cos(theta)};
  //}

} // namespace utils

/// A vector that uses floating point numbers to represent its members
template <typename V>
concept FloatVectorND = std::floating_point<utils::ValueType<V>> && (Vector2D<V> || Vector3D<V>);

namespace utils {
  inline double etaToAngle(const double eta) {
    return std::atan(std::exp(-eta)) * 2.;
  }

  inline double angleToEta(const double theta) {
    return -std::log(std::tan(0.5 * theta));
  }

  template <Vector3D V = edm4hep::Vector3f>
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

  template <Vector2or3D V>
  double magnitude(const V& v) {
    return std::hypot(vector_x(v), vector_y(v), vector_z(v));
  }

  template <Vector4D V>
  double magnitude(const V& v) {
    return std::hypot(vector_x(v), vector_y(v), vector_z(v)) - vector_t(v) * vector_t(v);
  }

  template <Vector3D V>
  double magnitudeTransverse(const V& v) {
    return std::hypot(vector_x(v), vector_y(v));
  }

  template <Vector3D V>
  double magnitudeLongitudinal(const V& v) {
    return vector_z(v);
  }

  template <FloatVectorND V>
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
  template <Vector2or3D V>
  double angleBetween(const V& v1, const V& v2) {
    const double dot = v1 * v2;
    if (dot == 0) {
      return 0.;
    }
    return acos(dot / (magnitude(v1) * magnitude(v2)));
  }

  // Project v onto v1
  template <Vector2or3D V>
  double projection(const V& v, const V& v1) {
    const double norm = magnitude(v1);
    if (norm == 0) {
      return magnitude(v);
    }
    return v * v1 / norm;
  }

} // namespace utils

template <edm4hep::Vector2D V>
inline constexpr V operator+(const V& v1, const V& v2) {
  return {edm4hep::utils::vector_x(v1) + edm4hep::utils::vector_x(v2),
          edm4hep::utils::vector_y(v1) + edm4hep::utils::vector_y(v2)};
}

template <edm4hep::Vector3D V>
inline constexpr V operator+(const V& v1, const V& v2) {
  return {edm4hep::utils::vector_x(v1) + edm4hep::utils::vector_x(v2),
          edm4hep::utils::vector_y(v1) + edm4hep::utils::vector_y(v2),
          edm4hep::utils::vector_z(v1) + edm4hep::utils::vector_z(v2)};
}

template <edm4hep::Vector4D V>
inline constexpr V operator+(const V& v1, const V& v2) {
  return {edm4hep::utils::vector_x(v1) + edm4hep::utils::vector_x(v2),
          edm4hep::utils::vector_y(v1) + edm4hep::utils::vector_y(v2),
          edm4hep::utils::vector_z(v1) + edm4hep::utils::vector_z(v2),
          edm4hep::utils::vector_t(v1) + edm4hep::utils::vector_t(v2)};
}

template <edm4hep::Vector2D V>
inline constexpr auto operator*(const V& v1, const V& v2) {
  return edm4hep::utils::vector_x(v1) * edm4hep::utils::vector_x(v2) +
      edm4hep::utils::vector_y(v1) * edm4hep::utils::vector_y(v2);
}

template <edm4hep::Vector3D V>
inline constexpr auto operator*(const V& v1, const V& v2) {
  return edm4hep::utils::vector_x(v1) * edm4hep::utils::vector_x(v2) +
      edm4hep::utils::vector_y(v1) * edm4hep::utils::vector_y(v2) +
      edm4hep::utils::vector_z(v1) * edm4hep::utils::vector_z(v2);
}

template <edm4hep::Vector4D V>
inline constexpr auto operator*(const V& v1, const V& v2) {
  return (edm4hep::utils::vector_x(v1) * edm4hep::utils::vector_x(v2) +
          edm4hep::utils::vector_y(v1) * edm4hep::utils::vector_y(v2) +
          edm4hep::utils::vector_z(v1) * edm4hep::utils::vector_z(v2)) -
      edm4hep::utils::vector_t(v1) * edm4hep::utils::vector_t(v2);
}

template <edm4hep::Vector2D V>
inline constexpr V operator*(const double d, const V& v) {
  using VT = edm4hep::utils::ValueType<V>;
  const VT x = d * edm4hep::utils::vector_x(v);
  const VT y = d * edm4hep::utils::vector_y(v);
  return {x, y};
}

template <edm4hep::Vector3D V>
inline constexpr V operator*(const double d, const V& v) {
  using VT = edm4hep::utils::ValueType<V>;
  const VT x = d * edm4hep::utils::vector_x(v);
  const VT y = d * edm4hep::utils::vector_y(v);
  const VT z = d * edm4hep::utils::vector_z(v);
  return {x, y, z};
}

template <edm4hep::Vector4D V>
inline constexpr V operator*(const double d, const V& v) {
  using VT = edm4hep::utils::ValueType<V>;
  const VT x = d * edm4hep::utils::vector_x(v);
  const VT y = d * edm4hep::utils::vector_y(v);
  const VT z = d * edm4hep::utils::vector_z(v);
  const VT t = d * edm4hep::utils::vector_t(v);
  return {x, y, z, t};
}

template <edm4hep::VectorND V>
inline constexpr V operator*(const V& v, const double d) {
  return d * v;
}

template <edm4hep::VectorND V>
inline constexpr V operator-(const V& v1, const V& v2) {
  return v1 + (-1. * v2);
}

template <edm4hep::VectorND V>
inline constexpr V operator/(const V& v, const double d) {
  return (1. / d) * v;
}

} // namespace edm4hep

#endif
#endif
