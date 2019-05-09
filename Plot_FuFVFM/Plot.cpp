#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <fstream>
#include <thread>
#include <mutex>
#include <sys/stat.h>
#include "txtParse.h"

// #define NUMOFSEEDS 30
#define NUMOFSEEDS 3
// #define NUMOFREQUESTS 100000 
#define NUMOFREQUESTS 10000 
// #define LTDOFSEG 4
#define LTDOFSEG 2

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
	static string Exec = "/home/mt1ger/Desktop/Simulator/NewSimSDM-EON/Sim_FuFVFM"; 
	static string Topo = "/home/mt1ger/Desktop/Simulator/NewSimSDM-EON/Topology/NSF14.txt";
	static string Path = "/home/mt1ger/Desktop/Simulator/NewSimSDM-EON/Plot_FuFVFM/";
	unsigned int cnt = 0;
	string Filename;
	txtParse txtparse;
	int NumofThreads = 0;
	vector<string> FolderSet;


	Core.push_back (1);
	// Core.push_back (2);
	Core.push_back (4);
	Core.push_back (7);

	// for (int i = 100; i < 130; i += 10) {
	for (int i = 10; i < 501; i += 10) {
		Lambda.push_back (i);
	}

	// cout << "Input a number for seed and press enter: " << endl;
	// cin >> Number; 
	Number = 40;
	srand (Number);
	for (int i = 0; i < NUMOFSEEDS; i++) {
		Seed.push_back (rand () / 65535);
	}

	

	for (unsigned int Max = 1; Max <= LTDOFSEG; Max++) {
		NumofThreads = Core.size () * Lambda.size () * Seed.size ();
		thread ThreadPointer[NumofThreads];

		string IsoFolder, Command, PlotFile;
		IsoFolder = Path + to_string (Max) + '/';
		FolderSet.push_back (IsoFolder);
		Command = "mkdir " + IsoFolder;
		PlotFile = IsoFolder + "Plot.txt";
		system (Command.c_str ());
		ofstream file (PlotFile);
		for (unsigned int k = 0; k < Core.size (); k++) {
			for (unsigned int lambda = 0; lambda < Lambda.size (); lambda++) {
				for (unsigned int seed = 0; seed < NUMOFSEEDS; seed++) {
					string Cmd = Exec + ' ' + Topo + ' ' + IsoFolder + ' ' + to_string (NumofRequests) + ' ' + to_string (Core[k]) + ' ' + to_string (Lambda[lambda] * Core[k]) + " 1 " + to_string (Seed[seed]) + ' ' + to_string(Max);
					ThreadPointer[cnt] = thread (thread_function, Cmd);
					cnt ++;
				}
			} 
		}
		for (int i = 0; i < NumofThreads; i++) {
			ThreadPointer[i].join ();
		}
		cnt = 0;
	}


	for (int i = 0; i < FolderSet.size (); i++) 
	txtparse.parse (Core, Lambda, Seed, FolderSet[i]);

	return 1;
}




