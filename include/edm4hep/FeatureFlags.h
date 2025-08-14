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
#ifndef EDM4HEP_FEATURE_FLAGS_H
#define EDM4HEP_FEATURE_FLAGS_H

// "Feature flag" to denote an EDM4hep version that already switched the
// MCParticle spin member to helicity. See
// https://github.com/key4hep/EDM4hep/pull/404 for some details on the reasons
// for the switch.
//
// This pre-processor constant was introduced mainly to ease the transition of
// downstream packages between the two versions, as a simple version check was
// not easily possible. The reason being that a "release-like" tag was
// undesirable to avoid advertising perceived stability and any other version
// would have lead to even more comical pre-release tags than v00-99.
//
// Once EDM4hep v01-00 has been released and support for pre-release versions in
// downstream packages has reached a sufficiently low level, it should be
// possible to safely remove this.
#define EDM4HEP_MCPARTICLE_HAS_HELICITY 1

#endif // EDM4HEP_FEATURE_FLAGS_H
