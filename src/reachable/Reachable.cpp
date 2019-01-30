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
    } //todo: add the conjunction
    for (int i = 0; i < stateSize; i++) {
        this->states.push_back(createVar("S'" + std::to_string(i)));
    }
    //std::cout << "INITIAL TABLE" << std::endl;
    //printTable();
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
    for (int i = 0; i < stateVector.size() - 1; i++) {
        and2(stateVector[i] ? this->states[i] : neg(this->states[i]), stateVector[i+1] ? this->states[i+1] : neg(this->states[i+1]));
    }
}

BDD_ID Reachable::computeTransitionRelations() {
    BDD_ID relations_tau = getTransitionRelation(0);
    for (int i = 1; i < stateSize; i++) {
        relations_tau = and2(relations_tau, getTransitionRelation(i));
    }
    //std::cout << "TRANSITION RELATIONS" << std::endl;
    //printTable();
    return relations_tau;
}

BDD_ID Reachable::compute_reachable_states() {
    return compute_reachable_state(0);
}

BDD_ID Reachable::compute_reachable_state(int state) {
    BDD_ID char_R = False();
    if (stateSize > 0) {
        BDD_ID relations_tau = computeTransitionRelations();
        BDD_ID char_R_it = getCharacteristicFunction(state);
        int index = 0;
        do {
            char_R = char_R_it;
            BDD_ID image = computeImage(index++, relations_tau, char_R);
            char_R_it = or2(char_R, image);
        } while (index < stateSize - 1);
    }
    return char_R;
}

BDD_ID Reachable::computeImage(int index, BDD_ID relations_tau, BDD_ID char_R) {
    /*Compute img(s0',s1')*/
    BDD_ID temp1 = and2(char_R, relations_tau);
    BDD_ID temp2 = or2(coFactorTrue(temp1, states[index+1]), coFactorFalse(temp1, states[index+1]));
    BDD_ID img_S0S1_next = or2(coFactorTrue(temp2, states[index]), coFactorFalse(temp2, states[index]));

    BDD_ID s0_prime = states[stateSize + index];
    BDD_ID s1_prime = states[stateSize + index + 1];

    /*Compute img(s0,s1)*/
    temp1 = and2(xnor2(states[index],s0_prime), xnor2(states[index+1],s1_prime));
    temp2 = and2(temp1, img_S0S1_next);
    BDD_ID temp3 = or2(coFactorTrue(temp2, s1_prime), coFactorFalse(temp2, s1_prime));
    BDD_ID img_SOS1 = or2(coFactorTrue(temp3, s0_prime), coFactorFalse(temp3, s0_prime));

    return img_SOS1;
}

BDD_ID Reachable::getCharacteristicFunction(int index) {
    return and2(xnor2(states[index],0),xnor2(states[index+1], 0));
}

BDD_ID Reachable::getTransitionRelation(int index) {
    BDD_ID sPrime = this->states[stateSize + index];
    return or2( and2(sPrime, this->transitions[index]), and2(neg(sPrime), neg(this->transitions[index])));
}

bool Reachable::is_reachable(const std::vector<bool>& stateVector) {
    BDD_ID representation_of_reachable_states = compute_reachable_states();
    BDD_ID cofactors[stateSize];
    BDD_ID universal_quant;

    for (int i = 0; i < stateSize - 1; i++) {
        if(stateVector[i])
            cofactors[i] = coFactorTrue(this->states[i],representation_of_reachable_states);
        else
            cofactors[i] = coFactorFalse(this->states[i],representation_of_reachable_states);
    }

    for (int i = 0; i < stateSize - 1; i++) {
        if(0 == i) universal_quant = cofactors[i];
        else{
            universal_quant = and2(universal_quant,cofactors[i]);
        }
    }

    if( universal_quant == representation_of_reachable_states) return true;
    else false;


}

