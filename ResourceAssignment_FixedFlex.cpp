/**************************************************
 * First-Fit  
 **************************************************/
// #define DEBUG_print_resource_state_on_the_path
// #define DEBUG_print_AvailableSpecSlots
// #define DEBUG_collect_eventid_of_blocked_requests //need to collaberate with debug_print_eventid_of_blocked_requests

// #define PRINT_allocation_block_release
#define LOCK_use_Modulation_Formats

#ifdef LOCK_use_Modulation_Formats
#include "ModulationFormats.h"
#endif
#ifndef LOCK_use_Modulation_Formats
#include "ModulationFormats1.h"
#endif

#include <iostream>
#include <string>
#include <list>
 
#include "ResourceAssignment_FixedFlex.h"

using namespace std;

void ResourceAssignment::check_availability_source (unsigned int predecessor, unsigned int successor, CircuitRequest * circuitRequest) {
	bool AvailableFlag = true;
	vector<int> HAvailableSpecSections;
	unsigned int NumofGroups;

	AvailableSpecSections.clear ();
	if (circuitRequest->OccupiedSpectralSlots % network->NumofCores != 0) {
		NumofGroups = circuitRequest->OccupiedSpectralSlots / network->NumofCores + 1 + GB;
	}
	else NumofGroups = circuitRequest->OccupiedSpectralSlots / network->NumofCores + GB;

	for (int i = 0; i < (NumofSpectralSlots - NumofGroups + 1); i++) {
		AvailableFlag = true;
		if (network->SpectralSlots[predecessor][successor][0][i] == false) {
			for (int j = 0; j < NumofGroups; j++) {
				if (network->SpectralSlots[predecessor][successor][0][i + j] == true) {
					i = i + j;
					AvailableFlag = false;
					break;
				}
			}

			if (AvailableFlag == true) {
				HAvailableSpecSections.push_back (i);
				HAvailableSpecSections.push_back (i + NumofGroups - 1);
				AvailableSpecSections.push_back (HAvailableSpecSections);
				HAvailableSpecSections.clear ();
			}
		}
	}
}


void ResourceAssignment::check_availability_link (vector<int> * CircuitRoute) {
	#ifdef DEBUG_print_AvailableSpecSlots
	cout << "Start to print AvailableSpecSlots" << endl;
	list< vector<int> >::iterator i;
	cout << AvailableSpecSections.size () << endl;
	for (i = AvailableSpecSections.begin (); i != AvailableSpecSections.end (); i++) {
		for (int j = 0; j < i->size (); j++) {
			cout << i->at (j) << ' ';
		}
		cout << "    ";
	}
	cout << endl;
	#endif

	list< vector<int> >::iterator i;

	for (int r = 2; r < CircuitRoute->size (); r++) {
		for (i = AvailableSpecSections.begin (); i != AvailableSpecSections.end (); i++) {
			for (int j = i->at (0); j <= i->at (1); j++) {
				if (network->SpectralSlots[CircuitRoute->at (r - 1)][CircuitRoute->at (r)][0][j] == true) {
					i = AvailableSpecSections.erase (i);
					i--;
					break;
				}
			}
		}
	}
}


