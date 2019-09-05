//
// Initial File created by ludwig on 27.11.18.
// Completed and Corrected by Hector Margarito and Bruno Pessanha
//

#ifndef VDSPROJECT_IMGCOMP_H
#define VDSPROJECT_IMGCOMP_H


#include <map>
#include "ReachableInterface.h"

namespace ClassProject {

    class Reachable : public ReachableInterface {

    public:
        BDD_ID xnor2(BDD_ID a, BDD_ID b);
        const std::vector<BDD_ID> &getStates() const; //check if compiles
        void setDelta(const std::vector<BDD_ID> &transitions);
        void setInitState(const std::vector<bool>& stateVector);
        BDD_ID compute_reachable_states();
        bool is_reachable(const std::vector<bool>& stateVector);
        Reachable(unsigned int stateSize);
        ~Reachable();

    private:
        size_t stateSize;
        std::vector<BDD_ID> states;
        std::vector<BDD_ID> transitions;
        BDD_ID getTransitionRelation(int index);
        BDD_ID computeTransitionRelations();
        BDD_ID computeImage(BDD_ID relations_tau, BDD_ID char_R);
        BDD_ID characteristic_S0;
    };
}
#endif //VDSPROJECT_IMGCOMP_H
