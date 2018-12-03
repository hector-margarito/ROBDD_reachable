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
#include <tuple>
#include <functional>
#include "ManagerInterface.h"

namespace ClassProject {
    typedef std::tuple<BDD_ID,BDD_ID,BDD_ID> computed_key_t;
    struct key_hash : public std::unary_function<computed_key_t, std::size_t> {
       std::size_t operator()(const computed_key_t& k) const
       {
          return std::get<0>(k) ^ std::get<1>(k) ^ std::get<2>(k);
       }
    };

    struct key_equal : public std::binary_function<computed_key_t, computed_key_t, bool>
    {
       bool operator()(const computed_key_t& v0, const computed_key_t& v1) const
       {
          return (
                   std::get<0>(v0) == std::get<0>(v1) &&
                   std::get<1>(v0) == std::get<1>(v1) &&
                   std::get<2>(v0) == std::get<2>(v1)
                 );
       }
    };

    const BDD_ID BDD_ID_0 = 0;
    const BDD_ID BDD_ID_1 = 1;

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

            std::string getTopVarName(const BDD_ID &root);

            void findVars(const BDD_ID &root, std::set<BDD_ID> &vars_of_root);

            void findNodes(const BDD_ID &root, std::set<BDD_ID> &nodes_of_root);

            size_t uniqueTableSize();

            BDD_ID ite(const BDD_ID i, const BDD_ID t, const BDD_ID e);

            BDD_ID coFactorTrue(const BDD_ID f, const BDD_ID x);

            BDD_ID coFactorFalse(const BDD_ID f, const BDD_ID x);

            BDD_ID coFactorTrue(const BDD_ID f);

            BDD_ID coFactorFalse(const BDD_ID f);
            
            BDD_ID and2(const BDD_ID a, const BDD_ID b);

            BDD_ID or2(const BDD_ID a, const BDD_ID b);

            BDD_ID xor2(const BDD_ID a, const BDD_ID b);

            BDD_ID neg(const BDD_ID a);

            BDD_ID nand2(const BDD_ID a, const BDD_ID b);

            BDD_ID nor2(const BDD_ID a, const BDD_ID b);

        private:

            bool isTerminal(const BDD_ID i, const BDD_ID t, const BDD_ID e);
            bool isTerminal(const BDD_ID f, const BDD_ID x);
            bool hasKey(const BDD_ID id);
            BDD_ID createNode(const std::string &label, const BDD_ID top_var, const  BDD_ID high, const BDD_ID low);
            BDD_ID findOrAddUniqueTable(const BDD_ID top_var, const BDD_ID high, const BDD_ID low);
            BDD_ID getMin(const BDD_ID x, const BDD_ID y);

            std::string name;
            BDD_ID bdd_count;
            std::unordered_map<BDD_ID,BDD_Node_t> uniqueTable;
            std::unordered_map<computed_key_t,BDD_ID,key_hash,key_equal> computedTable;
            const BDD_ID BDD_ID_False = 0;
            const BDD_ID BDD_ID_True = 1;
    };
}
#endif

// Local Variables:
// mode: c++
// End:
