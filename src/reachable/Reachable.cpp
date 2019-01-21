//
// Created by ludwig on 27.11.18.
//

#include "Reachable.h"
#include <string>

using namespace ClassProject;

Reachable::Reachable(unsigned int stateSize) : ReachableInterface(stateSize) {
    this->states.reserve(stateSize);
    for (int i = 0; i < stateSize; i++) {
        this->states[i] = createVar("S" + std::to_string(i));
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
    for (int i = 0; i < transitions.size(); i++) {
        createVar("S'" + std::to_string(i));
    }
}

void Reachable::setInitState(const std::vector<bool>& stateVector) {
    for (int i = 0; i < stateVector.size(); i++) {
        this->states[i] = stateVector[i] ? this->states[i] : neg(this->states[i]);
    }
}

BDD_ID Reachable::computeTransitionRelations() {
    BDD_ID relations_tau = getTransitionRelation(0);
    for (int i = 1; i < states.size(); i++) {
        relations_tau = and2(relations_tau, getTransitionRelation(i));
    }
    return relations_tau;
}

BDD_ID Reachable::compute_reachable_states() {
    return compute_reachable_state(0);
}

BDD_ID Reachable::compute_reachable_state(int state) {
    BDD_ID char_R = False();
    if (states.size() > 0) {
        BDD_ID relations_tau = computeTransitionRelations();
        BDD_ID char_R_it = getCharacteristicFunction(state);
        int index = 0;
        do {
            char_R = char_R_it;
            BDD_ID image = computeImage(index++, relations_tau, char_R);
            char_R_it = or2(char_R, image);
        } while (char_R != char_R_it);
    }
    return char_R;
}

BDD_ID Reachable::computeImage(int index, BDD_ID relations_tau, BDD_ID char_R) {
    /*Compute img(s0',s1')*/
    BDD_ID temp1 = and2(char_R, relations_tau);
    BDD_ID temp2 = or2(coFactorTrue(temp1, states[index+1]), coFactorFalse(temp1, states[index+1]));
    BDD_ID img_S0S1_next = or2(coFactorTrue(temp2, states[index]), coFactorFalse(temp2, states[index]));

    /*Compute img(s0,s1)*/
    temp1 = and2(xnor2(states[index],transitions[index]), xnor2(states[index+1],transitions[index+1]));
    temp2 = and2(temp1, img_S0S1_next);
    BDD_ID temp3 = or2(coFactorTrue(temp2,transitions[index+1]), coFactorFalse(temp2, transitions[index+1]));
    BDD_ID img_SOS1 = or2(coFactorTrue(temp3,transitions[index]), coFactorFalse(temp3,transitions[index]));

    return img_SOS1;
}

BDD_ID Reachable::getCharacteristicFunction(int index) {
    return and2(xnor2(states[index],0),xnor2(states[index+1], 0));
}

BDD_ID Reachable::getTransitionRelation(int index) {
    return or2( and2(neg(states[index]), transitions[index]), and2(states[index], neg(transitions[index])));
}

bool Reachable::is_reachable(const std::vector<bool>& stateVector) {
    bool isReachable = true;
    for (int i = 0; i < stateVector.size() && isReachable; i++) {
        isReachable = stateVector[i] ? 
            states[i] == compute_reachable_state(states[i]) : 
            neg(states[i]) == compute_reachable_state(neg(states[i]));
    }
    return isReachable;
}

