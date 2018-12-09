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
    BDD_ID AandB_ID = manager->ite(varA_ID, varB_ID, BDD_ID_0);

    EXPECT_EQ(BDD_ID_0, manager->topVar(BDD_ID_0));
    EXPECT_EQ(BDD_ID_1, manager->topVar(BDD_ID_1));
    EXPECT_EQ(varA_ID, manager->topVar(varA_ID));
    EXPECT_EQ(varB_ID, manager->topVar(varB_ID));
    EXPECT_EQ(varA_ID, manager->topVar(AandB_ID));
}

TEST (ManagerTest_GetTopVarName, BDDNodeShouldHaveTopVarName) { 
    ClassProject::Manager *manager = new Manager("TestManager");
    BDD_ID varA_ID = manager->createVar("a");
    BDD_ID varB_ID = manager->createVar("b");
    BDD_ID varC_ID = manager->createVar("c");
    BDD_ID AandB_ID = manager->ite(varA_ID, varB_ID, BDD_ID_0);

    EXPECT_STREQ("0", manager->getTopVarName(BDD_ID_0).c_str());
    EXPECT_STREQ("1", manager->getTopVarName(BDD_ID_1).c_str());
    EXPECT_STREQ("a", manager->getTopVarName(varA_ID).c_str());
    EXPECT_STREQ("b", manager->getTopVarName(varB_ID).c_str());
    EXPECT_STREQ("a", manager->getTopVarName(AandB_ID).c_str());
}

TEST (ManagerTest_FindVars, ManagerShouldFindVariables) { 
    ClassProject::Manager *manager = new Manager("TestManager");
    BDD_ID varA_ID = manager->createVar("a");
    BDD_ID varB_ID = manager->createVar("b");
    BDD_ID varC_ID = manager->createVar("b");
    BDD_ID AandB_ID = manager->ite(varA_ID, varB_ID, BDD_ID_0);

    std::set<BDD_ID> vars, vars_AandB;
    manager->findVars(BDD_ID_0, vars);
    EXPECT_TRUE(vars.empty());

    manager->findVars(varA_ID, vars);
    EXPECT_EQ(1, vars.size());

    BDD_ID varA_aux = *vars.find(varA_ID);
    EXPECT_TRUE(manager->isVariable(varA_aux));
    EXPECT_EQ(varA_ID, varA_aux);

    manager->findVars(AandB_ID, vars_AandB);

    EXPECT_EQ(2, vars_AandB.size());
    EXPECT_EQ(varA_ID, *(vars_AandB.find(varA_ID)));
    EXPECT_EQ(varB_ID, *(vars_AandB.find(varB_ID)));
}

TEST (ManagerTest_FindNodes, ManagerShouldFindNodes) { 
    ClassProject::Manager *manager = new Manager("TestManager");
    BDD_ID varA_ID = manager->createVar("a");
    BDD_ID varB_ID = manager->createVar("b");

    std::set<BDD_ID> vars_1, vars_2, vars_3;
    manager->findNodes(BDD_ID_0, vars_1);
    EXPECT_EQ(1, vars_1.size());

    manager->findNodes(varA_ID, vars_2);
    // We should expect the own variable a and the terminal nodes 0 and 1    
    EXPECT_EQ(3, vars_2.size()); 
    EXPECT_EQ(varA_ID, *(vars_2.find(varA_ID)));
    EXPECT_EQ(BDD_ID_0, *(vars_2.find(BDD_ID_0)));
    EXPECT_EQ(BDD_ID_1, *(vars_2.find(BDD_ID_1)));

    BDD_ID AandB_ID = manager->ite(varA_ID, varB_ID, BDD_ID_0);
    manager->findNodes(AandB_ID, vars_3);
    EXPECT_EQ(4, vars_3.size());
    EXPECT_EQ(AandB_ID, *(vars_3.find(AandB_ID)));
    EXPECT_EQ(varB_ID, *(vars_3.find(varB_ID)));
    EXPECT_EQ(BDD_ID_0, *(vars_3.find(BDD_ID_0)));
    EXPECT_EQ(BDD_ID_1, *(vars_3.find(BDD_ID_1)));
}

