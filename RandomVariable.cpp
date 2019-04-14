#include <cstdlib>
#include <cmath>
#include "RandomVariable.h"

double universal_rv () {
	return (double) rand () / (double) RAND_MAX;
}

int uniform_rv (int NumofNodes) {
	return (int) (universal_rv () * NumofNodes);	
}

double exponential_rv (double lambda) {
    double exp;

    exp = ((-1) / lambda) * log (universal_rv ());
    return (exp);
}   	
