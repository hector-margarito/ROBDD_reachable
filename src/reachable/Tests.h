//
// Created by tobias on 29.11.18.
// Completed and Corrected by Hector Margarito and Bruno Pessanha
//

#ifndef VDSPROJECT_TESTS_H
#define VDSPROJECT_TESTS_H

#include "Reachable.h"
#include "gtest/gtest.h"
using namespace ClassProject;

TEST(managerTest, HowTo_Example) {
    ClassProject::Reachable comp(2);

    auto states = comp.getStates();
    std::vector<BDD_ID> functions;

    auto s0 = states.at(0);
    auto s1 = states.at(1);
    //s0' = not(s0)
    functions.push_back(comp.neg(s0));
    //s1' = not(s1)
    functions.push_back(comp.neg(s1));
    //Add transition functions
    comp.setDelta(functions);
    //Add init state
    comp.setInitState({false,false});

    ASSERT_TRUE(comp.is_reachable({false,false}));
    ASSERT_TRUE(comp.is_reachable({true,true}));
    ASSERT_FALSE(comp.is_reachable({true,false}));
    ASSERT_FALSE(comp.is_reachable({false,true}));
}

TEST(managerTest, More_states) {
    ClassProject::Reachable comp(5);

    auto states = comp.getStates();
    std::vector<BDD_ID> functions;

    auto s0 = states.at(0);
    auto s1 = states.at(1);
    auto s2 = states.at(2);
    auto s3 = states.at(3);
    auto s4 = states.at(4);
    //s0' = not(s0)
    functions.push_back(comp.neg(s0));
    //s1' = not(s1)
    functions.push_back(comp.neg(s1));
    //s2' = (s2)
    functions.push_back(s2);
    //s3' = not(s3)
    functions.push_back(comp.neg(s3));
    //s4' = not(s4)
    functions.push_back(comp.neg(s4));
    //Add transition functions
    comp.setDelta(functions);
    //Add init state
    comp.setInitState({false,false,true,false,false});

    ASSERT_TRUE(comp.is_reachable({false,false,true,false,false}));
    ASSERT_TRUE(comp.is_reachable({true,true,true,true,true}));
    ASSERT_FALSE(comp.is_reachable({true,false,true,false,false}));
    ASSERT_FALSE(comp.is_reachable({false,true,true,false,false}));
    ASSERT_FALSE(comp.is_reachable({true,true,false,true,true}));
    ASSERT_FALSE(comp.is_reachable({false,false,false,false,false}));
}

TEST(managerTest, exceptionErrorCheck_wrongComputeStatesSize) {
    ClassProject::Reachable comp(2);

    auto states = comp.getStates();
    std::vector<BDD_ID> functions;

    auto s0 = states.at(0);
    auto s1 = states.at(1);
    //s0' = not(s0)
    functions.push_back(comp.neg(s0));
    //s1' = not(s1)
    functions.push_back(comp.neg(s1));

    comp.setDelta(functions);
    comp.setInitState({false,false});

    //Add transition functions
    EXPECT_ANY_THROW(comp.is_reachable({true, true, true}));
}

TEST(managerTest, exceptionErrorCheck_wrongTransitionFunctionSize) {
    ClassProject::Reachable comp(2);

    auto states = comp.getStates();
    std::vector<BDD_ID> functions;

    auto s0 = states.at(0);
    auto s1 = states.at(1);
    //s0' = not(s0)
    functions.push_back(comp.neg(s0));
    //s1' = not(s1)
    functions.push_back(comp.neg(s1));
    //s1' = not(s1)
    functions.push_back(comp.neg(s1));
    //Add transition functions
    EXPECT_ANY_THROW(comp.setDelta(functions));
}

TEST(managerTest, exceptionErrorCheck_wrongInitialStateSize) {
    ClassProject::Reachable comp(2);

    auto states = comp.getStates();
    std::vector<BDD_ID> functions;

    auto s0 = states.at(0);
    auto s1 = states.at(1);
    //s0' = not(s0)
    functions.push_back(comp.neg(s0));
    //s1' = not(s1)
    functions.push_back(comp.neg(s1));
    //Add transition functions
    EXPECT_ANY_THROW(comp.setInitState({true,true,false}));
}

TEST(managerTest, exceptionErrorCheck_noInitState) {
    ClassProject::Reachable comp(2);

    auto states = comp.getStates();
    std::vector<BDD_ID> functions;

    auto s0 = states.at(0);
    auto s1 = states.at(1);
    //s0' = not(s0)
    functions.push_back(comp.neg(s0));
    //s1' = not(s1)
    functions.push_back(comp.neg(s1));
    //Add transition functions
    comp.setDelta(functions);

    EXPECT_ANY_THROW(comp.is_reachable({true,true}));
}

TEST(managerTest, exceptionErrorCheck_noDelta) {
    ClassProject::Reachable comp(2);

    auto states = comp.getStates();
    std::vector<BDD_ID> functions;

    auto s0 = states.at(0);
    auto s1 = states.at(1);
    //s0' = not(s0)
    functions.push_back(comp.neg(s0));
    //s1' = not(s1)
    functions.push_back(comp.neg(s1));
    //Add init state
    comp.setInitState({false,false});

    EXPECT_ANY_THROW(comp.is_reachable({true,true}));
}

TEST(managerTest, More_than_2_states) {
    ClassProject::Reachable comp(3);

    auto states = comp.getStates();
    std::vector<BDD_ID> functions;

    auto s0 = states.at(0);
    auto s1 = states.at(1);
    auto s2 = states.at(2);
    //s0' = not(s0)
    functions.push_back(comp.neg(s0));
    //s1' = not(s1)
    functions.push_back(comp.neg(s1));
    //s2' = not(s2)
    functions.push_back(comp.neg(s2));
    //Add transition functions
    comp.setDelta(functions);
    //Add init state
    comp.setInitState({false,false,false});

    ASSERT_TRUE(comp.is_reachable({true,true,true}));
    ASSERT_TRUE(comp.is_reachable({false,false,false}));
    ASSERT_FALSE(comp.is_reachable({true,true,false}));
    ASSERT_FALSE(comp.is_reachable({true,false,true}));
    ASSERT_FALSE(comp.is_reachable({false,true,true}));
}


#endif //VDSPROJECT_TESTS_H
