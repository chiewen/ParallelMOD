#include <gtest/gtest.h>
#include "lock_free_grid_test.h"
//#include "pgrid_test.h"
//#include "parallel_mod_test.h"

int main(int ac, char* av[]) {
	testing::InitGoogleTest(&ac, av);
	return RUN_ALL_TESTS();
}
