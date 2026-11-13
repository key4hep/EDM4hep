/*
 * Copyright (c) 2014-2026 Key4hep-Project.
 *
 * This file is part of Key4hep.
 * See https://key4hep.github.io/key4hep-doc/ for further info.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef EDM4HEP_UNITS_H
#define EDM4HEP_UNITS_H

namespace edm4hep::units {

namespace si {
  /// electric charge of a single electron in Coulomb
  static constexpr double e = 1.602176634e-19;
} // namespace si

/// @namespace edm4hep::units
///
/// @brief Constants for consistent definition of physical units.
///
/// All physical quantities have a numerical value and a unit. The ones given
/// here define the base units as well conversion factors between them.
///
/// The base units for EDM4hep are
///
/// - length is expressedn in mm
/// - time is expressed in ns
/// - angles are expressed in radian
/// - charges are typically expressed in units of the elementary charge
///   **unless stated differently**, e.g. for some detector readouts
/// - energy, mass and momentum are expressed in GeV

/// Millimeter - natural unit for length
static constexpr double mm = 1.0;
/// Meter
static constexpr double m = 1000 * mm;
/// Nanosceond - natural unit for time
static constexpr double ns = 1.0;
/// Second
static constexpr double s = 1e9 * ns;
/// GeV - natural unit for energy
static constexpr double GeV = 1.0;
/// MeV
static constexpr double MeV = 1e-3 * GeV;
/// keV
static constexpr double keV = 1e-6 * GeV;
/// eV
static constexpr double eV = 1e-9 * GeV;
/// Joule
static constexpr double J = eV / si::e;
/// electric charge
static constexpr double e = 1.0;
/// Volt
static constexpr double V = eV / e;
/// Tesla
static constexpr double T = V * s / (m * m);

} // namespace edm4hep::units

#endif // EDM4HEP_UNITS_H
