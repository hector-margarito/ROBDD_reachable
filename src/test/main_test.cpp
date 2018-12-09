#include "Tests.h"
#include "Manager.h"
#include "ManagerInterface.h"

using namespace ClassProject;

int main(int argc, char* argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
