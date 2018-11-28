// A minimalistic BDD library, following Wolfgang Kunz lecture slides
//
// Written by Markus Wedler 2014

#ifndef mwBDD_H
#define mwBDD_H

#include <cassert>
#include <iostream>
#include <list>
#include <vector>
#include <unordered_map>
#include <set>
#include <string>
#include "ManagerInterface.h"

namespace ClassProject {

#define BDD_ID_0 0
#define BDD_ID_1 1

    typedef struct BDD_Node_t {
        std::string label;
        BDD_ID bdd;
        BDD_ID high;
        BDD_ID low;
        BDD_ID topvar;
    } BDD_Node_t;

    class Manager : public ManagerInterface {
        public:
            Manager(std::string name);

            BDD_ID createVar(const std::string &label);

            const BDD_ID &True();

            const BDD_ID &False();

            bool isConstant(const BDD_ID f);

            bool isVariable(const BDD_ID x);

            BDD_ID topVar(const BDD_ID f);

            size_t uniqueTableSize();

        private:
            std::string name;

            BDD_ID bdd_count;

            std::unordered_map<BDD_ID,BDD_Node_t> uniqueTable;

            const BDD_ID BDD_ID_False = 0;

            const BDD_ID BDD_ID_True = 1;

    };
}
#endif

// Local Variables:
// mode: c++
// End:
