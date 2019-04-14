#ifndef _H_TXTPARSE
#define _H_TXTPARSE

#include <vector>

using namespace std;

class txtParse {
	public:
		txtParse () {}
		~txtParse () {}
		void parse (vector<int> &PotentialCore, vector<int> &PotentialLambda, vector<int> &PatentialSeed, string path);
};

#endif

