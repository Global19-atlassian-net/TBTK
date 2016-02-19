/** @file PropertyExtractorChebyshev.cpp
 *
 *  @author Kristofer Björnson
 */

#include "../include/PropertyExtractorChebyshev.h"

using namespace std;

PropertyExtractorChebyshev::PropertyExtractorChebyshev(ChebyshevSolver *cSolver,
							int numCoefficients,
							int energyResolution,
							bool useGPUToCalculateCoefficients,
							bool useGPUToGenerateGreensFunctions,
							bool useLookupTable){
	this->cSolver = cSolver;
	this->numCoefficients = numCoefficients;
	this->energyResolution = energyResolution;
	this->useGPUToCalculateCoefficients = useGPUToCalculateCoefficients;
	this->useGPUToGenerateGreensFunctions = useGPUToGenerateGreensFunctions;
	this->useLookupTable = useLookupTable;

	if(useLookupTable){
		cSolver->generateLookupTable(numCoefficients, energyResolution);
		if(useGPUToGenerateGreensFunctions)
			cSolver->loadLookupTableGPU();
	}
	else if(useGPUToGenerateGreensFunctions){
		cout << "Error in PropertyExtractorChebyshev: useLookupTable cannot be false if useGPUToGenerateGreensFunction is true.\n";
		exit(1);
	}
}

PropertyExtractorChebyshev::~PropertyExtractorChebyshev(){
	if(useGPUToGenerateGreensFunctions)
		cSolver->destroyLookupTableGPU();

//	if(useLookupTable)
}

complex<double>* PropertyExtractorChebyshev::calculateGreensFunction(Index to, Index from){
	vector<Index> toIndices;
	toIndices.push_back(to);

	return calculateGreensFunctions(toIndices, from);
}

complex<double>* PropertyExtractorChebyshev::calculateGreensFunctions(vector<Index> &to, Index from){
	complex<double> *coefficients = new complex<double>[energyResolution*to.size()];

	if(useGPUToCalculateCoefficients){
		cSolver->calculateCoefficientsGPU(to, from, coefficients, numCoefficients);
	}
	else{
		cout << "Error in PropertyExtractorChebyshev::calculateGreensFunctions: CPU generation of coefficients not yet supported.\n";
		exit(1);
	}

	complex<double> *greensFunction = new complex<double>[energyResolution*to.size()];

	if(useGPUToGenerateGreensFunctions){
		for(unsigned int n = 0; n < to.size(); n++){
			cSolver->generateGreensFunctionGPU(greensFunction,
								&(coefficients[n*numCoefficients]));
		}
	}
	else{
		if(useLookupTable){
			#pragma omp parallel for
			for(unsigned int n = 0; n < to.size(); n++){
				cSolver->generateGreensFunction(greensFunction,
								&(coefficients[n*numCoefficients]));
			}
		}
		else{
			#pragma omp parallel for
			for(unsigned int n = 0; n < to.size(); n++){
				cSolver->generateGreensFunction(greensFunction,
								&(coefficients[n*numCoefficients]),
								numCoefficients,
								energyResolution);
			}
		}
	}

	delete [] coefficients;

	return greensFunction;
}

double* PropertyExtractorChebyshev::calculateLDOS(Index pattern, Index ranges){
	for(unsigned int n = 0; n < pattern.indices.size(); n++){
		if(pattern.indices.at(n) >= 0)
			ranges.indices.at(n) = 1;
	}

	int ldosArraySize = 1.;
	for(unsigned int n = 0; n < ranges.indices.size(); n++){
		if(pattern.indices.at(n) < IDX_SUM_ALL)
			ldosArraySize *= ranges.indices.at(n);
	}
	double *ldos = new double[ldosArraySize];
	for(int n = 0; n < ldosArraySize; n++)
		ldos[n] = 0.;

	calculate(calculateLDOSCallback, (void*)ldos, pattern, ranges, 0, 1);

	return ldos;
}

