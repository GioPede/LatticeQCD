#pragma once
#include "Math/latticemath.h"

// Prototype for class Action. Every implementation of a physical action should
// inherit from this class.
class Action{

public:
    Action();
    // virtual methods for the action class
    virtual void initAction(Lattice* lattice) = 0;
    virtual double compute(int x, int y, int z, int t, int mu,
                           SU3& newLink, SU3& constPart) = 0;
    virtual SU3 computeConstant(int x, int y, int z, int t, int mu) = 0;
};
