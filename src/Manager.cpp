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
    node.topvar = bdd_count;
    node.high  = BDD_ID_1;
    node.low = BDD_ID_0;
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

bool Manager::isConstant(const BDD_ID id) {
    return id == BDD_ID_0 || id == BDD_ID_1;
}

bool Manager::isVariable(const BDD_ID id) {
    return !isConstant(id) && id == topVar (id);
}

BDD_ID Manager::topVar(const BDD_ID id) {
    return uniqueTable.at(id).topvar;
}

std::string Manager::getTopVarName(const BDD_ID &root) {
    return uniqueTable.at(root).label;
}

void Manager::findVars(const BDD_ID &root, std::set<BDD_ID> &vars_of_root) {
    if (isVariable(root)) {
        vars_of_root.insert(root);
    } else if (!isConstant(root)) {
        BDD_Node_t rootNode = uniqueTable.at(root);
        findVars(rootNode.high, vars_of_root);
        findVars(rootNode.low, vars_of_root);
    }
}

void Manager::findNodes(const BDD_ID &root, std::set<BDD_ID> &nodes_of_root) {
    nodes_of_root.insert(root);
    if (!isConstant(root)) {
        BDD_Node_t rootNode = uniqueTable.at(root);
        findNodes(rootNode.high, nodes_of_root);
        findNodes(rootNode.low, nodes_of_root);
    }
}

BDD_ID Manager::ite(const BDD_ID i, const BDD_ID t, const BDD_ID e){
    return -1;
}
