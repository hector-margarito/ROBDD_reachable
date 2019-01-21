#include <cassert>

#include "Manager.h"
#include <unordered_map>

using namespace ClassProject;

Manager::Manager(){
    setInitValues();
}

Manager::Manager(std::string name) : name(name) {
    setInitValues();
}

void Manager::setInitValues() {
    bdd_count = 0;
    createNode("0", False(), False(), False());
    createNode("1", True(), True(), True());
}

BDD_ID Manager::createNode(const std::string &label, const BDD_ID top_var, const BDD_ID high, const BDD_ID low) {
    BDD_ID bdd_ID = bdd_count++;
    BDD_Node_t node;
    node.label = label;
    node.topvar = top_var;
    node.high  = high;
    node.low = low;
    uniqueTable.insert(std::make_pair(bdd_ID, node));
    computedTable.insert(std::make_pair(std::make_tuple(top_var, high, low), bdd_ID));
    return bdd_ID;
}

BDD_ID Manager::createVar(const std::string &label) {
    return createNode(label, bdd_count, True(), False());
}

size_t Manager::uniqueTableSize() {
    return uniqueTable.size();
}

const BDD_ID &Manager::False() {
    return BDD_ID_0;
}

const BDD_ID &Manager::True() {
    return BDD_ID_1;
}

bool Manager::isConstant(const BDD_ID id) {
    return id == BDD_ID_0 || id == BDD_ID_1;
}

bool Manager::isVariable(const BDD_ID id) {
    return !isConstant(id) && id == topVar(id);
}

BDD_ID Manager::topVar(const BDD_ID id) {
    return uniqueTable.at(id).topvar;
}

std::string Manager::getTopVarName(const BDD_ID &root) {
    return uniqueTable.at(topVar(root)).label;
}