void ResourceAssignment::handle_requests (CircuitRequest * circuitRequest) {
	RoutingTable routingTable (network);	

	vector<int> CircuitRoute;
	bool AvailableFlag = true;
	vector< vector<int> > AssignedSpectralSection;
	vector<int> HAssignedSpectralSection;
	string MF = "BPSK";

	CircuitRoute = routingTable.get_shortest_path (circuitRequest->Src, circuitRequest->Dest);

	#ifdef DEBUG_print_resource_state_on_the_path
	cout << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" << endl;
	for (int i = 0; i < CircuitRoute.size (); i++) {
		cout << CircuitRoute[i] << ' ';
	}
	cout << endl;
	cout << "PRINT resources BEFORE Allcation: " << endl;
	for (int i = 1; i < CircuitRoute.size (); i++) {
		cout << "On link " << CircuitRoute[i - 1] << " to " << CircuitRoute[i] << endl;
		for (int c = 0; c < network->NumofCores; c++) {
			for (int j = 0; j < NumofSpectralSlots; j++) {
				cout <<  network->SpectralSlots[CircuitRoute[i - 1]][CircuitRoute[i]][c][j] << ' ';
			}
			cout << endl;
		}
	}
	#endif

	#ifdef LOCK_use_Modulation_Formats
	ModulationFormats modulationFormats (circuitRequest, network);
	MF = modulationFormats.mf_chosen (CircuitRoute, &circuitRequest->OccupiedSpectralSlots, &circuitRequest->DataSize);
	#endif

	// Calculate possible SpectralSlotSections on the link between source and its successor
	check_availability_source (CircuitRoute[0], CircuitRoute[1], circuitRequest);

	check_availability_link (&CircuitRoute);

	#ifdef DEBUG_print_AvailableSpecSlots
	cout << "Start to print AvailableSpecSlots" << endl;
	list< vector<int> >::iterator i;
	cout << AvailableSpecSections.size () << endl;
	for (i = AvailableSpecSections.begin (); i != AvailableSpecSections.end (); i++) {
		for (int j = 0; j < i->size (); j++) {
			cout << i->at (j) << ' ';
		}
		cout << "    ";
	}
	cout << endl;
	#endif

	if (AvailableSpecSections.empty ()) AvailableFlag = false;
	else {
		list< vector<int> >::iterator begin = AvailableSpecSections.begin ();

		HAssignedSpectralSection.push_back (begin->at (0));
		HAssignedSpectralSection.push_back (begin->at (1));
		AssignedSpectralSection.push_back (HAssignedSpectralSection);

		for (int i = 1; i < CircuitRoute.size (); i++) {
			for (int j = begin->at (0); j <= begin->at (1); j++) {
				network->SpectralSlots[CircuitRoute[i - 1]][CircuitRoute[i]][0][j] = true;
			}
		}
	}

		
	if (AvailableFlag == false) {
		network->NumofDoneRequests++;

		#ifdef DEBUG_collect_EventID_of_blocked_requests
		network->BlockedRequests.push_back (circuitRequest->EventID);
		#endif

		#ifdef PRINT_allocation_block_release
		cout << "^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^" << endl;
		cout << "Request " << circuitRequest->EventID << " is blocked" << endl;
		cout << "Modulation Format: " << MF << endl;
		cout << "^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^" << endl;
		#endif

		network->NumofFailedRequests++;
	}
	else if (AvailableFlag == true) {

		#ifdef PRINT_allocation_block_release
		cout << "------------------------------------------------------------" << endl;
		cout << "Request ID: " << circuitRequest->EventID << "\tStart: " << circuitRequest->EventTime << "\tEnd: " << circuitRequest->StartTime + circuitRequest->Duration << endl;
		cout << "Source: " << circuitRequest->Src << "  Destination: " << circuitRequest->Dest << "  ModulationFormats: " << MF << endl;
		cout << "Path: ";
		for(unsigned int t = 0; t < CircuitRoute.size() - 1; t++)
			cout << CircuitRoute.at(t) << " --> ";
		cout << CircuitRoute.at (CircuitRoute.size() - 1) << endl;
		cout << "Spectral Section: " << AssignedSpectralSection[0][0] << " to " << AssignedSpectralSection[0][1] << endl; 
		cout << "------------------------------------------------------------" << endl;
		#endif

		CircuitRelease * circuitRelease;
		circuitRelease = new CircuitRelease (circuitRequest->EventID, CircuitRoute, AssignedSpectralSection, circuitRequest->StartTime + circuitRequest->Duration, network->NumofCores);
		eventQueue->queue_insert (circuitRelease);

		network->NumofTransponders = network->NumofTransponders + network->NumofCores;
		network->NumofAllocatedRequests++;
		network->TotalHoldingTime += circuitRequest->Duration;
		network->TotalTranspondersUsed += network->NumofCores;
		network->TotalCoresUsed += network->NumofCores;
		network->TotalGBUsed += network->NumofCores;
		network->TotalDataSize += circuitRequest->DataSize;
		network->TotalSSUsed += circuitRequest->OccupiedSpectralSlots; 
		network->TotalSSOccupied += (AssignedSpectralSection[0][1] - AssignedSpectralSection[0][0] + 1) * network->NumofCores; 
	}

	#ifdef DEBUG_print_resource_state_on_the_path
	for (int i = 0; i < CircuitRoute.size (); i++) {
		cout << CircuitRoute[i] << ' ';
	}
	cout << endl;
	cout << "Start to print resources on the path after allocation" << endl;
	for (int i = 1; i < CircuitRoute.size (); i++) {
		cout << "On link " << CircuitRoute[i - 1] << " to " << CircuitRoute[i] << endl;
		for (int c = 0; c < network->NumofCores; c++) {
			for (int j = 0; j < NumofSpectralSlots; j++) {
				cout <<  network->SpectralSlots[CircuitRoute[i - 1]][CircuitRoute[i]][c][j] << ' ';
			}
			cout << endl;
		}
	}
	cout << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" << endl;
	cout << endl;
	#endif
}


