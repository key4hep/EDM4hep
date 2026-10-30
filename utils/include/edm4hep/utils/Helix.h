#ifndef EDM4HEP_UTILS_HELIX_H
#define EDM4HEP_UTILS_HELIX_H

#include <edm4hep/TrackState.h>
#include <edm4hep/Vector3d.h>

#include <optional>

namespace edm4hep::utils {

/** A trajectory in a uniform magnetic field parallel to z.
 *
 * Positions and path lengths are in mm, momenta in GeV, charge in units of the
 * positron charge, and the signed z component of the field in tesla. The signed
 * curvature is omega = charge * Bz * 2.99792458e-4 / pT. Positive omega rotates
 * the momentum clockwise in the xy plane when moving along the trajectory.
 *
 * Impact parameters are measured relative to parameterReferencePoint. phi0 is
 * the momentum azimuth at the transverse closest approach to that point, in
 * [-pi, pi]. It equals the direction at the reference point when that point
 * lies on the trajectory. The closest approach within half a turn of the supplied position is used;
 * no attempt is made to infer the production turn of a curling track.
 */
class Helix {
public:
  /// pT [GeV] = fieldToCurvature * |q * Bz [T]| * R [mm], i.e. c * 1e-9 (eV -> GeV) * 1e-3 (m -> mm)
  static constexpr double fieldToCurvature = 2.99792458e-4;

  /// Construct from position and momentum. Zero charge or field gives a line.
  /// Throws std::invalid_argument for non-finite inputs or zero transverse momentum.
  Helix(const edm4hep::Vector3d& position, const edm4hep::Vector3d& momentum, double charge, double bField,
        const edm4hep::Vector3d& parameterReferencePoint = {});

  /// Construct from canonical parameters at transverse closest approach.
  /// Assumes unit absolute charge; its sign follows omega / Bz. Both omega and
  /// Bz must be nonzero, and omega * d0 <= 1 (otherwise the point described is
  /// the farthest from the reference). An excess within float rounding is
  /// moved onto omega * d0 = 1. Use the Cartesian constructor for straight
  /// tracks. Throws std::invalid_argument for invalid or non-finite inputs.
  static Helix fromCanonical(double phi0, double d0, double z0, double omega, double tanLambda, double bField,
                             const edm4hep::Vector3d& parameterReferencePoint = {});

  /// Construct from a TrackState whose phi is the direction at the transverse
  /// closest approach to its referencePoint, with D0/Z0 relative to that point.
  /// For a referencePoint on the track this is the direction at that point.
  /// Same requirements as fromCanonical.
  static Helix fromTrackState(const edm4hep::TrackState& trackState, double bField);

  /// TrackState with D0, phi, Z0 at the transverse closest approach to
  /// referencePoint, e.g. a hit position (D0 = Z0 = 0 if it lies on the track)
  /// or the origin for AtIP. Z0 uses the turn within half a turn of getPosition().
  /// Time and covariance are left at zero.
  edm4hep::TrackState toTrackState(int32_t location, const edm4hep::Vector3d& referencePoint) const;

  double getD0() const { return m_d0; }
  double getPhi0() const { return m_phi0; }
  double getZ0() const { return m_z0; }
  double getOmega() const { return m_omega; }
  double getTanLambda() const { return m_tanLambda; }
  double getCharge() const { return m_charge; }
  const edm4hep::Vector3d& getPosition() const { return m_position; }
  const edm4hep::Vector3d& getMomentum() const { return m_momentum; }
  const edm4hep::Vector3d& getParameterReferencePoint() const { return m_reference; }

  /// Position/momentum after a signed transverse path length from getPosition().
  edm4hep::Vector3d positionAt(double transversePathLength) const;
  edm4hep::Vector3d momentumAt(double transversePathLength) const;

  /// Smallest non-negative transverse path length from getPosition() at which
  /// the trajectory reaches the plane at z. Empty if it never does.
  std::optional<double> pathToZ(double z) const;

  /// Smallest non-negative transverse path length from getPosition() at which
  /// the trajectory reaches the cylinder of the given radius around the z axis,
  /// following later turns if needed. Empty if it never does.
  /// Throws std::invalid_argument for a negative or non-finite radius.
  std::optional<double> pathToRadius(double radius) const;

  /// Smallest non-negative transverse path length from getPosition() at which
  /// the trajectory reaches the plane parallel to z through point with the given
  /// normal, e.g. a face of a polygonal barrel. Empty if it never does.
  /// Throws std::invalid_argument for non-finite inputs or a normal that is zero
  /// in xy or has a nonzero z component.
  std::optional<double> pathToPlane(const edm4hep::Vector3d& point, const edm4hep::Vector3d& normal) const;

  /// Momentum at the transverse projection of a point onto the trajectory.
  /// At the circle centre (where the azimuth is undefined), use the initial direction.
  edm4hep::Vector3d getExtrapolatedMomentum(const edm4hep::Vector3d& point) const;

  struct PointDistance {
    double transverse;       ///< Non-negative distance to the projected circle or line.
    double longitudinal;     ///< Non-negative z residual at that projection.
    double threeDimensional; ///< Quadrature sum of the two residuals.
    double path;             ///< Signed transverse path length from getPosition() to the projection.
  };

  /// Residuals at the transverse projection, choosing the turn nearest the point
  /// in z. This is not a minimization of the full 3D distance. For a point on
  /// the circle axis, any azimuth is a projection; use one matching its z when possible.
  PointDistance getDistanceToPoint(const edm4hep::Vector3d& point) const;

private:
  /// Signed transverse path to the nearest transverse projection (within half a turn).
  double projectedPath(const edm4hep::Vector3d& point) const;

  struct Canonical {
    double d0;
    double phi0;
    double z0;
  };
  /// Impact parameters relative to a reference point.
  Canonical canonicalAt(const edm4hep::Vector3d& reference) const;

  /// Smallest non-negative path solving quadratic * q^2 + 2 * halfLinear * q + constant = 0,
  /// where q = 2 tan(omega * path / 2) / omega. quadraticScale bounds the terms summed into quadratic.
  std::optional<double> firstCrossing(double quadratic, double quadraticScale, double halfLinear,
                                      double constant) const;

  edm4hep::Vector3d m_position{};
  edm4hep::Vector3d m_momentum{};
  edm4hep::Vector3d m_reference{};
  double m_charge{};
  double m_pt{};
  double m_phi{};
  double m_omega{};
  double m_tanLambda{};
  double m_d0{};
  double m_phi0{};
  double m_z0{};
};

} // namespace edm4hep::utils

#endif // EDM4HEP_UTILS_HELIX_H
