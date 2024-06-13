/*
 * Copyright (c) 2014-2024 Key4hep-Project.
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
#ifndef EDM4HEP_CONSTANTS_H
#define EDM4HEP_CONSTANTS_H

#include <cstdint>

#define DEPRECATED_LABEL(name, newname)                                                                                \
  static constexpr const auto name [[deprecated("Use 'edm4hep::labels::" #newname "' instead")]] = labels::newname

namespace edm4hep {
namespace labels {
  static constexpr const char* CellIDEncoding = "CellIDEncoding";
  static constexpr const char* EventHeader = "EventHeader";
  static constexpr const char* EventWeights = "EventWeightNames";
  static constexpr const char* ShapeParameterNames = "shapeParameterNames";
  static constexpr const char* EventFilterStats = "EventFilterStats";

  /// The collection parameter name for accessing the names of the parameters for
  /// a ParticleID collection
  static constexpr const char* PIDParameterNames = "ParameterNames";
  static constexpr const char* PIDAlgoName = "AlgoName";
  static constexpr const char* PIDAlgoType = "AlgoType";
} // namespace labels

DEPRECATED_LABEL(CellIDEncoding, CellIDEncoding);
DEPRECATED_LABEL(EventHeaderName, EventHeader);
DEPRECATED_LABEL(EventWeights, EventWeights);
DEPRECATED_LABEL(shapeParameterNames, ShapeParameterNames);
DEPRECATED_LABEL(EventFilterStats, EventFilterStats);

DEPRECATED_LABEL(pidParameterNames, PIDParameterNames);
DEPRECATED_LABEL(pidAlgoName, PIDAlgoName);
DEPRECATED_LABEL(pidAlgoType, PIDAlgoType);

// Use 16 bits to encode the dimension
// Could go to 8 bits, but would need a fix in podio first
using DimType = std::uint16_t;

/// The enum for accessing cartesian coordinate values in covariance matrices
enum class Cartesian : DimType { x = 0, y, z };

/// The enum for accessing four momentum coordinate values in covariance
/// matrices
enum class FourMomCoords : DimType { x = 0, y, z, t };

/// The enum for accessing track parameter values in covariance matrices
enum class TrackParams : DimType { d0 = 0, phi, omega, z0, tanLambda, time };

/// Enum for accessing the covariance matrix in the TrackerPulse
enum class TrackerPulseDims : DimType { charge = 0, time };

} // namespace edm4hep

#undef DEPRECATED_LABEL

#endif // EDM4HEP_CONSTANTS_H