void ResourceAssignment::handle_releases (CircuitRelease * circuitRelease) {
	#ifdef DEBUG_print_resource_state_on_the_path
	cout << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" << endl;
	for (int i = 0; i < circuitRelease->CircuitRoute.size (); i++) {
		cout << circuitRelease->CircuitRoute[i] << ' ';
	}
	cout << endl;
	cout << "PRINT resources BEFORE Release" << endl;
	for (int i = 1; i < circuitRelease->CircuitRoute.size (); i++) {
		cout << "On link " << circuitRelease->CircuitRoute[i-1] << " to " << circuitRelease->CircuitRoute[i] << endl;
		for (int c = 0; c < network->NumofCores; c++) {
			for (int j = 0; j < NumofSpectralSlots; j++) {
				cout <<  network->SpectralSlots[circuitRelease->CircuitRoute[i - 1]][circuitRelease->CircuitRoute[i]][c][j] << ' ';
			}
			cout << endl;
		}
	}
	#endif

	for (int i = 1; i < circuitRelease->CircuitRoute.size (); i++) {
		for (int k = circuitRelease->OccupiedSpectralSection[0][0]; k <= circuitRelease->OccupiedSpectralSection[0][1]; k++) {
			network->SpectralSlots[circuitRelease->CircuitRoute[i - 1]][circuitRelease->CircuitRoute[i]][0][k] = false;	
		}
	}

	
	network->NumofDoneRequests++;
	network->NumofTransponders = network->NumofTransponders - circuitRelease->TranspondersUsed;
	
	#ifdef PRINT_allocation_block_release
	cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
	cout << "Release Event: " << circuitRelease->EventID << "\tTime: " << circuitRelease->EventTime << endl;
	for (int i = 0; i < circuitRelease->OccupiedSpectralSection.size (); i++) {
		cout << "Spectral Section: " << circuitRelease->OccupiedSpectralSection[i][0] << " to " << circuitRelease->OccupiedSpectralSection[i][1] << endl;
	}
	cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
	#endif

	#ifdef DEBUG_print_resource_state_on_the_path
	for (int i = 0; i < circuitRelease->CircuitRoute.size (); i++) {
		cout << circuitRelease->CircuitRoute[i] << ' ';
	}
	cout << endl;
	cout << "PRINT resources AFTER Release" << endl;
	for (int i = 1; i < circuitRelease->CircuitRoute.size (); i++) {
		cout << "On link " << circuitRelease->CircuitRoute[i-1] << " to " << circuitRelease->CircuitRoute[i] << endl;
		for (int c = 0; c < network->NumofCores; c++) {
			for (int j = 0; j < NumofSpectralSlots; j++) {
				cout <<  network->SpectralSlots[circuitRelease->CircuitRoute[i - 1]][circuitRelease->CircuitRoute[i]][c][j] << ' ';
			}
			cout << endl;
		}
	}
	cout << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" << endl;
	cout << endl;
	#endif
}
