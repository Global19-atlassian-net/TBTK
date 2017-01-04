/* Copyright 2016 Kristofer Björnson
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

/** @package TBTKcalc
 *  @file LDOS.h
 *  @brief Property container for spectral function
 *
 *  @author Kristofer Björnson
 */

#ifndef COM_DAFER45_TBTK_SPECTRAL_FUNCTION
#define COM_DAFER45_TBTK_SPECTRAL_FUNCTION

#include "LDOS.h"

namespace TBTK{
namespace Property{

class SpectralFunction : public LDOS{
public:
	/** Constructor. */
	SpectralFunction(
		int dimensions,
		const int *ranges,
		double lowerBound,
		double upperBound,
		int resolution
	);

	/** Constructor. */
	SpectralFunction(
		int dimensions,
		const int *ranges,
		double lowerBound,
		double upperBound,
		int resolution,
		const double *data
	);

	/** Destructor. */
	~SpectralFunction();
private:
};

};	//End namespace Property
};	//End namespace TBTK

#endif
