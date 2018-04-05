#include "TBTK/Solver/Diagonalizer.h"

#include "gtest/gtest.h"

namespace TBTK{
namespace Solver{

TEST(Diagonalizer, Constructor){
	//Not testable on its own.
}

TEST(Diagonalizer, Destructor){
	//Not testable on its own.
}

int selfConsistencyCounter;
bool selfConsistencyCallback(Diagonalizer &diagonalizer){
	selfConsistencyCounter++;
	if(selfConsistencyCounter == 10)
		return true;
	else
		return false;
}

TEST(Diagonalizer, setSelfConsistencyCallback){
	Model model;
	model.setVerbose(false);
	model << HoppingAmplitude(1, {1}, {0}) + HC;
	model.construct();

	Diagonalizer solver;
	solver.setVerbose(false);
	solver.setModel(model);
	selfConsistencyCounter = 0;
	solver.setSelfConsistencyCallback(selfConsistencyCallback);
	solver.run();
	EXPECT_EQ(selfConsistencyCounter, 10);

	selfConsistencyCounter = 0;
	solver.setSelfConsistencyCallback(selfConsistencyCallback);
	solver.setMaxIterations(5);
	solver.run();
	EXPECT_EQ(selfConsistencyCounter, 5);
}

TEST(Diagonalizer, setMaxIterations){
	//Tested through Diagonalizer::setSelfConsistencyCallback
}

TEST(Diagonalizer, run){
	//Tested through
	//Diagonalizer::setSelfConsistencyCallback
	//Diagonalizer::getEigenValues
	//Diagonalizer::getEigenVectors
	//Diagonalizer::getEigenValue
	//Diagonalizer::getAmplitude
}

TEST(Diagonalizer, getEigenValues){
	Model model;
	model.setVerbose(false);
	model << HoppingAmplitude(1, {1}, {0}) + HC;
	model.construct();

	Diagonalizer solver;
	solver.setVerbose(false);
	solver.setModel(model);
	solver.run();

	//Normal access.
	const double *eigenValues0 = solver.getEigenValues();
	EXPECT_DOUBLE_EQ(eigenValues0[0], -1);
	EXPECT_DOUBLE_EQ(eigenValues0[1], 1);

	//Access with write permissions.
	double *eigenValues1 = solver.getEigenValuesRW();
	EXPECT_DOUBLE_EQ(eigenValues1[0], -1);
	EXPECT_DOUBLE_EQ(eigenValues1[1], 1);

	//Verify that write to internal data works (note that write is
	//performed to eigenValues1, while read is from eigenValues0).
	eigenValues1[0] = 2;
	EXPECT_DOUBLE_EQ(eigenValues0[0], 2);
}

TEST(Diagonalizer, getEigenValuesRW){
	//Tested in Diagonalizer::getEigenValues().
}

TEST(Diagonalizer, getEigenVectors){
	Model model;
	model.setVerbose(false);
	model << HoppingAmplitude(1, {1}, {0}) + HC;
	model.construct();

	Diagonalizer solver;
	solver.setVerbose(false);
	solver.setModel(model);
	solver.run();

	//Normal access.
	const std::complex<double> *eigenVectors0 = solver.getEigenVectors();
	EXPECT_DOUBLE_EQ(real(eigenVectors0[0]/eigenVectors0[1]), -1);
	EXPECT_DOUBLE_EQ(imag(eigenVectors0[0]/eigenVectors0[1]), 0);
	EXPECT_DOUBLE_EQ(real(eigenVectors0[2]/eigenVectors0[3]), 1);
	EXPECT_DOUBLE_EQ(imag(eigenVectors0[2]/eigenVectors0[3]), 0);

	//Access with write permissions.
	std::complex<double> *eigenVectors1 = solver.getEigenVectorsRW();
	EXPECT_DOUBLE_EQ(real(eigenVectors1[0]/eigenVectors1[1]), -1);
	EXPECT_DOUBLE_EQ(imag(eigenVectors1[0]/eigenVectors1[1]), 0);
	EXPECT_DOUBLE_EQ(real(eigenVectors1[2]/eigenVectors1[3]), 1);
	EXPECT_DOUBLE_EQ(imag(eigenVectors1[2]/eigenVectors1[3]), 0);

	//Verify that write to internal data works (note that write is
	//performed to eigenVectors1, while read is from eigenVectors0).
	eigenVectors1[0] = 2;
	EXPECT_DOUBLE_EQ(real(eigenVectors0[0]), 2);
	EXPECT_DOUBLE_EQ(imag(eigenVectors0[0]), 0);
}

TEST(Diagonalizer, getEigenVectorsRW){
	//Tested in Diagonalizer::getEigenVectorsRW
}

TEST(Diagonalizer, getEigenValue){
	Model model;
	model.setVerbose(false);
	model << HoppingAmplitude(1, {1}, {0}) + HC;
	model.construct();

	Diagonalizer solver;
	solver.setVerbose(false);
	solver.setModel(model);
	solver.run();

	EXPECT_DOUBLE_EQ(solver.getEigenValue(0), -1);
	EXPECT_DOUBLE_EQ(solver.getEigenValue(1), 1);
}

TEST(Diagonalizer, getAmplitude){
	Model model;
	model.setVerbose(false);
	model << HoppingAmplitude(1, {1}, {0}) + HC;
	model.construct();

	Diagonalizer solver;
	solver.setVerbose(false);
	solver.setModel(model);
	solver.run();

	EXPECT_DOUBLE_EQ(
		real(solver.getAmplitude(0, {0})/solver.getAmplitude(0, {1})), -1
	);
	EXPECT_DOUBLE_EQ(
		imag(solver.getAmplitude(0, {0})/solver.getAmplitude(0, {1})), 0
	);
	EXPECT_DOUBLE_EQ(
		real(solver.getAmplitude(1, {0})/solver.getAmplitude(1, {1})), 1
	);
	EXPECT_DOUBLE_EQ(
		imag(solver.getAmplitude(1, {0})/solver.getAmplitude(1, {1})), 0
	);
}

};	//End of namespace Solver
};	//End of namespace TBTK
