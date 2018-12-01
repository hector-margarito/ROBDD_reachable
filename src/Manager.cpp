#include <cassert>

#include "Manager.h"

using namespace ClassProject;

Manager::Manager(std::string name) : name(name) {
    bdd_count = 0;
    createVar("0");
    createVar("1");
}

BDD_ID Manager::createNode(const std::string &label, const BDD_ID top_var, const BDD_ID high, const BDD_ID low) {
    BDD_Node_t node;
    node.label = label;
    node.bdd = bdd_count;
    node.topvar = top_var;
    node.high  = high;
    node.low = low;
    uniqueTable.insert(std::make_pair(node.bdd,node));
    bdd_count++;
    return node.bdd;
}

BDD_ID Manager::createVar(const std::string &label) {
    return createNode(label, bdd_count, BDD_ID_1, BDD_ID_0);
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
    BDD_ID id_result;
    // check first if the IDs are in the terminal
    if (hasKey(i) && hasKey(t) && hasKey(e)){
        // Check first for terminal case
        if (!isTerminal(i,t,e)){
            if (computedTable.find(std::make_tuple(i,t,e)) == computedTable.end()){
                BDD_ID top_var_x = std::min(std::min(topVar(i), topVar(t)), topVar(e));
                BDD_ID r_high = t;
                BDD_ID r_low = e;

                if (r_high != r_low) {
                    id_result = findOrAddUniqueTable(top_var_x, r_high, r_low);
                } else { 
                    id_result = r_high;
                }
            } else {
                id_result = computedTable.at(std::make_tuple(i, t, e));
            }
        } else {
            id_result = i;
        }
    } else {
        // do we nedd to add to the table?
        // Mostlikely an exception
        //
    }
    return id_result;
}

BDD_ID Manager::findOrAddUniqueTable(const BDD_ID top_var, const BDD_ID high, const BDD_ID low) {
    computed_key_t node_computed_key = std::make_tuple(top_var, high, low); 
    if (computedTable.find(node_computed_key) == computedTable.end()) {
        BDD_ID new_ID = createNode("", top_var, high, low);
        computedTable.insert(std::make_pair(node_computed_key, new_ID));
        return new_ID;
    }
    return computedTable.at(node_computed_key);
}

bool Manager::isTerminal(const BDD_ID i, const BDD_ID t, const BDD_ID e) {
    bool result = false;

    if ((i == BDD_ID_1 ) ||
       (i == BDD_ID_0 ) ||
       ((t == BDD_ID_1) && (e == BDD_ID_0)) ||
       (t == e)) {
        result = true;
    }

    return result;
}

bool Manager::hasKey(const BDD_ID id) {
    return (uniqueTable.end() != uniqueTable.find(id));
}

BDD_ID Manager::coFactorTrue(const BDD_ID f, BDD_ID x) {
    return -1;
}

BDD_ID Manager::coFactorFalse(const BDD_ID f, BDD_ID x) {
    return -1;
}

BDD_ID Manager::coFactorTrue(const BDD_ID f) {
    return -1;
}

BDD_ID Manager::coFactorFalse(const BDD_ID f) {
    return -1;
}
