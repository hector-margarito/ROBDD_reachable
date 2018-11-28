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

TEST (ManagerTest_IsConstant, BDDNodeShouldBeAConstant) { 
    ClassProject::Manager *manager = new Manager("TestManager");
    
    BDD_ID varId = manager->createVar("a");

    EXPECT_TRUE(manager->isConstant(BDD_ID_0));
    EXPECT_TRUE(manager->isConstant(BDD_ID_1));
    EXPECT_FALSE(manager->isConstant(varId));
}

TEST (ManagerTest_IsVariable, BDDNodeShouldBeAVariable) { 
    ClassProject::Manager *manager = new Manager("TestManager");
    BDD_ID varId = manager->createVar("a");

    EXPECT_FALSE(manager->isVariable(BDD_ID_0));
    EXPECT_FALSE(manager->isVariable(BDD_ID_1));
    EXPECT_TRUE(manager->isVariable(varId));
}

TEST (ManagerTest_TopVar, BDDNodeShouldHaveTopVar) { 
    ClassProject::Manager *manager = new Manager("TestManager");
    BDD_ID varA_ID = manager->createVar("a");
    BDD_ID varB_ID = manager->createVar("b");

    EXPECT_EQ(BDD_ID_0, manager->topVar(BDD_ID_0));
    EXPECT_EQ(BDD_ID_1, manager->topVar(BDD_ID_1));
    EXPECT_EQ(varA_ID, manager->topVar(varA_ID));
    EXPECT_EQ(varB_ID, manager->topVar(varB_ID));
}

TEST (ManagerTest_GetTopVarName, BDDNodeShouldHaveTopVarName) { 
    ClassProject::Manager *manager = new Manager("TestManager");
    BDD_ID varA_ID = manager->createVar("a");
    BDD_ID varB_ID = manager->createVar("b");

    EXPECT_STREQ("0", manager->getTopVarName(BDD_ID_0).c_str());
    EXPECT_STREQ("1", manager->getTopVarName(BDD_ID_1).c_str());
    EXPECT_STREQ("a", manager->getTopVarName(varA_ID).c_str());
    EXPECT_STREQ("b", manager->getTopVarName(varB_ID).c_str());
}

int main(int argc, char* argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