complex<double>* PropertyExtractorChebyshev::calculateSP_LDOS(Index pattern, Index ranges){
	hint = new int[1];
	((int*)hint)[0] = -1;
	for(unsigned int n = 0; n < pattern.indices.size(); n++){
		if(pattern.indices.at(n) == IDX_SPIN){
			((int*)hint)[0] = n;
			pattern.indices.at(n) = 0;
			ranges.indices.at(n) = 1;
			break;
		}
	}
	if(((int*)hint)[0] == -1){
		cout << "Error in PropertyExtractorChebyshev::calculateSP_LDOS: No spin index indicated.\n";
		delete [] (int*)hint;
		return NULL;
	}

	for(unsigned int n = 0; n < pattern.indices.size(); n++){
		if(pattern.indices.at(n) >= 0)
			ranges.indices.at(n) = 1;
	}

	int sp_ldosArraySize = 1;
	for(unsigned int n = 0; n < ranges.indices.size(); n++){
		if(pattern.indices.at(n) < IDX_SUM_ALL)
			sp_ldosArraySize *= ranges.indices.at(n);
	}
	complex<double> *sp_ldos = new complex<double>[4*sp_ldosArraySize];
	for(int n = 0; n < 4*sp_ldosArraySize; n++)
		sp_ldos[n] = 0.;

	calculate(calculateSP_LDOSCallback, (void*)sp_ldos, pattern, ranges, 0, 1);

	delete [] (int*)hint;

	return sp_ldos;
}

void PropertyExtractorChebyshev::calculateLDOSCallback(PropertyExtractorChebyshev *cb_this, void *ldos, const Index &index, int offset){
	complex<double> *greensFunction = cb_this->calculateGreensFunction(index, index);

	for(int n = 0; n < cb_this->energyResolution; n++)
		((double*)ldos)[cb_this->energyResolution*offset + n] -= imag(greensFunction[n])/M_PI;

	delete [] greensFunction;
}

void PropertyExtractorChebyshev::calculateSP_LDOSCallback(PropertyExtractorChebyshev *cb_this, void *sp_ldos, const Index &index, int offset){
	int spinIndex = ((int*)(cb_this->hint))[0];
	Index to(index);
	Index from(index);
	complex<double> *greensFunction;

	for(int n = 0; n < 4; n++){
		to.indices.at(spinIndex) = n/2;		//up, up, down, down
		from.indices.at(spinIndex) = n%2;	//up, down, up, down
		greensFunction = cb_this->calculateGreensFunction(to, from);

		for(int e = 0; e < cb_this->energyResolution; e++)
			((complex<double>*)sp_ldos)[4*cb_this->energyResolution*offset + 4*e + n] = greensFunction[e];

		delete [] greensFunction;
	}
}

void PropertyExtractorChebyshev::calculate(void (*callback)(PropertyExtractorChebyshev *cb_this, void *memory, const Index &index, int offset),
					void *memory, Index pattern, const Index &ranges, int currentOffset, int offsetMultiplier){
	int currentSubindex = pattern.indices.size()-1;
	for(; currentSubindex >= 0; currentSubindex--){
		if(pattern.indices.at(currentSubindex) < 0)
			break;
	}

	if(currentSubindex == -1){
		callback(this, memory, pattern, currentOffset);
	}
	else{
		int nextOffsetMultiplier = offsetMultiplier;
		if(pattern.indices.at(currentSubindex) < IDX_SUM_ALL)
			nextOffsetMultiplier *= ranges.indices.at(currentSubindex);
		bool isSumIndex = false;
		if(pattern.indices.at(currentSubindex) == IDX_SUM_ALL)
			isSumIndex = true;
		for(int n = 0; n < ranges.indices.at(currentSubindex); n++){
			pattern.indices.at(currentSubindex) = n;
			calculate(callback,
					memory,
					pattern,
					ranges,
					currentOffset,
					nextOffsetMultiplier
			);
			if(!isSumIndex)
				currentOffset += offsetMultiplier;
		}
	}
}
