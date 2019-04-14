#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <fstream>
#include <thread>
#include <mutex>
#include "txtParse.h"

#define NUMOFSEEDS 30
#define NUMOFREQUESTS 50

using namespace std;
mutex mtx;

void thread_function (string cmd) {
	mtx.lock ();
	system (cmd.c_str ());
	// cout << cmd << endl;
	mtx.unlock ();
}

int main () {
	vector<int> Core, Lambda, Seed;
	double Number;
	static unsigned int NumofRequests = NUMOFREQUESTS;
	static string Exec = "~/Desktop/Simulator/SimSDM-EON/Sim_FixedFlex"; 
	// static string Topo = "~/Desktop/Simulator/SimSDM-EON/Topology/NSF14.txt";
	static string Topo = "~/Desktop/Simulator/SimSDM-EON/Topology/Test.txt";
	unsigned int cnt = 0;
	string Filename;
	txtParse txtparse;

	ofstream plot ("Plot.txt");

	Core.push_back (1);
	Core.push_back (2);
	Core.push_back (4);
	// Core.push_back (7);

	for (int i = 100; i < 491; i += 10) {
		Lambda.push_back (i);
	}

	cout << "Input a number for seed and press enter: " << endl;
	cin >> Number; 
	srand (Number);
	for (int i = 0; i < NUMOFSEEDS; i++) {
		Seed.push_back (rand () / 65535);
	}

	
	cout << Core.size () << endl;
	cout << Lambda.size () << endl;
	cout << Seed.size () << endl;
	int NumofThreads = 0;
	NumofThreads = Core.size () * Lambda.size () * Seed.size ();
	thread ThreadPointer[NumofThreads];

	for (unsigned int k = 0; k < Core.size (); k++) {
		for (unsigned int lambda = 0; lambda < Lambda.size (); lambda++) {
			for (unsigned int seed = 0; seed < NUMOFSEEDS; seed++) {
				string Cmd = Exec + ' ' + Topo + " Plot.txt " + to_string (NumofRequests) + ' ' + to_string (Core[k]) + ' ' + to_string (Lambda[lambda] * Core[k]) + " 1 " + to_string (Seed[seed]);
				ThreadPointer[cnt] = thread (thread_function, Cmd);
				cnt ++;
			}
		} 
	}

	for (int i = 0; i < NumofThreads; i++) {
		ThreadPointer[i].join ();
	}

	plot.close ();
	txtparse.parse (Core, Lambda, Seed);
	return 1;
}