TEST (ManagerTest_ITE, ManagerShouldCreateNodeFromOperation) { 
    ClassProject::Manager *manager = new Manager("TestManager");
    BDD_ID varA_ID = manager->createVar("a");
    BDD_ID varB_ID = manager->createVar("b");
    BDD_ID varC_ID = manager->createVar("c");
    BDD_ID varD_ID = manager->createVar("d");

    // f = (A or B) and (C and D) 
    BDD_ID AorB_ID = manager->ite(varA_ID, 1, varB_ID);
    std::cout << "========== A or B END    ===========\n";
    BDD_ID CandD_ID = manager->ite(varC_ID, varD_ID, BDD_ID_0);
    std::cout << "========== C and D END   ===========\n";
    BDD_ID AND_ID = manager->ite(AorB_ID, CandD_ID, 0);
    std::cout << "========== Final AND END ===========\n";

    EXPECT_EQ(6,AorB_ID);
    EXPECT_EQ(7,CandD_ID);
    EXPECT_EQ(9,AND_ID);
    EXPECT_EQ(10, manager->uniqueTableSize());
}

TEST (ManagerTest_ITE, ManagerShouldThrowExceptionForUnkwnownNode) { 
    ClassProject::Manager *manager = new Manager("TestManager");
    BDD_ID varA_ID = manager->createVar("a");
    BDD_ID varB_ID = manager->createVar("b");

    EXPECT_THROW(manager->ite(varA_ID, varB_ID, 100), std::invalid_argument);
    EXPECT_THROW(manager->ite(100, varB_ID, varA_ID), std::invalid_argument);
    EXPECT_THROW(manager->ite(varA_ID, 100, varB_ID), std::invalid_argument);
}

TEST (ManagerTest_Cofactor, ManagerShouldSolveCofactorTrue) { 
    ClassProject::Manager *manager = new Manager("TestManager");
    BDD_ID varA_ID = manager->createVar("a");
    BDD_ID varB_ID = manager->createVar("b");

    BDD_ID AandB_ID = manager->ite(varA_ID, varB_ID, BDD_ID_0);
    EXPECT_EQ(BDD_ID_0, manager->coFactorTrue(BDD_ID_0, BDD_ID_0));
    EXPECT_EQ(BDD_ID_1, manager->coFactorTrue(BDD_ID_1, BDD_ID_1));
    EXPECT_EQ(varA_ID, manager->coFactorTrue(varA_ID, BDD_ID_0));
    EXPECT_EQ(varA_ID, manager->coFactorTrue(varA_ID, BDD_ID_1));
    EXPECT_EQ(varB_ID, manager->coFactorTrue(AandB_ID, varA_ID));
}

TEST (ManagerTest_CofactorTrue, ManagerShouldSolveCofactorTrueForOneNode) { 
    ClassProject::Manager *manager = new Manager("TestManager");
    BDD_ID varA_ID = manager->createVar("a");
    BDD_ID varB_ID = manager->createVar("b");
    BDD_ID AandB_ID = manager->ite(varA_ID, varB_ID, BDD_ID_0);

    EXPECT_EQ(BDD_ID_0, manager->coFactorTrue(BDD_ID_0));
    EXPECT_EQ(BDD_ID_1, manager->coFactorTrue(BDD_ID_1));
    EXPECT_EQ(BDD_ID_1, manager->coFactorTrue(varA_ID));
    EXPECT_EQ(varB_ID, manager->coFactorTrue(AandB_ID));
}

TEST (ManagerTest_Cofactor, ManagerShouldSolveCofactorFalse) { 
    ClassProject::Manager *manager = new Manager("TestManager");
    BDD_ID varA_ID = manager->createVar("a");
    BDD_ID varB_ID = manager->createVar("b");

    BDD_ID AandB_ID = manager->ite(varA_ID, varB_ID, BDD_ID_0);
    EXPECT_EQ(BDD_ID_0, manager->coFactorFalse(BDD_ID_0, BDD_ID_0));
    EXPECT_EQ(BDD_ID_1, manager->coFactorFalse(BDD_ID_1, BDD_ID_1));
    EXPECT_EQ(varA_ID, manager->coFactorFalse(varA_ID, BDD_ID_0));
    EXPECT_EQ(varA_ID, manager->coFactorFalse(varA_ID, BDD_ID_1));
    EXPECT_EQ(BDD_ID_0, manager->coFactorFalse(AandB_ID, varA_ID));
}

