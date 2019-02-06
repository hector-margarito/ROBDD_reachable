//
// Created by ludwig on 27.11.18.
//

#include "Reachable.h"
#include <string>

using namespace ClassProject;

Reachable::Reachable(unsigned int stateSize) : ReachableInterface(stateSize) {
    this->stateSize = stateSize;
    this->states.reserve(stateSize*2);
    for (int i = 0; i < stateSize; i++) {
        this->states.push_back(createVar("S" + std::to_string(i)));
    }
    for (int i = 0; i < stateSize; i++) {
        this->states.push_back(createVar("S'" + std::to_string(i)));
    }
    // For error detection of no setInitState() before reachability check
    characteristic_S0 = -1;
}

Reachable::~Reachable() {}

BDD_ID Reachable::xnor2(BDD_ID a, BDD_ID b) { 
     return ite(a,b,neg(b));
}

const std::vector<BDD_ID> &Reachable::getStates() const {
    return states;
}

void Reachable::setDelta(const std::vector<BDD_ID> &transitionFunctions) {
    this->transitions = transitionFunctions;
}

void Reachable::setInitState(const std::vector<bool>& stateVector) {
    BDD_ID state = BDD_ID_1;
    characteristic_S0 = BDD_ID_1;

    for (int i = 0; i < stateSize; i++) {
        state = and2(state, stateVector[i] ? this->states[i] : neg(this->states[i]));
        characteristic_S0 = and2(characteristic_S0, xnor2(this->states[i], stateVector[i]));
    }
}

BDD_ID Reachable::computeTransitionRelations() {
    BDD_ID relations_tau = getTransitionRelation(0);
    for (int i = 1; i < stateSize; i++) {
        relations_tau = and2(relations_tau, getTransitionRelation(i));
    }
    return relations_tau;
}

BDD_ID Reachable::compute_reachable_states() {
    BDD_ID char_R;
    BDD_ID relations_tau = computeTransitionRelations();
    BDD_ID char_R_it = characteristic_S0;

    do {
        char_R = char_R_it;
        BDD_ID image = computeImage(relations_tau, char_R); 
        char_R_it = or2(char_R, image);
    } while (char_R != char_R_it);
    
    return char_R;
}

BDD_ID Reachable::computeImage(BDD_ID relations_tau, BDD_ID char_R) {
    BDD_ID img, img_prime;
    BDD_ID s_prime;
    BDD_ID temp1, temp2, temp3;

    /* Compute image of S primes */
    img_prime = and2(char_R, relations_tau);
    for (int i = 0; i < stateSize; i++) {
        img_prime = or2(coFactorTrue(img_prime, states[i]), coFactorFalse(img_prime, states[i]));
    }
    
    /* Compute image of S */
    temp1 = xnor2(states[0],states[stateSize]);
    for (int i = 1; i < stateSize; i++) {
        s_prime = states[stateSize + i];
        temp1 = and2(temp1, xnor2(states[i],s_prime));
        temp2 = and2(temp1, img_prime);
    }

    img = or2(coFactorTrue(temp2, states[stateSize]), coFactorFalse(temp2, states[stateSize]));
    for (int i = 1; i < stateSize; i++) {
        s_prime = states[stateSize + i];
        img = or2(coFactorTrue(img, s_prime), coFactorFalse(img, s_prime));
    }
    
    return img;
}

BDD_ID Reachable::getTransitionRelation(int index) {
    BDD_ID sPrime = this->states[stateSize + index];
    return or2(and2(sPrime, this->transitions[index]), and2(neg(sPrime), neg(this->transitions[index])));
}

bool Reachable::is_reachable(const std::vector<bool>& stateVector) {
    bool result = false;
    try{
        if(-1 == characteristic_S0)
            throw std::runtime_error("No init state defined");

        if(this->transitions.empty())
            throw std::runtime_error("No deltas defined");

        BDD_ID reachable = compute_reachable_states();
        for (int i = 0; i < stateSize; i++) {
            reachable = stateVector[i] ? 
                coFactorTrue(reachable, this->states[i]) : 
                coFactorFalse(reachable, this->states[i]);
        }
        result = (reachable == BDD_ID_1)? true: false;
    }
    catch(std::runtime_error &e){
        std::cout << "Caught a runtime_error exception: "
                  << e.what () << '\n';
        result = false;
    }
    return result;
}

