# An implementation of a minimal BDD package in C++
Project class for Verification of Digital Systems class
Authors: [Hector Margarito](https://hector-margarito.github.io/), Bruno Pessanha

This package implement the fundamental manipulation methods for ROBDDs. An application
is also implemented which is to check whether a specific state is within the reachable
state space or not usin Symbolic Traversal of Finite State Machines.


## To compile 
```
mkdir build && cd &_
cmake ../
make
make doc
```

## To run test 
```
cd build/src/test
./VDSProject_test
```
## Structure of the repo
```
build/                Contains the needed files for building and its executable results
build/doc_doxygen/    Contains doxygen documentation
doc/                  Contains the doxygen file configuration
src/                  Contiains all source files
```
To add more Gtests, please include them in `src/test/Test.cpp`
