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

    typedef struct BDD_Node_t {
        std::string label;
        BDD_ID bdd;
        BDD_ID high;
        BDD_ID low;
        std::string topvar;
    } BDD_Node_t;

    class Manager : public ManagerInterface {
        public:
            Manager(std::string name);

            BDD_ID createVar(const std::string &label);

            size_t uniqueTableSize();

        private:
            std::string name;

            BDD_ID bdd_count;

            std::unordered_map<BDD_ID,BDD_Node_t> uniqueTable;
    };
}
#endif

// Local Variables:
// mode: c++
// End:
