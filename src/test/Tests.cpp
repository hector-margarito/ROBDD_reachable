#include "Tests.h"

ManagerTest::ManagerTest() { 
    manager = new Manager("TestManager"); 
}

void ManagerTest::SetUp() { }

void ManagerTest::TearDown() { 
    delete manager; 
}

ManagerTest::~ManagerTest()  { }

TEST_F(ManagerTest, ManagerTableShouldHaveTwoNodes) { 
    EXPECT_EQ(2, manager->uniqueTableSize());
}

TEST_F(ManagerTest, ManagerTableShouldHaveThreeNodesAfterCreateVar) { 
    BDD_ID varId = manager->createVar("a");
    EXPECT_NE(BDD_ID_0, varId);
    EXPECT_NE(BDD_ID_1, varId);
    EXPECT_EQ(3, manager->uniqueTableSize());
}

TEST_F(ManagerTest, ManagerTableShouldHaveBooleanMethods) { 
    EXPECT_TRUE(manager->True());
    EXPECT_FALSE(manager->False());
}

TEST_F(ManagerTest, IsConstant_BDDNodeShouldBeAConstant) { 
    BDD_ID varId = manager->createVar("a");

    EXPECT_TRUE(manager->isConstant(BDD_ID_0));
    EXPECT_TRUE(manager->isConstant(BDD_ID_1));
    EXPECT_FALSE(manager->isConstant(varId));
}

TEST_F(ManagerTest, IsVariable_BDDNodeShouldBeAVariable) { 
    BDD_ID varId = manager->createVar("a");

    EXPECT_FALSE(manager->isVariable(BDD_ID_0));
    EXPECT_FALSE(manager->isVariable(BDD_ID_1));
    EXPECT_TRUE(manager->isVariable(varId));
}

TEST_F(ManagerTest, TopVar_BDDNodeShouldHaveTopVar) { 
    BDD_ID varA_ID = manager->createVar("a");
    BDD_ID varB_ID = manager->createVar("b");
    BDD_ID AandB_ID = manager->ite(varA_ID, varB_ID, BDD_ID_0);

    EXPECT_EQ(BDD_ID_0, manager->topVar(BDD_ID_0));
    EXPECT_EQ(BDD_ID_1, manager->topVar(BDD_ID_1));
    EXPECT_EQ(varA_ID, manager->topVar(varA_ID));
    EXPECT_EQ(varB_ID, manager->topVar(varB_ID));
    EXPECT_EQ(varA_ID, manager->topVar(AandB_ID));
}

TEST_F(ManagerTest, GetTopVarName_BDDNodeShouldHaveTopVarName) { 
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

TEST_F(ManagerTest, FindVars_ManagerShouldFindVariables) { 
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

TEST_F(ManagerTest, FindNodes_ManagerShouldFindNodes) { 
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

TEST_F(ManagerTest, ITE_ManagerShouldCreateNodeFromOperation) { 
    BDD_ID varA_ID = manager->createVar("a");
    BDD_ID varB_ID = manager->createVar("b");
    BDD_ID varC_ID = manager->createVar("c");
    BDD_ID varD_ID = manager->createVar("d");

    // f = (A or B) and (C and D) 
    BDD_ID AorB_ID = manager->ite(varA_ID, 1, varB_ID);
    BDD_ID CandD_ID = manager->ite(varC_ID, varD_ID, BDD_ID_0);
    BDD_ID AND_ID = manager->ite(AorB_ID, CandD_ID, 0);
    // Term cases check, no new nodes created
    BDD_ID TermCase_1 = manager->ite(1,varA_ID,varB_ID);
    BDD_ID TermCase_2 = manager->ite(0,varA_ID,varB_ID);
    BDD_ID TermCase_3 = manager->ite(varC_ID,varA_ID,varA_ID);
    BDD_ID TermCase_4 = manager->ite(varA_ID,BDD_ID_1,BDD_ID_0);

    EXPECT_EQ(6,AorB_ID);
    EXPECT_EQ(7,CandD_ID);
    EXPECT_EQ(9,AND_ID);
    EXPECT_EQ(10, manager->uniqueTableSize());
    EXPECT_EQ(varA_ID,TermCase_1);
    EXPECT_EQ(varB_ID,TermCase_2);
    EXPECT_EQ(varA_ID,TermCase_3);
    EXPECT_EQ(varA_ID,TermCase_4);
}

TEST_F(ManagerTest, ITE_ManagerShouldDoReduction) { 
    BDD_ID varA_ID = manager->createVar("a");
    BDD_ID varB_ID = manager->createVar("b");
    BDD_ID AandB_ID = manager->and2(varA_ID,varB_ID);
    BDD_ID AandNotB = manager->and2(varA_ID,manager->neg(varB_ID));
    BDD_ID or_ID = manager->or2(AandB_ID,AandNotB);

    EXPECT_EQ(varA_ID,or_ID);
}

TEST_F(ManagerTest, Cofactor_ManagerShouldSolveCofactorTrue) { 
    BDD_ID varA_ID = manager->createVar("a");
    BDD_ID varB_ID = manager->createVar("b");
    BDD_ID varC_ID = manager->createVar("c");

    BDD_ID AandB_ID = manager->ite(varA_ID, varB_ID, BDD_ID_0);
    EXPECT_EQ(BDD_ID_0, manager->coFactorTrue(BDD_ID_0, BDD_ID_0));
    EXPECT_EQ(BDD_ID_1, manager->coFactorTrue(BDD_ID_1, BDD_ID_1));
    EXPECT_EQ(varA_ID, manager->coFactorTrue(varA_ID, BDD_ID_0));
    EXPECT_EQ(varA_ID, manager->coFactorTrue(varA_ID, BDD_ID_1));
    EXPECT_EQ(varB_ID, manager->coFactorTrue(AandB_ID, varA_ID));
    EXPECT_EQ(AandB_ID, manager->coFactorTrue(AandB_ID, varC_ID));
}

TEST_F(ManagerTest, CofactorTrue_ManagerShouldSolveCofactorTrueForOneNode) { 
    BDD_ID varA_ID = manager->createVar("a");
    BDD_ID varB_ID = manager->createVar("b");
    BDD_ID AandB_ID = manager->ite(varA_ID, varB_ID, BDD_ID_0);

    EXPECT_EQ(BDD_ID_0, manager->coFactorTrue(BDD_ID_0));
    EXPECT_EQ(BDD_ID_1, manager->coFactorTrue(BDD_ID_1));
    EXPECT_EQ(BDD_ID_1, manager->coFactorTrue(varA_ID));
    EXPECT_EQ(varB_ID, manager->coFactorTrue(AandB_ID));
}

TEST_F(ManagerTest, Cofactor_ManagerShouldSolveCofactorFalse) { 
    BDD_ID varA_ID = manager->createVar("a");
    BDD_ID varB_ID = manager->createVar("b");

    BDD_ID AandB_ID = manager->ite(varA_ID, varB_ID, BDD_ID_0);
    EXPECT_EQ(BDD_ID_0, manager->coFactorFalse(BDD_ID_0, BDD_ID_0));
    EXPECT_EQ(BDD_ID_1, manager->coFactorFalse(BDD_ID_1, BDD_ID_1));
    EXPECT_EQ(varA_ID, manager->coFactorFalse(varA_ID, BDD_ID_0));
    EXPECT_EQ(varA_ID, manager->coFactorFalse(varA_ID, BDD_ID_1));
    EXPECT_EQ(BDD_ID_0, manager->coFactorFalse(AandB_ID, varA_ID));
}

TEST_F(ManagerTest, CofactorFalse_ManagerShouldSolveCofactorFalseForOneNode) { 
    BDD_ID varA_ID = manager->createVar("a");
    BDD_ID varB_ID = manager->createVar("b");
    BDD_ID AandB_ID = manager->ite(varA_ID, varB_ID, BDD_ID_0);

    EXPECT_EQ(BDD_ID_0, manager->coFactorFalse(BDD_ID_0));
    EXPECT_EQ(BDD_ID_1, manager->coFactorFalse(BDD_ID_1));
    EXPECT_EQ(BDD_ID_0, manager->coFactorFalse(varA_ID));
    EXPECT_EQ(BDD_ID_0, manager->coFactorFalse(AandB_ID));
}

TEST_F(ManagerTest, Operation_AND_ManagerShouldSolveAndOperation) { 
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

TEST_F(ManagerTest, Operation_OR_ManagerShouldSolveOrOperation) { 
    BDD_ID varA_ID = manager->createVar("a");
    BDD_ID varB_ID = manager->createVar("b");

    BDD_ID AorB_ID = manager->or2(varA_ID, varB_ID);
    BDD_ID AorB_ID_2 = manager->ite(varA_ID, BDD_ID_1,varB_ID);

    EXPECT_EQ(AorB_ID, AorB_ID_2);
}

TEST_F(ManagerTest, Operation_NEG_ManagerShouldSolveNegOperation) { 
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
    //manager->printTable();
}

TEST_F(ManagerTest, Operation_XOR_ManagerShouldSolveXorOperation) { 
    BDD_ID varA_ID = manager->createVar("a");
    BDD_ID varB_ID = manager->createVar("b");

    EXPECT_TRUE(manager->xor2(BDD_ID_0, BDD_ID_1));
    EXPECT_TRUE(manager->xor2(BDD_ID_1, BDD_ID_0));
    EXPECT_FALSE(manager->xor2(BDD_ID_0, BDD_ID_0));
    EXPECT_FALSE(manager->xor2(BDD_ID_1, BDD_ID_1));
    EXPECT_EQ(8, manager->xor2(varA_ID, varB_ID));
}

TEST_F(ManagerTest, Operation_NOR_ManagerShouldSolveNorOperation) { 
    BDD_ID varA_ID = manager->createVar("a");
    BDD_ID varB_ID = manager->createVar("b");


    EXPECT_FALSE(manager->nor2(BDD_ID_0, BDD_ID_1));
    EXPECT_FALSE(manager->nor2(BDD_ID_1, BDD_ID_0));
    EXPECT_TRUE(manager->nor2(BDD_ID_0, BDD_ID_0));
    EXPECT_FALSE(manager->nor2(BDD_ID_1, BDD_ID_1));
    EXPECT_EQ(6, manager->nor2(varA_ID, varB_ID));
    //manager->printTable();
}

TEST_F(ManagerTest, Operation_NAND_ManagerShouldSolveNandOperation) { 
    BDD_ID varA_ID = manager->createVar("a");
    BDD_ID varB_ID = manager->createVar("b");

    EXPECT_TRUE(manager->nand2(BDD_ID_0, BDD_ID_1));
    EXPECT_TRUE(manager->nand2(BDD_ID_1, BDD_ID_0));
    EXPECT_TRUE(manager->nand2(BDD_ID_0, BDD_ID_0));
    EXPECT_FALSE(manager->nand2(BDD_ID_1, BDD_ID_1));
    EXPECT_EQ(6, manager->nand2(varA_ID, varB_ID));
}
