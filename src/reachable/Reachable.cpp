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
    BDD_ID img_SOS1, img_S0S1_prime;
    BDD_ID s0_prime, s1_prime;
    BDD_ID temp1, temp2, temp3;

    /* Compute image of S primes */
    img_S0S1_prime = and2(char_R, relations_tau);
    for (int i = 0; i < stateSize; i++) {
        img_S0S1_prime = or2(coFactorTrue(img_S0S1_prime, states[i]), coFactorFalse(img_S0S1_prime, states[i]));
    }
    
    /* Compute image of S */
    temp1 = xnor2(states[0],states[stateSize]);
    for (int i = 1; i < stateSize; i++) {
        s1_prime = states[stateSize + i];
        temp1 = and2(temp1, xnor2(states[i],s1_prime));
        temp2 = and2(temp1, img_S0S1_prime);
    }

    img_SOS1 = or2(coFactorTrue(temp2, states[stateSize]), coFactorFalse(temp2, states[stateSize]));
    for (int i = 1; i < stateSize; i++) {
        s0_prime = states[stateSize + i];
        img_SOS1 = or2(coFactorTrue(img_SOS1, s0_prime), coFactorFalse(img_SOS1, s0_prime));
    }
    
    return img_SOS1;
}

BDD_ID Reachable::getTransitionRelation(int index) {
    BDD_ID sPrime = this->states[stateSize + index];
    return or2(and2(sPrime, this->transitions[index]), and2(neg(sPrime), neg(this->transitions[index])));
}

bool Reachable::is_reachable(const std::vector<bool>& stateVector) {
    BDD_ID reachable = compute_reachable_states();
    for (int i = 0; i < stateSize; i++) {
        reachable = stateVector[i] ? 
            coFactorTrue(reachable, this->states[i]) : 
            coFactorFalse(reachable, this->states[i]);
    }
    return reachable == BDD_ID_1;
}

