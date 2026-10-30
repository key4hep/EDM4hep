#include "edm4hep/utils/Helix.h"

#include <cmath>
#include <limits>
#include <stdexcept>

namespace {
constexpr double twoPi = 6.28318530717958647693;
constexpr double roundingTolerance = 8. * std::numeric_limits<double>::epsilon();

bool finite(const edm4hep::Vector3d& v) { return std::isfinite(v.x) && std::isfinite(v.y) && std::isfinite(v.z); }

void checkPoint(const edm4hep::Vector3d& point) {
  if (!finite(point)) {
    throw std::invalid_argument("Helix: point must be finite");
  }
}

// Whether a point at the given offsets along and to the left of the direction
// of motion is the circle centre, up to rounding.
bool atCentre(double omega, double along, double normal) {
  const double x = 1. + omega * normal;
  const double y = omega * along;
  return std::abs(x) + std::abs(y) <= roundingTolerance * (1. + std::abs(omega * normal));
}

void checkPath(double path) {
  if (!std::isfinite(path)) {
    throw std::invalid_argument("Helix: path length must be finite");
  }
}
} // namespace

namespace edm4hep::utils {

Helix::Helix(const edm4hep::Vector3d& position, const edm4hep::Vector3d& momentum, double charge, double bField,
             const edm4hep::Vector3d& reference)
    : m_position(position), m_momentum(momentum), m_reference(reference), m_charge(charge),
      m_pt(std::hypot(momentum.x, momentum.y)), m_phi(std::atan2(momentum.y, momentum.x)) {
  if (!finite(position) || !finite(momentum) || !finite(reference) || !std::isfinite(charge) ||
      !std::isfinite(bField) || !(m_pt > 0.) || !std::isfinite(m_pt)) {
    throw std::invalid_argument("Helix: finite inputs and nonzero transverse momentum required");
  }
  m_omega = charge * fieldToCurvature * bField / m_pt;
  m_tanLambda = momentum.z / m_pt;
  if (!std::isfinite(m_omega) || !std::isfinite(m_tanLambda)) {
    throw std::invalid_argument("Helix: unrepresentable curvature or dip angle");
  }

  const auto canonical = canonicalAt(reference);
  m_d0 = canonical.d0;
  m_phi0 = canonical.phi0;
  m_z0 = canonical.z0;
}

Helix Helix::fromCanonical(double phi0, double d0, double z0, double omega, double tanLambda, double bField,
                           const edm4hep::Vector3d& reference) {
  if (!std::isfinite(phi0) || !std::isfinite(d0) || !std::isfinite(z0) || !std::isfinite(omega) ||
      !std::isfinite(tanLambda) || !std::isfinite(bField) || !finite(reference) || omega == 0. || bField == 0.) {
    throw std::invalid_argument("Helix: finite canonical parameters and nonzero omega and Bz required");
  }
  // Beyond omega * d0 = 1 the supplied point is the farthest from the reference,
  // not the closest. An excess of the size of float rounding (e.g. from a
  // TrackState) is moved onto the boundary, where the reference is the circle
  // centre and any phi0 and z0 remain valid.
  if (omega * d0 > 1.) {
    if (omega * d0 > 1. + 4. * std::numeric_limits<float>::epsilon()) {
      throw std::invalid_argument("Helix: omega * d0 > 1 does not describe a closest approach");
    }
    d0 = 1. / omega;
  }
  phi0 = std::remainder(phi0, twoPi);
  const double pt = std::abs(fieldToCurvature * bField / omega);
  Helix helix({reference.x - d0 * std::sin(phi0), reference.y + d0 * std::cos(phi0), reference.z + z0},
              {pt * std::cos(phi0), pt * std::sin(phi0), pt * tanLambda},
              std::copysign(1., omega) * std::copysign(1., bField), bField, reference);
  // Preserve the supplied canonical branch, including its z intercept.
  helix.m_d0 = d0;
  helix.m_phi0 = phi0;
  helix.m_z0 = z0;
  helix.m_omega = omega;
  helix.m_tanLambda = tanLambda;
  return helix;
}

Helix Helix::fromTrackState(const edm4hep::TrackState& trackState, double bField) {
  const auto& reference = trackState.referencePoint;
  return fromCanonical(trackState.phi, trackState.D0, trackState.Z0, trackState.omega, trackState.tanLambda, bField,
                       {reference.x, reference.y, reference.z});
}

edm4hep::TrackState Helix::toTrackState(int32_t location, const edm4hep::Vector3d& referencePoint) const {
  checkPoint(referencePoint);
  const auto canonical = canonicalAt(referencePoint);
  edm4hep::TrackState trackState{};
  trackState.location = location;
  trackState.D0 = canonical.d0;
  trackState.phi = canonical.phi0;
  trackState.omega = m_omega;
  trackState.Z0 = canonical.z0;
  trackState.tanLambda = m_tanLambda;
  trackState.referencePoint = edm4hep::Vector3f(referencePoint.x, referencePoint.y, referencePoint.z);
  return trackState;
}

edm4hep::Vector3d Helix::positionAt(double path) const {
  checkPath(path);
  const double halfAngle = 0.5 * m_omega * path;
  // sin(x)/x keeps the straight-line limit well behaved without subtracting
  // nearly equal sine/cosine values or introducing a curvature threshold.
  const double chord = path * (halfAngle == 0. ? 1. : std::sin(halfAngle) / halfAngle);
  return {m_position.x + chord * std::cos(m_phi - halfAngle), m_position.y + chord * std::sin(m_phi - halfAngle),
          m_position.z + path * m_tanLambda};
}

edm4hep::Vector3d Helix::momentumAt(double path) const {
  checkPath(path);
  const double phi = m_phi - m_omega * path;
  return {m_pt * std::cos(phi), m_pt * std::sin(phi), m_momentum.z};
}

std::optional<double> Helix::pathToZ(double z) const {
  if (!std::isfinite(z)) {
    throw std::invalid_argument("Helix: z must be finite");
  }
  if (z == m_position.z) {
    return 0.;
  }
  if (m_tanLambda == 0.) {
    return std::nullopt;
  }
  const double path = (z - m_position.z) / m_tanLambda;
  if (!(path >= 0.) || !std::isfinite(path)) {
    return std::nullopt;
  }
  return path;
}

std::optional<double> Helix::pathToRadius(double radius) const {
  if (!std::isfinite(radius) || radius < 0.) {
    throw std::invalid_argument("Helix: radius must be finite and non-negative");
  }
  const double along = m_position.x * std::cos(m_phi) + m_position.y * std::sin(m_phi);
  const double normal = m_position.y * std::cos(m_phi) - m_position.x * std::sin(m_phi);
  const double positionSquared = m_position.x * m_position.x + m_position.y * m_position.y;
  const double radiusSquared = radius * radius;
  const double excess = positionSquared - radiusSquared;
  if (std::abs(excess) <= roundingTolerance * (positionSquared + radiusSquared)) {
    return 0.;
  }
  const double curvatureNormal = m_omega * normal;
  const double curvatureExcess = 0.25 * m_omega * m_omega * excess;
  return firstCrossing(1. - curvatureNormal + curvatureExcess,
                       1. + std::abs(curvatureNormal) + std::abs(curvatureExcess), along, excess);
}

std::optional<double> Helix::pathToPlane(const edm4hep::Vector3d& point, const edm4hep::Vector3d& normal) const {
  const double normalLength = std::hypot(normal.x, normal.y);
  if (!finite(point) || !finite(normal) || normal.z != 0. || !(normalLength > 0.) || !std::isfinite(normalLength)) {
    throw std::invalid_argument("Helix: plane needs a finite point and a finite nonzero normal with zero z component");
  }
  const double nx = normal.x / normalLength;
  const double ny = normal.y / normalLength;
  const double positionDistance = nx * m_position.x + ny * m_position.y;
  const double planeDistance = nx * point.x + ny * point.y;
  const double distance = positionDistance - planeDistance;
  if (std::abs(distance) <= roundingTolerance * (std::abs(positionDistance) + std::abs(planeDistance))) {
    return 0.;
  }
  // Components of the unit normal along the direction of motion and to its left.
  const double along = nx * std::cos(m_phi) + ny * std::sin(m_phi);
  const double left = ny * std::cos(m_phi) - nx * std::sin(m_phi);
  const double curvatureDistance = 0.25 * m_omega * m_omega * distance;
  const double curvatureLeft = 0.5 * m_omega * left;
  return firstCrossing(curvatureDistance - curvatureLeft, std::abs(curvatureDistance) + std::abs(curvatureLeft),
                       0.5 * along, distance);
}

std::optional<double> Helix::firstCrossing(double quadratic, double quadraticScale, double halfLinear,
                                           double constant) const {
  // With the turning angle t = omega * path and q = 2 tan(t / 2) / omega, the
  // crossing conditions are quadratics in q whose coefficients stay bounded as
  // omega -> 0, where they reduce to the straight-line equation and q to the path.
  const double quadraticTolerance = roundingTolerance * quadraticScale;
  // A zero leading coefficient represents a crossing at half a turn, which
  // lies at infinity in q. Retain that root even when cancellation rounds the
  // coefficient slightly away from zero.
  if (std::abs(quadratic) <= quadraticTolerance) {
    quadratic = 0.;
  }
  double discriminant = halfLinear * halfLinear - quadratic * constant;
  const double discriminantTolerance = roundingTolerance * (halfLinear * halfLinear + std::abs(quadratic * constant)) +
                                       quadraticTolerance * std::abs(constant);
  if (discriminant < -discriminantTolerance) {
    return std::nullopt;
  }
  if (std::abs(discriminant) <= discriminantTolerance) {
    discriminant = 0.;
  }
  const double k = -(halfLinear + std::copysign(std::sqrt(discriminant), halfLinear));
  const double infinity = std::numeric_limits<double>::infinity();
  const double firstRoot = quadratic == 0. ? infinity : k / quadratic;
  const double secondRoot = quadratic == 0. ? (halfLinear == 0. ? infinity : -constant / (2. * halfLinear))
                                            : (k == 0. ? infinity : constant / k);
  std::optional<double> best;
  for (const double q : {firstRoot, secondRoot}) {
    double path;
    if (std::isnan(q)) {
      continue;
    } else if (std::isinf(q)) {
      // The crossing is half a turn away.
      if (m_omega == 0.) {
        continue;
      }
      path = 0.5 * twoPi / std::abs(m_omega);
    } else {
      const double x = 0.5 * m_omega * q;
      path = x == 0. ? q : q * std::atan(x) / x;
      if (path < 0.) {
        if (m_omega == 0.) {
          continue;
        }
        path += twoPi / std::abs(m_omega);
      }
    }
    if (std::isfinite(path) && (!best || path < *best)) {
      best = path;
    }
  }
  return best;
}

Helix::Canonical Helix::canonicalAt(const edm4hep::Vector3d& reference) const {
  const double dx = m_position.x - reference.x;
  const double dy = m_position.y - reference.y;
  const double normal = dy * std::cos(m_phi) - dx * std::sin(m_phi);
  // Rationalize (1 - |omega * centre|) / omega to avoid cancellation at high pT.
  const double centreNorm = std::hypot(m_omega * dx + std::sin(m_phi), m_omega * dy - std::cos(m_phi));
  const double d0 = (2. * normal - m_omega * (dx * dx + dy * dy)) / (1. + centreNorm);
  const double path = projectedPath(reference);
  const double z0 = m_position.z - reference.z + path * m_tanLambda;
  if (!std::isfinite(d0) || !std::isfinite(z0)) {
    throw std::invalid_argument("Helix: unrepresentable impact parameters");
  }
  return {d0, std::remainder(m_phi - m_omega * path, twoPi), z0};
}

double Helix::projectedPath(const edm4hep::Vector3d& point) const {
  const double dx = point.x - m_position.x;
  const double dy = point.y - m_position.y;
  const double along = dx * std::cos(m_phi) + dy * std::sin(m_phi);
  if (m_omega == 0.) {
    return along;
  }
  const double normal = dy * std::cos(m_phi) - dx * std::sin(m_phi);
  // At the circle centre all azimuths have the same transverse distance.
  return atCentre(m_omega, along, normal) ? 0. : std::atan2(m_omega * along, 1. + m_omega * normal) / m_omega;
}

edm4hep::Vector3d Helix::getExtrapolatedMomentum(const edm4hep::Vector3d& point) const {
  checkPoint(point);
  return momentumAt(projectedPath(point));
}

Helix::PointDistance Helix::getDistanceToPoint(const edm4hep::Vector3d& point) const {
  checkPoint(point);
  double path = projectedPath(point);
  if (m_omega != 0. && m_tanLambda != 0.) {
    const double dx = point.x - m_position.x;
    const double dy = point.y - m_position.y;
    const double normal = dy * std::cos(m_phi) - dx * std::sin(m_phi);
    const double along = dx * std::cos(m_phi) + dy * std::sin(m_phi);
    if (atCentre(m_omega, along, normal)) {
      path = (point.z - m_position.z) / m_tanLambda;
    } else {
      const double period = twoPi / std::abs(m_omega);
      const double zPeriod = period * m_tanLambda;
      const double turns = std::round((point.z - m_position.z - path * m_tanLambda) / zPeriod);
      if (turns != 0.) {
        path += turns * period;
      }
    }
  }
  const auto projection = positionAt(path);
  const double transverse = std::hypot(point.x - projection.x, point.y - projection.y);
  const double longitudinal = std::abs(point.z - projection.z);
  return {transverse, longitudinal, std::hypot(transverse, longitudinal), path};
}

} // namespace edm4hep::utils
