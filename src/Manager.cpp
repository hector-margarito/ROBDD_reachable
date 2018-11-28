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

const BDD_ID &Manager::False() {
    return BDD_ID_False;
}

const BDD_ID &Manager::True() {
    return BDD_ID_True;
}

bool Manager::isConstant(const BDD_ID f) {
    return false;
}

bool Manager::isVariable(const BDD_ID x) {
    return false;
}
