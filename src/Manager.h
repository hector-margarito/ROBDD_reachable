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
#include <sstream>
#include "ManagerInterface.h"

/** @brief ClassProject contains all of the definitions for BDD
 * 
 */
namespace ClassProject {

    typedef std::tuple<BDD_ID,BDD_ID,BDD_ID> computed_key_t;
    struct key_hash : public std::unary_function<computed_key_t, std::size_t> {
       std::size_t operator()(const computed_key_t& k) const {
          return std::get<0>(k) ^ std::get<1>(k) ^ std::get<2>(k);
       }
    };

    struct key_equal : public std::binary_function<computed_key_t, computed_key_t, bool> {
       bool operator()(const computed_key_t& v0, const computed_key_t& v1) const {
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

    /** @class Manager class implements ManagerInterface that was provided.
     *
     */
    class Manager : public ManagerInterface {
        public:
            /** @brief Constructor of Manager
             *
             *  @details During construction leaf 0 and 1 are created
             */
            Manager(std::string name);

            /** @brief Creates a new variable for the BDD
             *
             *  @param[in] label - label of the new variable 
             *  @return - #BDD_ID of the newly created variable 
             */
            BDD_ID createVar(const std::string &label);

            /** @brief Returns the ID representing True
             *
             *  @return - the #BDD_ID representing True 
             */
            const BDD_ID &True();

            /** @brief Returns the ID representing False
             *
             *  @return - the #BDD_ID representing False
             */
            const BDD_ID &False();

            /** @brief Returns true if 'f' is a leaf node
             *
             *  @param[in] f - Node ID to check
             *  @return - true if 'f' is a leaf node
             */
            bool isConstant(const BDD_ID f);

            /** @brief Returns true if 'x' is a variable
             *
             *  @param[in] x - Node ID to check
             *  @return - true if 'x' is a variable
             */
            bool isVariable(const BDD_ID x);

            /** @brief Returns the ID of top variable of the BDD node f
             *
             *  @param[in] f - Node ID to check
             *  @return - #BDD_ID of top variable of the BDD node f
             *  */
            BDD_ID topVar(const BDD_ID f);

            /** @brief Return the name of top variable of the BDD node root
             *
             *  @param[in] root - Node ID to check
             *  @return - name of the of the top variable of the specified root node
             *  */
            std::string getTopVarName(const BDD_ID &root);

            /** @brief Returns the set of variables which are either top variable of the BDD node root or the reachable nodes from root.
             *
             *  @param[in] root - Node root to check
             *  @param[out] vars_of_root - set of variables that are top
             *  variable or reachable node
             *  */
            void findVars(const BDD_ID &root, std::set<BDD_ID> &vars_of_root);

            /** @brief Returns the set of BDD nodes which are reachable from the BDD node root(including itself)
             * 
             *  @param[in] root - Node root to check
             *  @param[out] nodes_of_root - set of nodes which are reable from
             *  the BDD node root
             *  */
            void findNodes(const BDD_ID &root, std::set<BDD_ID> &nodes_of_root);

            /** @brief Returns the number of the nodes currently exist in the unique table of the Manager class
             *
             * @return - number of nodes
             */
            size_t uniqueTableSize();

            /** @brief Implements the if-then-else algorithm
             *
             *  @param[in] i - Node i of the if-then-else algorithm
             *  @param[in] t - Node t of the if-then-else algorithm
             *  @param[in] e - Node e of the if-then-else algorithm
             * @return - the #BDD_ID of the new node that represents the ITE
             */
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

            void printTable();

        private:

            bool isTerminal(const BDD_ID i, const BDD_ID t, const BDD_ID e);
            bool isTerminal(const BDD_ID f, const BDD_ID x);
            bool hasKey(const BDD_ID id);
            BDD_ID createNode(const std::string &label, const BDD_ID top_var, const  BDD_ID high, const BDD_ID low);
            BDD_ID findOrAddUniqueTable(const BDD_ID top_var, const BDD_ID high, const BDD_ID low);
            BDD_ID getMin(const BDD_ID x, const BDD_ID y);
            std::string getLabel(const BDD_ID top_var, const BDD_ID high, const BDD_ID low);

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
