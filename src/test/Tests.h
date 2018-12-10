#ifndef VDS_PROJECT_GTESTS_H
#define VDS_PROJECT_GTESTS_H

#include "gtest/gtest.h"
#include "../Manager.h"

using namespace ClassProject;

class ManagerTest: public ::testing::Test { 

public: 
    ManagerTest();

    void SetUp();

    void TearDown();

    ~ManagerTest();

 protected:
    Manager *manager;  
};


#endif //VDS_PROJECT_TESTS_H

