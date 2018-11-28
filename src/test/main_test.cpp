//
// Created by ludwig on 22.11.16.
//


#include "Tests.h"
#include "Manager.h"
#include "ManagerInterface.h"

using namespace ClassProject;

TEST (ManagerTest, ManagerTableShouldHaveTwoNodes) { 
    Manager *manager = new Manager("TestManager");
    EXPECT_EQ(2, manager->uniqueTableSize());
}

TEST (ManagerTest, ManagerTableShouldHaveThreeNodesAfterCreateVar) { 
    ClassProject::Manager *manager = new Manager("TestManager");
    BDD_ID varId = manager->createVar("a");
    EXPECT_NE(BDD_ID_0, varId);
    EXPECT_NE(BDD_ID_1, varId);
    EXPECT_EQ(3, manager->uniqueTableSize());
}

TEST (ManagerTest, ManagerTableShouldHaveBooleanMethods) { 
    ClassProject::Manager *manager = new Manager("TestManager");
    EXPECT_TRUE(manager->True());
    EXPECT_FALSE(manager->False());
}

int main(int argc, char* argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
