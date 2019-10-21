
#include <pthread.h>
#include <time.h>
#include <fstream>
#include <iostream>
#include <vector>
#include <string.h>
#include "Network.h"
#include "RoutingTable.h"


using namespace std;


clock_t StartPoint, EndPoint;
int EndFlag = 0;

static void * timer_thread (void *) {
	StartPoint = clock ();
	while (1) {
		EndPoint = clock ();
		if (EndFlag == 1) {
			pthread_exit (NULL);
		}
	}
}


int main (int argc, char *argv[]) {
	pthread_t timer;
	string Path;

	Network * network;
	Network net;
	network = &net;
	

	if (argc != 9) {
		cout << "Please input arguments in the following order: " << endl;	
		cout << "\tThe file for network topology" << endl; 
		cout << "\tThe path to store results" << endl;
		cout << "\tTotal number of requests" << endl;
		cout << "\tThe number of cores" << endl;
		cout << "\tAverage arriving rate of request (Lambda)" << endl;
		cout << "\tAverage holding time (1 / Mu)" << endl;
		cout << "\tSeed for random number generation" << endl;
		cout << "\tSections Number limitation" << endl;
		cout << endl;
		exit (0);
	}

	strcpy (network->FileName, argv[1]);
	Path = argv[2];
	network->NumofRequests = atol (argv[3]);
	network->NumofCores = atoi (argv[4]);
	network->Lambda = atof (argv[5]); 
	network->Mu = atof (argv[6]);
	srand (atof (argv[7]));
	network->SectionNumLimitation = atof (argv[8]);
	
	pthread_create (&timer, NULL, timer_thread, NULL);

	network->init ();

	network->simulation ();
	double Erlang = network->Lambda / network->Mu;
	double BlockingProbability = (double) network->NumofFailedRequests / (double) network->NumofRequests;

	fstream filep;
	string FilePath;
	FilePath = Path + "MaxSec.txt";
	filep.open (FilePath, fstream::app);
	filep << to_string (network->MaxNumofSections) + ' '; 
	filep.close ();

	fstream fp;
	string ResultFile;
	ResultFile = Path + "Plot.txt";
	fp.open (ResultFile, fstream::app);
	string plot = to_string (network->NumofCores) + ' ' + to_string (Erlang) + ' ' + argv[7] + ' ' + to_string (BlockingProbability) + ' ' + to_string (network->MaxNumofTransponders) + ' ' + to_string (network->AvgCoresUsed) + ' ' + to_string (network->AvgHoldingTime) + ' ' + to_string (network->AvgTranspondersUsed)+ ' ' + to_string (network->AvgGBUsed) + ' ' + to_string (network->AvgIntFrag) + ' ' + to_string (network->AvgExtFrag) + ' ' + to_string (network->AvgHybridFrag) + ' ' + to_string (network->Numof200SC6) + ' ' + to_string (network->Numof200SC4) + ' ' + to_string (network->Numof200SC2) + ' ' + to_string (network->Numof100SC6) + ' ' + to_string (network->Numof100SC4) + ' ' + to_string (network->Numof100SC2) + ' ' + to_string (network->Numof50SC6) + ' ' + to_string (network->Numof50SC4) + ' ' +  to_string (network->Numof50SC2) + ' ' + to_string (network->Numof25SC) + ' ' + to_string (network->block_400) + ' ' + to_string (network->block_100) + ' ' + to_string (network->block_40) + '\n'; 
	fp << plot;
	fp.close ();

	EndFlag = 1;
	pthread_join (timer, NULL);

	cout << "The incoming requests distribution: " << endl;
	cout << network->probe_40 << ' ' << network->probe_100 << ' ' << network->probe_400 << endl;
	cout << "The blocked request distribution: " << endl;
	cout << network->block_40 << ' ' << network->block_100 << ' ' << network->block_400 << endl;

	double TimeSpent = (((double)(EndPoint - StartPoint))/ CLOCKS_PER_SEC);
	cout << "Time spent in \"s\" is " << TimeSpent << endl;
	cout << "************************************************************" << endl;

	return 1;
}
