#include <cassert>

#include "Manager.h"

using namespace ClassProject;

Manager::Manager(std::string name) : name(name) {
    bdd_count = 0;
}

BDD_ID Manager::createVar(const std::string &label) {
    return 0;
}

size_t Manager::uniqueTableSize() {
    return 0;
}

