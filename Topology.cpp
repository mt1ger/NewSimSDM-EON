#include <iostream>
#include <cstdio>
#include <vector>
#include "Topology.h"

using namespace std;

void Topology::read_topology (void) {
	FILE * nettopo;
	char file_name[100];
	unsigned int number;
	int temp;
	vector <double> HNodesWeight;

	nettopo = fopen (network->FileName, "r");

	fscanf (nettopo, "%d", &network->NumofNodes);		

	for (int i = 0; i < network->NumofNodes; i++) {
		for (int j = 0; j < network->NumofNodes; j++) {
			fscanf (nettopo, "%d", &temp); 
			HNodesWeight.push_back (temp);
		}

	network->NodesWeight.push_back (HNodesWeight);
		HNodesWeight.clear ();
	}

	fclose (nettopo);
}


