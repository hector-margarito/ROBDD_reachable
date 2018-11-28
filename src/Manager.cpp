#include <cassert>

#include "Manager.h"

using namespace ClassProject;

Manager::Manager(std::string name) : name(name) {
    bdd_count = 0;
    createVar("0");
    createVar("1");
}

BDD_ID Manager::createVar(const std::string &label) {
    BDD_Node_t node;
    node.label = label;
    node.bdd = bdd_count;
    uniqueTable.insert(std::make_pair(node.bdd,node));
    bdd_count++;
    return node.bdd;
}

size_t Manager::uniqueTableSize() {
    return uniqueTable.size();
}

