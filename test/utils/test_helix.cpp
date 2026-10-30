#include "edm4hep/utils/Helix.h"

#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

#include <cmath>
#include <limits>
#include <stdexcept>
#include <tuple>
#include <utility>
#include <vector>

using Catch::Approx;
using edm4hep::utils::Helix;

namespace {
constexpr double conversion = Helix::fieldToCurvature;
constexpr double pi = 3.14159265358979323846;

void checkVector(const edm4hep::Vector3d& actual, const edm4hep::Vector3d& expected, double tolerance = 1.e-9) {
  REQUIRE(actual.x == Approx(expected.x).margin(tolerance));
  REQUIRE(actual.y == Approx(expected.y).margin(tolerance));
  REQUIRE(actual.z == Approx(expected.z).margin(tolerance));
}
} // namespace

TEST_CASE("Helix follows the Lorentz force with signed field and charge", "[helix]") {
  for (double charge : {-2., -1., 1., 2.}) {
    for (double field : {-2., 2.}) {
      CAPTURE(charge, field);
      const Helix helix({0., 0., 7.}, {1., 0., 0.5}, charge, field);
      const double omega = charge * conversion * field;
      const double radius = 1. / std::abs(omega);
      const double sign = std::copysign(1., omega);
      REQUIRE(helix.getOmega() == Approx(omega));
      REQUIRE(helix.getCharge() == charge);
      REQUIRE(helix.getTanLambda() == Approx(0.5));
      REQUIRE(helix.getD0() == Approx(0.).margin(1.e-12));
      REQUIRE(helix.getZ0() == Approx(7.));
      checkVector(helix.positionAt(pi * radius / 2.), {radius, -sign * radius, 7. + pi * radius / 4.});
      checkVector(helix.momentumAt(pi * radius / 2.), {0., -sign, 0.5});
      checkVector(helix.positionAt(-pi * radius / 2.), {-radius, -sign * radius, 7. - pi * radius / 4.});
      const auto point = helix.positionAt(123.);
      checkVector(helix.getExtrapolatedMomentum(point), helix.momentumAt(123.));
      REQUIRE(helix.getDistanceToPoint(point).threeDimensional == Approx(0.).margin(1.e-9));
    }
  }
}

TEST_CASE("Helix canonical parameters round trip after propagation", "[helix]") {
  const edm4hep::Vector3d reference{12., -34., 56.};
  for (double omega : {-0.001, 0.001}) {
    for (double phi : {-3.1, -0.4, 0., 3.1}) {
      CAPTURE(omega, phi);
      const auto canonical = Helix::fromCanonical(phi, 3., -5., omega, -0.7, 2., reference);
      REQUIRE(canonical.getD0() == Approx(3.));
      REQUIRE(canonical.getZ0() == Approx(-5.));
      REQUIRE(canonical.getPhi0() == Approx(phi).margin(1.e-12));
      checkVector(canonical.getPosition(),
                  {reference.x - 3. * std::sin(phi), reference.y + 3. * std::cos(phi), reference.z - 5.});
      for (double path : {-700., 0., 900.}) {
        const Helix propagated(canonical.positionAt(path), canonical.momentumAt(path), canonical.getCharge(), 2.,
                               reference);
        CAPTURE(path);
        REQUIRE(propagated.getD0() == Approx(3.).margin(1.e-9));
        REQUIRE(propagated.getZ0() == Approx(-5.).margin(1.e-9));
        REQUIRE(propagated.getPhi0() == Approx(phi).margin(1.e-12));
        REQUIRE(propagated.getOmega() == Approx(omega));
        REQUIRE(propagated.getTanLambda() == Approx(-0.7));
      }
    }
  }
}

TEST_CASE("Helix converts to and from TrackState", "[helix]") {
  // States written by hand: phi is the direction at the transverse closest
  // approach to the reference point, D0 and Z0 are relative to it.
  const double phi0 = 0.7, d0 = 40., z0 = -6., pt = 1.5, tanLambda = 0.4, field = 2., path = 200.;
  const edm4hep::Vector3d pca{30., -20., 15.};
  const auto left = [](double phi) { return edm4hep::Vector3d{-std::sin(phi), std::cos(phi), 0.}; };
  const edm4hep::Vector3d reference{pca.x - d0 * left(phi0).x, pca.y - d0 * left(phi0).y, pca.z - z0};
  for (double charge : {-1., 1.}) {
    CAPTURE(charge);
    const double omega = charge * conversion * field / pt;
    // Positive omega turns clockwise around a centre on the right.
    const edm4hep::Vector3d centre{pca.x - left(phi0).x / omega, pca.y - left(phi0).y / omega, 0.};
    const double hitPhi = phi0 - omega * path;
    const edm4hep::Vector3d hit{centre.x + left(hitPhi).x / omega, centre.y + left(hitPhi).y / omega,
                                pca.z + path * tanLambda};
    const edm4hep::Vector3d hitMomentum{pt * std::cos(hitPhi), pt * std::sin(hitPhi), pt * tanLambda};

    edm4hep::TrackState state{};
    state.D0 = d0;
    state.phi = phi0;
    state.omega = omega;
    state.Z0 = z0;
    state.tanLambda = tanLambda;
    state.referencePoint = edm4hep::Vector3f(reference.x, reference.y, reference.z);
    const auto fromState = Helix::fromTrackState(state, field);
    REQUIRE(fromState.getCharge() == charge);
    checkVector(fromState.getParameterReferencePoint(), reference, 1.e-5);
    // TrackState stores floats.
    REQUIRE(fromState.getDistanceToPoint(hit).threeDimensional == Approx(0.).margin(1.e-3));
    checkVector(fromState.getExtrapolatedMomentum(hit), hitMomentum, 1.e-5);

    const Helix atHit(hit, hitMomentum, charge, field);
    const auto onTrack = atHit.toTrackState(edm4hep::TrackState::AtLastHit, hit);
    REQUIRE(onTrack.location == edm4hep::TrackState::AtLastHit);
    REQUIRE(onTrack.D0 == Approx(0.).margin(1.e-6));
    REQUIRE(onTrack.phi == Approx(hitPhi));
    REQUIRE(onTrack.Z0 == Approx(0.).margin(1.e-6));
    REQUIRE(onTrack.referencePoint.x == Approx(hit.x));

    const edm4hep::Vector3d offset{hit.x - 3. * left(hitPhi).x, hit.y - 3. * left(hitPhi).y, hit.z - 7.};
    const auto offTrack = atHit.toTrackState(edm4hep::TrackState::AtOther, offset);
    REQUIRE(offTrack.D0 == Approx(3.));
    REQUIRE(offTrack.phi == Approx(hitPhi));
    REQUIRE(offTrack.Z0 == Approx(7.));

    const auto atReference = atHit.toTrackState(edm4hep::TrackState::AtIP, reference);
    REQUIRE(atReference.D0 == Approx(d0));
    REQUIRE(atReference.phi == Approx(phi0));
    REQUIRE(atReference.Z0 == Approx(z0));
    REQUIRE(atReference.omega == Approx(omega));
    REQUIRE(atReference.tanLambda == Approx(tanLambda));
  }
}

TEST_CASE("Helix canonical parameters must describe the closest approach", "[helix]") {
  for (double omega : {-0.01, 0.01}) {
    CAPTURE(omega);
    const double sign = std::copysign(1., omega);
    // omega * d0 > 1 is the farthest point of the circle from the reference.
    REQUIRE_THROWS_AS(Helix::fromCanonical(0., sign * 150., 0., omega, 0.3, 2.), std::invalid_argument);
    REQUIRE_THROWS_AS(Helix::fromCanonical(0., sign * 100.001, 0., omega, 0.3, 2.), std::invalid_argument);
    for (double d0 : {-150., 0., 99.}) {
      CAPTURE(d0);
      const auto helix = Helix::fromCanonical(0.3, sign * d0, 0., omega, 0.3, 2.);
      const Helix reconstructed(helix.getPosition(), helix.getMomentum(), helix.getCharge(), 2.);
      REQUIRE(reconstructed.getD0() == Approx(sign * d0).margin(1.e-9));
      REQUIRE(reconstructed.getPhi0() == Approx(0.3));
      REQUIRE(reconstructed.getZ0() == Approx(0.).margin(1.e-9));
    }
    // omega * d0 = 1 puts the reference at the circle centre.
    const auto centred = Helix::fromCanonical(0.3, sign * 100., 0., omega, 0.3, 2.);
    REQUIRE(centred.getDistanceToPoint({0., 0., 0.}).transverse == Approx(100.));
  }
  // Float rounding in a TrackState can push omega * d0 just above 1.
  for (double omega : {-0.001, 0.001}) {
    for (double d0 : {1000., 999.99999}) {
      CAPTURE(omega, d0);
      const auto helix = Helix::fromCanonical(0.3, std::copysign(d0, omega), 4., omega, 0.3, 2.);
      const auto state = helix.toTrackState(edm4hep::TrackState::AtOther, {});
      const auto back = Helix::fromTrackState(state, 2.);
      REQUIRE(back.getOmega() * back.getD0() <= 1.);
      REQUIRE(back.getD0() == Approx(helix.getD0()));
      REQUIRE(back.getPhi0() == Approx(0.3));
      REQUIRE(back.getZ0() == Approx(4.));
      REQUIRE(back.getDistanceToPoint(helix.positionAt(500.)).threeDimensional == Approx(0.).margin(1.e-3));
    }
  }
}

TEST_CASE("Helix residuals select the closest turn in z", "[helix]") {
  for (double omega : {-0.01, 0.01}) {
    for (double slope : {-0.4, 0., 0.4}) {
      CAPTURE(omega, slope);
      const auto helix = Helix::fromCanonical(0., 0., 2., omega, slope, 2.);
      const double radius = 1. / std::abs(omega);
      const double sign = std::copysign(1., omega);
      const double path = 6. * pi * radius + 0.5 * pi * radius;
      auto point = helix.positionAt(path);
      // At a quarter turn the radial direction is +x, for either curvature sign.
      point.x += 3.;
      point.z += 4.;
      const auto distance = helix.getDistanceToPoint(point);
      REQUIRE(distance.transverse == Approx(3.).margin(1.e-9));
      REQUIRE(distance.longitudinal == Approx(4.).margin(1.e-9));
      REQUIRE(distance.threeDimensional == Approx(5.).margin(1.e-9));
      // Without a slope every turn is equally close in z, so the first one is used.
      REQUIRE(distance.path == Approx(slope == 0. ? 0.5 * pi * radius : path));
      checkVector(helix.getExtrapolatedMomentum(point), helix.momentumAt(path));
      // Points behind the start have a negative path.
      REQUIRE(helix.getDistanceToPoint(helix.positionAt(-100.)).path == Approx(-100.));
      const auto centre = helix.getDistanceToPoint({0., -sign * radius, 100.});
      REQUIRE(centre.transverse == Approx(radius));
      REQUIRE(centre.longitudinal == Approx(slope == 0. ? 98. : 0.).margin(1.e-9));
    }
  }
}

TEST_CASE("Helix supports straight tracks and the small curvature limit", "[helix]") {
  for (const auto& helix :
       {Helix({1., 2., 3.}, {3., 4., 10.}, 0., 2.), Helix({1., 2., 3.}, {3., 4., 10.}, 1., 0.),
        Helix({1., 2., 3.}, {3., 4., 10.}, 1., 1.e-16), Helix({1., 2., 3.}, {3., 4., 10.}, 1., 1.e-307)}) {
    REQUIRE(helix.getD0() == Approx(0.4).margin(1.e-12));
    REQUIRE(helix.getZ0() == Approx(-1.4).margin(1.e-12));
    checkVector(helix.positionAt(10.), {7., 10., 23.});
    checkVector(helix.momentumAt(10.), {3., 4., 10.});
    // Normal displacement (3 mm) plus a z displacement (4 mm).
    const auto distance = helix.getDistanceToPoint({4.6, 11.8, 27.});
    REQUIRE(distance.transverse == Approx(3.));
    REQUIRE(distance.longitudinal == Approx(4.));
    REQUIRE(distance.threeDimensional == Approx(5.));
    // |P + s u| = 5 with P.u = 2.2 and |P|^2 = 5.
    const auto barrel = helix.pathToRadius(5.);
    REQUIRE(barrel.has_value());
    REQUIRE(*barrel == Approx(-2.2 + std::sqrt(2.2 * 2.2 + 20.)));
    const auto endcap = helix.pathToZ(23.);
    REQUIRE(endcap.has_value());
    REQUIRE(*endcap == Approx(10.));
    REQUIRE_FALSE(helix.pathToZ(0.).has_value());
  }
  // A line moving away from the cylinder never reaches it.
  REQUIRE_FALSE(Helix({10., 0., 0.}, {1., 0., 0.}, 0., 2.).pathToRadius(5.).has_value());
  REQUIRE(Helix({10., 0., 0.}, {1., 0., 0.}, 0., 2.).pathToRadius(20.).value() == Approx(10.));
}

TEST_CASE("Helix reaches cylinders and planes in the direction of motion", "[helix]") {
  for (double charge : {-1., 1.}) {
    for (double field : {-2., 2.}) {
      for (double phi : {-2.5, 0., 1.}) {
        CAPTURE(charge, field, phi);
        const double radius = 1. / std::abs(charge * conversion * field);
        const Helix helix({0., 0., 0.}, {std::cos(phi), std::sin(phi), 0.25}, charge, field);
        // A circle through the origin reaches radius r after turning 2 asin(r / 2R).
        for (double r : {10., 1000., 3000.}) {
          CAPTURE(r);
          const double first = 2. * radius * std::asin(r / (2. * radius));
          const auto path = helix.pathToRadius(r);
          REQUIRE(path.has_value());
          REQUIRE(*path == Approx(first));
          const auto position = helix.positionAt(*path);
          REQUIRE(std::hypot(position.x, position.y) == Approx(r));
          REQUIRE(position.z == Approx(0.25 * first));
          // From the far side of the circle the next crossing is on the way back.
          const double start = pi * radius;
          const Helix farSide(helix.positionAt(start), helix.momentumAt(start), charge, field);
          const auto back = farSide.pathToRadius(r);
          REQUIRE(back.has_value());
          REQUIRE(*back == Approx(2. * pi * radius - first - start));
        }
        REQUIRE_FALSE(helix.pathToRadius(2.1 * radius).has_value());
        REQUIRE(helix.pathToRadius(0.).value() == Approx(0.).margin(1.e-12));
        REQUIRE(helix.pathToZ(100.).value() == Approx(400.));
        REQUIRE_FALSE(helix.pathToZ(-100.).has_value());
      }
    }
  }
}

TEST_CASE("Helix reaches planes parallel to z", "[helix]") {
  for (double charge : {-1., 1.}) {
    for (double field : {-2., 2.}) {
      for (double phi : {-2.5, 0., 1.}) {
        CAPTURE(charge, field, phi);
        const double radius = 1. / std::abs(charge * conversion * field);
        // Positive omega turns clockwise, i.e. towards minus the left direction.
        const double turn = std::copysign(1., charge * field);
        const edm4hep::Vector3d along{std::cos(phi), std::sin(phi), 0.};
        const edm4hep::Vector3d left{-std::sin(phi), std::cos(phi), 0.};
        const auto local = [&](double a, double l) {
          return edm4hep::Vector3d{a * along.x + l * left.x, a * along.y + l * left.y, 123.};
        };
        const Helix helix({0., 0., 0.}, {along.x, along.y, 0.25}, charge, field);
        const std::vector<std::tuple<edm4hep::Vector3d, edm4hep::Vector3d, double>> crossings{
            {local(0.5 * radius, 0.), along, pi * radius / 6.},
            {local(-0.5 * radius, 0.), {-along.x, -along.y, 0.}, 7. * pi * radius / 6.},
            {local(0., -turn * radius), left, pi * radius / 2.},
            // Tangent half a turn away.
            {local(0., -2. * turn * radius), left, pi * radius},
        };
        for (const auto& [point, normal, expected] : crossings) {
          CAPTURE(expected / radius);
          const auto path = helix.pathToPlane(point, normal);
          REQUIRE(path.has_value());
          REQUIRE(*path == Approx(expected));
          const auto position = helix.positionAt(*path);
          REQUIRE((position.x - point.x) * normal.x + (position.y - point.y) * normal.y == Approx(0.).margin(1.e-6));
        }
        REQUIRE_FALSE(helix.pathToPlane(local(0., turn), left).has_value());
        REQUIRE_FALSE(helix.pathToPlane(local(1.5 * radius, 0.), along).has_value());
        REQUIRE(helix.pathToPlane(local(0., 0.), along).value() == Approx(0.).margin(1.e-12));
      }
    }
  }
  const Helix line({1., 2., 3.}, {1., 0., 1.}, 0., 2.);
  REQUIRE(line.pathToPlane({10., 0., 0.}, {-2., 0., 0.}).value() == Approx(9.));
  REQUIRE_FALSE(line.pathToPlane({-10., 0., 0.}, {1., 0., 0.}).has_value());
  REQUIRE_FALSE(line.pathToPlane({0., 5., 0.}, {0., 1., 0.}).has_value());
  const Helix almostLine({1., 2., 3.}, {1., 0., 1.}, 1.e-12, 2.);
  REQUIRE(almostLine.pathToPlane({10., 0., 0.}, {1., 0., 0.}).value() == Approx(9.));
  REQUIRE_THROWS_AS(line.pathToPlane({0., 0., 0.}, {1., 0., 1.}), std::invalid_argument);
  REQUIRE_THROWS_AS(line.pathToPlane({0., 0., 0.}, {0., 0., 0.}), std::invalid_argument);
  REQUIRE_THROWS_AS(line.pathToPlane({std::nan(""), 0., 0.}, {1., 0., 0.}), std::invalid_argument);
}

TEST_CASE("Helix cylinder crossings for degenerate geometries", "[helix]") {
  for (double field : {-2., 2.}) {
    CAPTURE(field);
    // A circle centred on the z axis stays on its own cylinder.
    const double radius = 1. / (conversion * std::abs(field));
    const Helix helix({radius, 0., 0.}, {0., std::copysign(1., field), 1.}, -1., field);
    REQUIRE(std::hypot(helix.positionAt(1234.).x, helix.positionAt(1234.).y) == Approx(radius));
    REQUIRE(helix.pathToRadius(radius).value() == 0.);
    REQUIRE_FALSE(helix.pathToRadius(0.5 * radius).has_value());
    REQUIRE_FALSE(helix.pathToRadius(2. * radius).has_value());
    // The distance to the axis grows for half a turn after the PCA and is symmetric
    // about it, so starting 0.1 rad past the PCA the cylinder through the point at
    // pi + 0.15 rad is first crossed at pi - 0.15 rad, almost half a turn later.
    const double omega = std::copysign(0.01, field);
    const auto pca = Helix::fromCanonical(0., 5., 0., omega, 0., field);
    const Helix start(pca.positionAt(0.1 / std::abs(omega)), pca.momentumAt(0.1 / std::abs(omega)), pca.getCharge(),
                      field);
    const auto target = pca.positionAt((pi + 0.15) / std::abs(omega));
    const auto nearlyHalf = start.pathToRadius(std::hypot(target.x, target.y));
    REQUIRE(nearlyHalf.has_value());
    REQUIRE(*nearlyHalf == Approx((pi - 0.25) / std::abs(omega)));
    REQUIRE_FALSE(pca.pathToZ(10.).has_value());
  }
}

TEST_CASE("Helix retains tangent cylinder intersections under rotation", "[helix]") {
  for (double omega : {-0.01, 0.01}) {
    for (double phi : {0., 0.006, 0.2, 0.7, 1.5, -2.5}) {
      CAPTURE(omega, phi);
      const auto pca = Helix::fromCanonical(phi, std::copysign(5., omega), 0., omega, 0., 2.);
      const double radius = 1. / std::abs(omega);
      const auto tangent = pca.pathToRadius(195.);
      REQUIRE(tangent.has_value());
      REQUIRE(*tangent == Approx(pi * radius).margin(1.e-9));
      const auto position = pca.positionAt(*tangent);
      REQUIRE(std::hypot(position.x, position.y) == Approx(195.).margin(1.e-9));
      REQUIRE(pca.pathToRadius(5.).value() == 0.);
      REQUIRE_FALSE(pca.pathToRadius(195. + 1.e-6).has_value());
      REQUIRE_FALSE(pca.pathToRadius(5. - 1.e-6).has_value());
      const auto justInside = pca.pathToRadius(195. - 1.e-6);
      REQUIRE(justInside.has_value());
      REQUIRE(*justInside < *tangent);
      const auto justInsidePosition = pca.positionAt(*justInside);
      REQUIRE(std::hypot(justInsidePosition.x, justInsidePosition.y) == Approx(195. - 1.e-6).margin(1.e-9));

      // Starting away from either extremum must retain both tangent cylinders,
      // including the inner cylinder reached after most of a complete turn.
      const Helix start(pca.positionAt(0.7 * radius), pca.momentumAt(0.7 * radius), pca.getCharge(), 2.);
      for (const auto& target : {std::pair{195., (pi - 0.7) * radius}, std::pair{5., (2. * pi - 0.7) * radius}}) {
        const auto path = start.pathToRadius(target.first);
        REQUIRE(path.has_value());
        REQUIRE(*path == Approx(target.second).margin(1.e-9));
        const auto hit = start.positionAt(*path);
        REQUIRE(std::hypot(hit.x, hit.y) == Approx(target.first).margin(1.e-9));
      }
    }
  }
  const Helix line({-10., 5., 0.}, {1., 0., 0.}, 0., 2.);
  REQUIRE(line.pathToRadius(5.).value() == Approx(10.));
  REQUIRE_FALSE(line.pathToRadius(5. - 1.e-6).has_value());
  REQUIRE_FALSE(Helix({10., 5., 0.}, {1., 0., 0.}, 0., 2.).pathToRadius(5.).has_value());
}

TEST_CASE("Helix already in a z plane has a zero path to that plane", "[helix]") {
  for (double charge : {0., 1.}) {
    for (double pz : {-1., 0., 1.}) {
      CAPTURE(charge, pz);
      const Helix helix({1., 2., 3.}, {1., 0., pz}, charge, 2.);
      REQUIRE(helix.pathToZ(3.).value() == 0.);
      if (pz == 0.) {
        REQUIRE_FALSE(helix.pathToZ(4.).has_value());
        REQUIRE_FALSE(helix.pathToZ(2.).has_value());
      }
    }
  }
}

TEST_CASE("Helix chooses a local PCA for a curling track", "[helix]") {
  const auto helix = Helix::fromCanonical(0.2, -2., 500., 0.01, 0.3, 2.);
  const double period = 2. * pi / helix.getOmega();
  const Helix nextTurn(helix.positionAt(period + 40.), helix.momentumAt(period + 40.), 1., 2.);
  REQUIRE(nextTurn.getD0() == Approx(-2.));
  REQUIRE(nextTurn.getZ0() == Approx(500. + period * 0.3));
  REQUIRE(nextTurn.getPhi0() == Approx(0.2));
}

TEST_CASE("Helix rejects singular or non-finite inputs", "[helix]") {
  const double nan = std::numeric_limits<double>::quiet_NaN();
  REQUIRE_THROWS_AS(Helix({}, {0., 0., 1.}, 1., 2.), std::invalid_argument);
  REQUIRE_THROWS_AS(Helix({nan, 0., 0.}, {1., 0., 0.}, 1., 2.), std::invalid_argument);
  REQUIRE_THROWS_AS(Helix({}, {1., nan, 0.}, 1., 2.), std::invalid_argument);
  REQUIRE_THROWS_AS(Helix({}, {1., 0., 0.}, nan, 2.), std::invalid_argument);
  REQUIRE_THROWS_AS(Helix({}, {1., 0., 0.}, 1., nan), std::invalid_argument);
  REQUIRE_THROWS_AS(Helix::fromCanonical(0., 0., 0., 0., 1., 2.), std::invalid_argument);
  REQUIRE_THROWS_AS(Helix::fromCanonical(0., 0., 0., 0.01, 1., 0.), std::invalid_argument);
  REQUIRE_THROWS_AS(Helix::fromCanonical(nan, 0., 0., 0.01, 1., 2.), std::invalid_argument);
  const Helix helix({}, {1., 0., 0.}, 1., 2.);
  REQUIRE_THROWS_AS(helix.positionAt(nan), std::invalid_argument);
  REQUIRE_THROWS_AS(helix.momentumAt(nan), std::invalid_argument);
  REQUIRE_THROWS_AS(helix.getExtrapolatedMomentum({0., nan, 0.}), std::invalid_argument);
  REQUIRE_THROWS_AS(helix.getDistanceToPoint({0., 0., nan}), std::invalid_argument);
  REQUIRE_THROWS_AS(helix.pathToZ(nan), std::invalid_argument);
  REQUIRE_THROWS_AS(helix.pathToRadius(nan), std::invalid_argument);
  REQUIRE_THROWS_AS(helix.pathToRadius(-1.), std::invalid_argument);
}
