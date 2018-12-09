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

    /** Used for creating keys for our computed table */
    typedef std::tuple<BDD_ID,BDD_ID,BDD_ID> computed_key_t;

    /** A hash of a tuple of 3 elements to guarantee they are unique */
    struct key_hash : public std::unary_function<computed_key_t, std::size_t> {
       std::size_t operator()(const computed_key_t& k) const {
          return std::get<0>(k) ^ std::get<1>(k) ^ std::get<2>(k);
       }
    };

    /** Compare the elements of the tuple */
    struct key_equal : public std::binary_function<computed_key_t, computed_key_t, bool> {
       bool operator()(const computed_key_t& v0, const computed_key_t& v1) const {
           return (
            std::get<0>(v0) == std::get<0>(v1) &&
            std::get<1>(v0) == std::get<1>(v1) &&
            std::get<2>(v0) == std::get<2>(v1)
            );
       }
    };
    
    /** BDD_ID representing 0 leaf */
    const BDD_ID BDD_ID_0 = 0;
    /** BDD_ID representing 1 leaf */
    const BDD_ID BDD_ID_1 = 1;

    /** Structure that a BDD node will contain */
    typedef struct BDD_Node_t {
        std::string label; /**< label of the node */
        BDD_ID bdd; /**< bdd id of the node */
        BDD_ID high; /**< bdd high */
        BDD_ID low;  /**< bdd low */
        BDD_ID topvar; /**< top var variable */
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

            /**
             * @brief Returns the positive cofactor of the function defined by <B>f</B> with respect to function <B>x</B>. 
             * 
             * @details Example: coFactorFalse(node_ID_f, node_ID_X);
             * 
             * @param[in] f - ID of the node f
             * @param[in] x - ID of the node x
             * 
             * @return Returns the BDD_ID of the positive co-factor of node <B>f</B>
             * 
             */
            BDD_ID coFactorTrue(const BDD_ID f, const BDD_ID x);

            /**
             * @brief Returns the negative cofactor of the function defined by <B>f</B> with respect to function <B>x</B>. 
             * 
             * @details Example: coFactorFalse(node_ID_f, node_ID_X);
             * 
             * @param[in] f - ID of the node f
             * @param[in] x - ID of the node x
             * 
             * @return Returns the BDD_ID of the negative co-factor of node <B>f</B>
             * 
             */
            BDD_ID coFactorFalse(const BDD_ID f, const BDD_ID x);

            /**
             * @brief Returns the positive cofactor of the function defined by node <B>f</B>
             * 
             * @details Example: coFactorTrue(node_ID);
             * 
             * @param[in] f - ID of the node
             * 
             * @return Returns the BDD_ID of the positive co-factor of node <B>f</B>
             * 
             */
            BDD_ID coFactorTrue(const BDD_ID f);

            /**
             * @brief Returns the negative cofactor of the function defined by node <B>f</B>
             * 
             * @details Example: coFactorFalse(node_ID);
             * 
             * @param[in] f - ID of the node
             * 
             * @return Returns the BDD_ID of the negative co-factor of node <B>f</B>
             * 
             */
            BDD_ID coFactorFalse(const BDD_ID f);
            
            /**
             * @brief AND operation. The operation of <B>A AND B</B>.
             * 
             * If this node does not exist, creates a new node represeting it or retrieves the ID of the existing node.
             * 
             * @details Example: and2(node1_ID, node2_ID);
             * 
             * @param[in] a - ID of first node
             * @param[in] b - ID of second node
             * 
             * @return Returns the BDD_ID of the new OR node
             * 
             */
            BDD_ID and2(const BDD_ID a, const BDD_ID b);

            /**
             * @brief OR operation. The operation of <B>A OR B</B>
             * 
             * If this node does not exist, creates a new node represeting it or retrieves the ID of the existing node.
             * 
             * @details Example: or2(node1_ID, node2_ID);
             * 
             * @param[in] a - ID of first node
             * @param[in] b - ID of second node
             * 
             * @return Returns the BDD_ID of the new OR node
             * 
             */
            BDD_ID or2(const BDD_ID a, const BDD_ID b);

            /**
             * @brief Exclusive OR (aka XOR) operation. Using AND and OR, the following operation is executed: 
             * <B>(A AND !B) OR (!A AND B)</B>
             * 
             * If this node does not exist, creates a new node represeting it or retrieves the ID of the existing node.
             * 
             * @details Example: xor2(node1_ID, node2_ID);
             * 
             * @param[in] a - ID of first node
             * @param[in] b - ID of second node
             * 
             * @return Returns the BDD_ID of the new XOR node
             * 
             */
            BDD_ID xor2(const BDD_ID a, const BDD_ID b);

            /**
             * @brief NEG operation. The negation of node A (<B>!A</B>)
             * 
             * If this node does not exist, creates a new node represeting it or retrieves the ID of the existing node.
             * 
             * @details Example: neg(node_ID);
             * 
             * @param[in] a - ID of node to be negated
             * 
             * @return Returns the BDD_ID of the new negated node
             * 
             */
            BDD_ID neg(const BDD_ID a);

            /**
             * @brief NAND operation. The negation of <B>A AND B</B>
             * 
             * If this node does not exist, creates a new node represeting it or retrieves the ID of the existing node.
             * 
             * @details Example: nand2(node1_ID, node2_ID);
             * 
             * @param[in] a - ID of first node
             * @param[in] b - ID of second node
             * 
             * @return Returns the BDD_ID of the new NAND node
             * 
             */
            BDD_ID nand2(const BDD_ID a, const BDD_ID b);

            /**
             * @brief NOR operation. The negation of <B>A OR B</B>
             * 
             * If this node does not exist, creates a new node represeting it or retrieves the ID of the existing node.
             * 
             * @details Example: nor2(node1_ID, node2_ID);
             * 
             * @param[in] a - ID of first node
             * @param[in] b - ID of second node
             * 
             * @return Returns the BDD_ID of the new NOR node
             * 
             */
            BDD_ID nor2(const BDD_ID a, const BDD_ID b);

            /**
             * @brief Prints the content of the unique table for debugging purposes
             *
             */
            void printTable();

        private:

            /**
             * @brief Checks whether a node is terminal for ITE function
             * 
             * @param i - first node
             * @param t - then node
             * @param e - else node
             * @return true - when this is a terminal case for ITE
             * @return false - when it is not
             */
            bool isTerminal(const BDD_ID i, const BDD_ID t, const BDD_ID e);

            /**
             * @brief  Checkes whether a node is terminal for CoFactor functions
             * 
             * @param f 
             * @param x 
             * @return true - when this is a terminal case for CoFactor
             * @return false - when it is not
             */
            bool isTerminal(const BDD_ID f, const BDD_ID x);

            /**
             * @brief Checks whether the BDD has the key id in its unique table
             * 
             * @param id 
             * @return true 
             * @return false 
             */
            bool hasKey(const BDD_ID id);

            /**
             * @brief Creates a BDD Node
             * 
             * @param label 
             * @param top_var 
             * @param high 
             * @param low 
             * @return BDD_ID 
             */
            BDD_ID createNode(const std::string &label, const BDD_ID top_var, const  BDD_ID high, const BDD_ID low);

            /**
             * @brief Tries to finds a node with those parameters and adds it the the unique table when it is not found
             * 
             * @param top_var 
             * @param high 
             * @param low 
             * @return BDD_ID 
             */
            BDD_ID findOrAddUniqueTable(const BDD_ID top_var, const BDD_ID high, const BDD_ID low);

            /**
             * @brief Get the smallest node ID which is not a constant
             * 
             * @param x - node 1
             * @param y - node 2
             * @return BDD_ID - the ID of the smallest node
             */
            BDD_ID getMin(const BDD_ID x, const BDD_ID y);

            /**
             * @brief Get the Label of the node
             * 
             * @param top_var - top variable
             * @param high 
             * @param low 
             * @return std::string - the label
             */
            std::string getLabel(const BDD_ID top_var, const BDD_ID high, const BDD_ID low);

            /**
             * @brief Get the Terminal Case Id
             * 
             * @param i 
             * @param t 
             * @param e 
             * @return BDD_ID - the id of the terminal node
             */
            BDD_ID getTerminalCaseId(const BDD_ID i, const BDD_ID t, const BDD_ID e);

            /** Name of the manager */
            std::string name;

            /** The ID of the next BDD node */
            BDD_ID bdd_count;

            /** The unique table map used to keep the BDD nodes*/
            std::unordered_map<BDD_ID,BDD_Node_t> uniqueTable;

            /** The computed table map used to have a quick access to BDD_ID */
            std::unordered_map<computed_key_t,BDD_ID,key_hash,key_equal> computedTable;

            /** False */
            const BDD_ID BDD_ID_False = 0;

            /** True */
            const BDD_ID BDD_ID_True = 1;
    };
}
#endif