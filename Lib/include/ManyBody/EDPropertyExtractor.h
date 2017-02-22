/* Copyright 2017 Kristofer Björnson
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
 *  @file EDPropertyExtractor.h
 *  @brief Extracts physical properties from the DiagonalizationSolver
 *
 *  @author Kristofer Björnson
 */

#ifndef COM_DAFER45_TBTK_ED_PROPERTY_EXTRACTOR
#define COM_DAFER45_TBTK_ED_PROPERTY_EXTRACTOR

#include "ChebyshevSolver.h"
#include "ExactDiagonalizationSolver.h"
#include "PropertyExtractor.h"

#include <complex>

namespace TBTK{

class EDPropertyExtractor : public PropertyExtractor{
public:
	/** Constructor. */
	EDPropertyExtractor(ExactDiagonalizationSolver *edSolver);

	/** Destructor. */
	~EDPropertyExtractor();

	/** Calculate Green's function. */
	std::complex<double>* calculateGreensFunction(
		Index to,
		Index from,
		ChebyshevSolver::GreensFunctionType type = ChebyshevSolver::GreensFunctionType::Retarded
	);

	/** Overrides PropertyExtractor::calculateExpectationValue(). */
	virtual std::complex<double> calculateExpectationValue(
		Index to,
		Index from
	);

	/** Overrides PropertyExtractor::calculateDensity(). */
	virtual Property::Density* calculateDensity(
		Index pattern,
		Index ranges
	);

	/** Overrides PropertyExtractor::calculateMagnetization().  */
	virtual Property::Magnetization* calculateMagnetization(
		Index pattern,
		Index ranges
	);

	/** Overrides PropertyExtractor::calculateLDOS(). */
	virtual Property::LDOS* calculateLDOS(Index pattern, Index ranges);

	/** Overrides PropertyExtractor::calculateSpinPolarizedLDOS(). */
	virtual Property::SpinPolarizedLDOS* calculateSpinPolarizedLDOS(
		Index pattern,
		Index ranges
	);
private:
	/** DiagonalizationSolver to work on. */
	ExactDiagonalizationSolver *edSolver;

	/** Callback for calculating density. Used by calculateDensity(). */
	static void calculateDensityCallback(
		PropertyExtractor *cb_this,
		void *density,
		const Index &index,
		int offset
	);

	/** Callback for calculating magnetization. Used by
	 *  calculateMangetization(). */
	static void calculateMagnetizationCallback(
		PropertyExtractor *cb_this,
		void *density,
		const Index &index,
		int offset
	);

	/** Callback for calculating local density of states. Used by
	 *  calculateLDOS(). */
	static void calculateLDOSCallback(
		PropertyExtractor *cb_this,
		void *ldos,
		const Index &index,
		int offset
	);

	/** Callback for calculating spin-polarized local density of states.
	 *  Used by calculateSpinPolarizedLDOS(). */
	static void calculateSpinPolarizedLDOSCallback(
		PropertyExtractor *cb_this,
		void *spinPolarizedLDOS,
		const Index &index,
		int offset
	);
};

};	//End of namespace TBTK

#endif