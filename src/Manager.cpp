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

BDD_ID Manager::getMin(const BDD_ID x, const BDD_ID y) {
    bool x_isConstant = isConstant(x);
    bool y_isConstant = isConstant(y);
    if (!x_isConstant && !y_isConstant)
        return std::min(x,y);
    if (x_isConstant)
        return y;
    return x;
}

BDD_ID Manager::ite(const BDD_ID i, const BDD_ID t, const BDD_ID e){
    BDD_ID id_result;

    std::cout << "Manager::ite( " << i << ", " << t << ", " << e << ")" << std::endl;
    // check first if the IDs are in the terminal
    if (hasKey(i) && hasKey(t) && hasKey(e)){
        // Check first for terminal case
        if (!isTerminal(i,t,e)){
            if (computedTable.find(std::make_tuple(i,t,e)) == computedTable.end()){
                BDD_ID top_var_x = getMin(getMin(topVar(i), topVar(t)), topVar(e));
                std::cout << "ITE: top_var_x: " << top_var_x << "\n";
                BDD_ID r_high = coFactorTrue(t,top_var_x);
                BDD_ID r_low = coFactorFalse(e,top_var_x);
                std::cout << "r_high: " << r_high << "\n";
                std::cout << "r_low: " << r_low << "\n";

                if (r_high != r_low) {
                    id_result = findOrAddUniqueTable(top_var_x, r_high, r_low);
                } else { 
                    id_result = r_high;
                    std::cout << "R_HIGH: " << r_high << " Manager::ite( " << i << ", " << t << ", " << e << ")" << std::endl;
                }
            } else {
                std::cout << "GET FROM UNIQUE TABLE: Manager::ite( " << i << ", " << t << ", " << e << ")" << std::endl;
                id_result = computedTable.at(std::make_tuple(i, t, e));
            }
        } else {
            std::cout << "TERMINAL CASE: Manager::ite( " << i << ", " << t << ", " << e << ")" << std::endl;
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
    return isConstant(i) || ((t == BDD_ID_1) && (e == BDD_ID_0)) || (t == e);
}

bool Manager::isTerminal(const BDD_ID f, const BDD_ID x) {
    return isConstant(f) || isConstant(x) || topVar(f) > x;
}

bool Manager::hasKey(const BDD_ID id) {
    return (uniqueTable.end() != uniqueTable.find(id));
}

BDD_ID Manager::coFactorTrue(const BDD_ID f, BDD_ID x) {
    BDD_ID result_id;
    std::cout << "Manager::coFactorTrue\n f: " << f << "  x: " << x << "\n";
    if (!isTerminal(f,x)) {
        BDD_Node_t node_f = uniqueTable.at(f);
        std::cout << "node_f: " << node_f.bdd << "\n";
        if (topVar(f) != x) {
            BDD_ID co_high = coFactorTrue(node_f.high, x);
            std::cout << "co_high: " << co_high << "\n";
            BDD_ID co_low = coFactorTrue(node_f.low, x);
            std::cout << "co_low: " << co_low << "\n";
            std::cout << "node_f.topvar: " << node_f.topvar << "\n";
            result_id = ite(node_f.topvar, co_high, co_low);
            std::cout << "result_id: " << result_id << "\n";
        } else {
            result_id = coFactorTrue(node_f.high);
        }
    } else {
        result_id = f;
    }
    return result_id;
}

BDD_ID Manager::coFactorFalse(const BDD_ID f, BDD_ID x) {
    BDD_ID result_id;
    std::cout << "Manager::coFactorFalse\n f: " << f << "  x: " << x << "\n";
    if (!isTerminal(f,x)) {
        BDD_Node_t node_f = uniqueTable.at(f);
        std::cout << "node_f: " << node_f.bdd << "\n";
        if (topVar(f) != x) {
            BDD_ID co_high = coFactorFalse(node_f.high, x);
            std::cout << "co_high: " << co_high << "\n";
            BDD_ID co_low = coFactorFalse(node_f.low, x);
            std::cout << "co_low: " << co_low << "\n";
            std::cout << "node_f.topvar: " << node_f.topvar << "\n";
            result_id = ite(node_f.topvar, co_high, co_low);
            std::cout << "result_id: " << result_id << "\n";
        } else {
            result_id = coFactorFalse(node_f.low);
        }
    } else {
        result_id = f;
    }
    return result_id;
}

BDD_ID Manager::coFactorTrue(const BDD_ID f) {
    BDD_ID result_id;
    std::cout << "Manager::coFactor\n f" << f << "\n"; 
    if (!isConstant(f)) {
        BDD_Node_t node_f = uniqueTable.at(f);
        std::cout << "node_f.id: " << f << "\n"; 
        BDD_ID co_high = coFactorTrue(node_f.high);
        BDD_ID co_low = coFactorTrue(node_f.low);
        result_id = ite(node_f.topvar, co_high, co_low);
    } else {
        result_id = f; 
    }
    return result_id;
}

BDD_ID Manager::coFactorFalse(const BDD_ID f) {
    BDD_ID result_id;
    std::cout << "Manager::coFactor\n f" << f << "\n"; 
    if (!isConstant(f)) {
        BDD_Node_t node_f = uniqueTable.at(f);
        std::cout << "node_f.id: " << f << "\n"; 
        BDD_ID co_high = coFactorFalse(node_f.high);
        BDD_ID co_low = coFactorFalse(node_f.low);
        result_id = ite(node_f.topvar, co_high, co_low);
    } else {
        result_id = f; 
    }
    return result_id;
}