TEST (ManagerTest_Operation, ManagerShouldSolveAndOperation) { 
    ClassProject::Manager *manager = new Manager("TestManager");
    BDD_ID varA_ID = manager->createVar("a");
    BDD_ID varB_ID = manager->createVar("b");

    // Verify same result using ite() & and2(). Same operation 
    // should not be added again to the Unique table
    BDD_ID AandB_ID = manager->and2(varA_ID, varB_ID);
    EXPECT_EQ(5, manager->uniqueTableSize());
    BDD_ID AandB_ID_2 = manager->ite(varA_ID, varB_ID, BDD_ID_0);
    EXPECT_EQ(5, manager->uniqueTableSize()); // just to check it didn't add same operation

    EXPECT_EQ(AandB_ID, AandB_ID_2);

    //Isomorphic Graph shouldn't be added to Uniquetable
    AandB_ID_2 = manager->and2(varB_ID, varA_ID);
    EXPECT_EQ(AandB_ID, AandB_ID_2);
    EXPECT_EQ(5, manager->uniqueTableSize());
}

TEST (ManagerTest_Operation, ManagerShouldSolveOrOperation) { 
    ClassProject::Manager *manager = new Manager("TestManager");
    BDD_ID varA_ID = manager->createVar("a");
    BDD_ID varB_ID = manager->createVar("b");

    BDD_ID AorB_ID = manager->or2(varA_ID, varB_ID);
    BDD_ID AorB_ID_2 = manager->ite(varA_ID, BDD_ID_1,varB_ID);

    EXPECT_EQ(AorB_ID, AorB_ID_2);
}

TEST (ManagerTest_Operation, ManagerShouldSolveNegOperation) { 
    ClassProject::Manager *manager = new Manager("TestManager");
    BDD_ID varA_ID = manager->createVar("a");
    BDD_ID varB_ID = manager->createVar("b");
    BDD_ID varC_ID = manager->createVar("c");

    EXPECT_EQ(5, manager->neg(varA_ID));
    EXPECT_EQ(BDD_ID_1, manager->neg(BDD_ID_0));
    EXPECT_EQ(BDD_ID_0, manager->neg(BDD_ID_1));

    BDD_ID AandB_ID = manager->and2(varA_ID, varB_ID);
    BDD_ID negAndB_ID = manager->neg(AandB_ID);
    EXPECT_EQ(8, negAndB_ID);

    EXPECT_EQ(varA_ID, manager->topVar(negAndB_ID));
    manager->neg(manager->or2(varA_ID,manager->and2(varB_ID,manager->neg(varC_ID))));
    manager->printTable();
}

TEST (ManagerTest_Operation, ManagerShouldSolveXorOperation) { 
    ClassProject::Manager *manager = new Manager("TestManager");
    BDD_ID varA_ID = manager->createVar("a");
    BDD_ID varB_ID = manager->createVar("b");

    EXPECT_TRUE(manager->xor2(BDD_ID_0, BDD_ID_1));
    EXPECT_TRUE(manager->xor2(BDD_ID_1, BDD_ID_0));
    EXPECT_FALSE(manager->xor2(BDD_ID_0, BDD_ID_0));
    EXPECT_FALSE(manager->xor2(BDD_ID_1, BDD_ID_1));
    EXPECT_EQ(10, manager->xor2(varA_ID, varB_ID));
}

TEST (ManagerTest_Operation, ManagerShouldSolveNorOperation) { 
    ClassProject::Manager *manager = new Manager("TestManager");
    BDD_ID varA_ID = manager->createVar("a");
    BDD_ID varB_ID = manager->createVar("b");


    EXPECT_FALSE(manager->nor2(BDD_ID_0, BDD_ID_1));
    EXPECT_FALSE(manager->nor2(BDD_ID_1, BDD_ID_0));
    EXPECT_TRUE(manager->nor2(BDD_ID_0, BDD_ID_0));
    EXPECT_FALSE(manager->nor2(BDD_ID_1, BDD_ID_1));
    EXPECT_EQ(6, manager->nor2(varA_ID, varB_ID));
    manager->printTable();
}

TEST (ManagerTest_Operation, ManagerShouldSolveNandOperation) { 
    ClassProject::Manager *manager = new Manager("TestManager");
    BDD_ID varA_ID = manager->createVar("a");
    BDD_ID varB_ID = manager->createVar("b");

    EXPECT_TRUE(manager->nand2(BDD_ID_0, BDD_ID_1));
    EXPECT_TRUE(manager->nand2(BDD_ID_1, BDD_ID_0));
    EXPECT_TRUE(manager->nand2(BDD_ID_0, BDD_ID_0));
    EXPECT_FALSE(manager->nand2(BDD_ID_1, BDD_ID_1));
    EXPECT_EQ(6, manager->nand2(varA_ID, varB_ID));
}

int main(int argc, char* argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