void Manager::findVars(const BDD_ID &root, std::set<BDD_ID> &vars_of_root) {
    if (isVariable(root)) {
        vars_of_root.insert(root);
    } else if (!isConstant(root)) {
        BDD_Node_t rootNode = uniqueTable.at(root);
        if (vars_of_root.find(rootNode.topvar) == vars_of_root.end()) {
            vars_of_root.insert(rootNode.topvar);
        }
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
    if (isConstant(x))
        return y;

    if(isConstant(y))
        return x;

    return std::min(x,y);
}

BDD_ID Manager::ite(const BDD_ID i, const BDD_ID t, const BDD_ID e){
    BDD_ID id_result;

    // Check first for terminal case
    if (!isTerminal(i,t,e)){
        computed_key_t node_computed_key = std::make_tuple(i,t,e);
        if (computedTable.find(node_computed_key) == computedTable.end()){
            BDD_ID top_var_x = getMin(getMin(topVar(i), topVar(t)), topVar(e));

            BDD_Node_t node_x = uniqueTable.at(top_var_x);
            BDD_ID r_high = ite(coFactorTrue(i,top_var_x), coFactorTrue(t,top_var_x), coFactorTrue(e,top_var_x));
            BDD_ID r_low = ite(coFactorFalse(i,top_var_x), coFactorFalse(t,top_var_x), coFactorFalse(e,top_var_x));

            if (r_high != r_low) {
                id_result = findOrAddUniqueTable(top_var_x, r_high, r_low);
            } else { 
                id_result = r_high;
            }
        } else {
            id_result = computedTable.at(node_computed_key);
        }
    } else {
        id_result = getTerminalCaseId(i, t, e);
    }
    return id_result;
}

BDD_ID Manager::getTerminalCaseId(const BDD_ID i, const BDD_ID t, const BDD_ID e) {
    BDD_ID id_result = False();
    /* Check each terminal case to return the appropiate response */
    if (isConstant(i)) {
        if (True() == i) {
            id_result = t;
        } else {
            id_result = e;
        }
    } else if (t == e) {
        id_result = t;
    } else if (t == True() && e == False()) {
        id_result = i;
    }
    return id_result;
}

BDD_ID Manager::findOrAddUniqueTable(const BDD_ID top_var, const BDD_ID high, const BDD_ID low) {
    computed_key_t node_computed_key = std::make_tuple(top_var, high, low); 
    if (computedTable.find(node_computed_key) == computedTable.end()) {
        BDD_ID new_ID = createNode(getLabel(top_var, high, low), top_var, high, low);
        return new_ID;
    }
    return computedTable.at(node_computed_key);
}

std::string Manager::getLabel(const BDD_ID top_var, const BDD_ID high, const BDD_ID low) {
    std::stringstream sstm;
    sstm << top_var << "," << high << "," << low;
    return sstm.str();
}

bool Manager::isTerminal(const BDD_ID i, const BDD_ID t, const BDD_ID e) {
    return isConstant(i) || ((t == True()) && (e == False())) || (t == e);
}

bool Manager::isTerminal(const BDD_ID f, const BDD_ID x) {
    return isConstant(f) || isConstant(x) || topVar(f) > x;
}

bool Manager::hasKey(const BDD_ID id) {
    return (uniqueTable.end() != uniqueTable.find(id));
}

BDD_ID Manager::coFactorTrue(const BDD_ID f, BDD_ID x) {
    BDD_ID result_id;
    if (!isTerminal(f,x)) {
        BDD_Node_t node_f = uniqueTable.at(f);
        if (node_f.topvar != x) {
            BDD_ID co_high = coFactorTrue(node_f.high, x);
            BDD_ID co_low = coFactorTrue(node_f.low, x);
            result_id = ite(node_f.topvar, co_high, co_low);
        } else {
            result_id = node_f.high;
        }
    } else {
        result_id = f;
    }
    return result_id;
}

BDD_ID Manager::coFactorFalse(const BDD_ID f, BDD_ID x) {
    BDD_ID result_id;
    if (!isTerminal(f,x)) {
        BDD_Node_t node_f = uniqueTable.at(f);
        if (node_f.topvar != x) {
            BDD_ID co_high = coFactorFalse(node_f.high, x);
            BDD_ID co_low = coFactorFalse(node_f.low, x);
            result_id = ite(node_f.topvar, co_high, co_low);
        } else {
            result_id = node_f.low;
        }
    } else {
        result_id = f;
    }
    return result_id;
}

BDD_ID Manager::coFactorTrue(const BDD_ID f) {
    return coFactorTrue(f,topVar(f));
}

BDD_ID Manager::coFactorFalse(const BDD_ID f) {
    return coFactorFalse(f,topVar(f));
}

BDD_ID Manager::and2(const BDD_ID a, const BDD_ID b) {
    return ite(a,b,False());
}

BDD_ID Manager::or2(const BDD_ID a, const BDD_ID b) {
    return ite(a,True(),b);
}

BDD_ID Manager::xor2(const BDD_ID a, const BDD_ID b) {
    return ite(a,neg(b),b);
}

BDD_ID Manager::neg(const BDD_ID a) {
    BDD_ID result;

    if (isConstant(a)) {
        if (a == False()) {
            result = True();
        } else {
            result = False();
        }
    } else {
        result = ite(a, False(), True());
    }

    return result;
}

BDD_ID Manager::nand2(const BDD_ID a, const BDD_ID b) {
    return ite(a,neg(b),True());
}

BDD_ID Manager::nor2(const BDD_ID a, const BDD_ID b) {
    return ite(a,False(),neg(b));
}

void Manager::printTable() {
    std::unordered_map<BDD_ID,BDD_Node_t>::iterator it;
    std::cout << std::endl << "\t\t[ UNIQUE TABLE ]" << std::endl << std::endl;
    std::cout << "\t|\t"  << "LABEL"  << '\t' << "ID" << '\t' << "TOPVAR" << '\t' 
        << "HIGH" << '\t' << "LOW" << "\t|" << std::endl  << std::endl;

    for (it = uniqueTable.begin(); it != uniqueTable.end(); it++) {
        std::cout << "\t|\t" << it->second.label<< '\t' << it->first << '\t'<< it->second.topvar 
            << '\t'<< it->second.high<< '\t'<< it->second.low<< "\t|" << std::endl << std::endl;
    }
}
