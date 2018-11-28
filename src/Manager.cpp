#include <cassert>

#include "Manager.h"

using namespace ClassProject;

Manager::Manager(std::string name) : name(name) {
    bdd_count = 0;

    BDD_Node_t node_0;
    node_0.label = "0";
    node_0.bdd = bdd_count;
    uniqueTable.insert(std::make_pair(node_0.bdd,node_0));
    bdd_count++;

    BDD_Node_t node_1;
    node_1.label = "1";;
    node_1.bdd = bdd_count;
    uniqueTable.insert(std::make_pair(node_1.bdd,node_1));
    bdd_count++;
}

BDD_ID Manager::createVar(const std::string &label) {
    return 0;
}

size_t Manager::uniqueTableSize() {
    return uniqueTable.size();
}

