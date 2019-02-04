//
// Created by tobias on 29.11.18.
//

#ifndef VDSPROJECT_TESTS_H
#define VDSPROJECT_TESTS_H

#include "Reachable.h"
#include "gtest/gtest.h"
using namespace ClassProject;

TEST(managerTest, HowTo_Example) {
    ClassProject::Reachable comp(3);

    auto states = comp.getStates();
    std::vector<BDD_ID> functions;

    auto s0 = states.at(0);
    auto s1 = states.at(1);
    auto s2 = states.at(1);
    //s0' = not(s0)
    functions.push_back(comp.neg(s0));
    //s1' = not(s1)
    functions.push_back(comp.neg(s1));
    functions.push_back(comp.neg(s2));
    //Add transition functions
    comp.setDelta(functions);
    //Add init state
    comp.setInitState({false,false,false});

    ASSERT_TRUE(comp.is_reachable({true,true,true}));
    ASSERT_TRUE(comp.is_reachable({false,false,false}));
    ASSERT_FALSE(comp.is_reachable({true,false,false}));
    ASSERT_FALSE(comp.is_reachable({false,true,true}));
}



#endif //VDSPROJECT_TESTS_H
