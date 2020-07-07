#include "gtest/gtest.h"

#include "TBTK/TBTK.h"
#include "TBTK/Test/BlockStructureDescriptor.h"

int main(int argc, char **argv){
	TBTK::Initialize();
	::testing::InitGoogleTest(&argc, argv);

	return RUN_ALL_TESTS();
}