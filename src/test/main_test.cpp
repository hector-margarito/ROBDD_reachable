//
// Created by ludwig on 22.11.16.
//


#include "Tests.h"
#include "Manager.h"

TEST (ManagerTest, ManagerTableShouldHaveTwoNodes) { 
    ClassProject::Manager *manager = new ClassProject::Manager("TestManager");
    EXPECT_EQ (2, manager->uniqueTableSize());;
}

int main(int argc, char* argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
