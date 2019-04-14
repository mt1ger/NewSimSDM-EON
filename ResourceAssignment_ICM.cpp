/**************************************************
 * First-Fit  
 **************************************************/
// #define DEBUG_print_SrcAvailableSpecSlots
// #define DEBUG_print_resource_state_on_the_path
// #define DEBUG_print_AvailableSpecSlots
// #define DEBUG_collect_EventID_of_blocked_requests //need to collaberate with debug_print_eventid_of_blocked_requests
//
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
 
#include "ResourceAssignment_ICM.h"

using namespace std;

void ResourceAssignment::check_availability_source (unsigned int predecessor, unsigned int successor, CircuitRequest * circuitRequest) {
	list<int> HAvailableSpecSlots;

	AvailableSpecSlots.clear ();
	for (int c = 0; c < network->NumofCores; c++) {
		HAvailableSpecSlots.push_back (c);
		for (int i = 0; i < NUMOFSPECTRALSLOTS; i++) {
			if (network->SpectralSlots[predecessor][successor][c][i] == false){
				HAvailableSpecSlots.push_back (i);
			}
		}
		AvailableSpecSlots.push_back (HAvailableSpecSlots);
		HAvailableSpecSlots.clear ();
	}
}



void ResourceAssignment::check_availability_link (vector<int> * CircuitRoute) {
	list<int>::iterator i;

	#ifdef DEBUG_print_SrcAvailableSpecSlots
	cout << "PRINT Source Available SpecSlots" << endl;
	for (int c = 0; c < network->NumofCores; c++) {
		cout << "Number of SrcAvailableSpecSlots: " << AvailableSpecSlots[c].size () - 1 << endl;
		for (i = AvailableSpecSlots[c].begin (); i != AvailableSpecSlots[c].end (); i++) {
				cout << *i << ' ';
		}
		cout << endl;
	}
	#endif

	for (int r = 2; r < CircuitRoute->size (); r++) {
		for (int c = 0; c < network->NumofCores; c++) {
			for (i = (++AvailableSpecSlots[c].begin ()); i != AvailableSpecSlots[c].end (); i++) {
				if (network->SpectralSlots[CircuitRoute->at (r - 1)][CircuitRoute->at (r)][c][*i] == true) {
					i = AvailableSpecSlots[c].erase (i);
					i--;
				}
			}
		}
	}
}


void ResourceAssignment::handle_requests (CircuitRequest * circuitRequest) {
	RoutingTable routingTable (network);

	vector<int> CircuitRoute;
	bool AvailableFlag = true;
	vector<int> AssignedSS;
	vector< vector<int> > AssignedSpectralSection;
	vector<int> HAssignedSpectralSection;
	string MF = "BPSK";
	unsigned int mfTimes = 0;
	unsigned int NumofGroups = 0;
	unsigned int TempNumofTransponders = 0; // Number of Group Sections

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
			for (int j = 0; j < NUMOFSPECTRALSLOTS; j++) {
				cout <<  network->SpectralSlots[CircuitRoute[i - 1]][CircuitRoute[i]][c][j] << ' ';
			}
			cout << endl;
		}
	}
	#endif

	// cout << "Before Modulation: " << circuitRequest->DataSize << ' ' << circuitRequest->OccupiedSpectralSlots << endl;
	#ifdef LOCK_use_Modulation_Formats
	ModulationFormats modulationFormats (circuitRequest, network);
	modulationFormats.mf_chosen (CircuitRoute, &circuitRequest->OccupiedSpectralSlots, &circuitRequest->DataSize, &MF, &mfTimes);
	#endif
	// cout << "After Modulation: " << circuitRequest->DataSize << ' ' << circuitRequest->OccupiedSpectralSlots << endl;

	
	// Calculate possible SpectralSlotSections on the link between source and its successor
	check_availability_source (CircuitRoute[0], CircuitRoute[1], circuitRequest);

	check_availability_link (&CircuitRoute);

	#ifdef DEBUG_print_AvailableSpecSlots
	cout << "PRINT Available Spectral Slots" << endl;
	list<int>::iterator i;
	for (int c = 0; c < network->NumofCores; c++) {
	cout << "Number of Available Spectral Slots: "<< AvailableSpecSlots[c].size () - 1 << endl;
		for (i = AvailableSpecSlots[c].begin (); i != AvailableSpecSlots[c].end (); i++) {
				cout << *i << ' ';
		}
		cout << endl;
	}
	#endif

	for (int c = 0; c < network->NumofCores; c++) {
		AssignedSS.clear ();
		HAssignedSpectralSection.clear ();
		AssignedSpectralSection.clear ();
		AssignedSpectralSection.push_back ({c});
		TempNumofTransponders = 0;
		AvailableFlag = true;
		if (circuitRequest->OccupiedSpectralSlots >= (AvailableSpecSlots[c].size () - 1)) {
			AvailableFlag = false;
			continue;
		}
		else {
			AvailableFlag = false;
			list<int>::iterator i;
			for (i = (++AvailableSpecSlots[c].begin ()); i != AvailableSpecSlots[c].end (); i++) {
				if (AssignedSS.empty ()) {
					AssignedSS.push_back (*i);
					HAssignedSpectralSection.push_back (*i);
					HAssignedSpectralSection.push_back (*i);
					AssignedSpectralSection.push_back (HAssignedSpectralSection);
					TempNumofTransponders = 1;
				}
				else {
					int Temp; 
					Temp = AssignedSS.back ();
					if (*i == Temp + 1) {
						AssignedSS.push_back (*i);
						HAssignedSpectralSection.at (1) = *i;
						AssignedSpectralSection.at (AssignedSpectralSection.size () - 1) = HAssignedSpectralSection;
					}
					else {
						if ((HAssignedSpectralSection[1] != HAssignedSpectralSection[0])) {
							AssignedSpectralSection.push_back (HAssignedSpectralSection);
							HAssignedSpectralSection.clear ();
							TempNumofTransponders++;
							AssignedSS.push_back (*i);
							HAssignedSpectralSection.push_back (*i);
							HAssignedSpectralSection.push_back (*i);
						}
						else {
							HAssignedSpectralSection.clear ();
							AssignedSS.erase (AssignedSS.end () - 1);
							AssignedSS.push_back (*i);
							HAssignedSpectralSection.push_back (*i);
							HAssignedSpectralSection.push_back (*i);
						}
					}

					if ((circuitRequest->OccupiedSpectralSlots + TempNumofTransponders) == AssignedSS.size ()) {
						AvailableFlag = true;
						for (int j = 1; j < CircuitRoute.size (); j++) {
							for (int i = 0; i < AssignedSS.size (); i++) {
								network->SpectralSlots[CircuitRoute[j - 1]][CircuitRoute[j]][c][AssignedSS[i]] = true;
							}
						}
						break;
					}
					else {
						AvailableFlag = false;

					}
				}
			}
			if (AvailableFlag == true) break;
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
		cout << "ModulationFormats: " << MF << endl;
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
		cout << "Core: " << AssignedSpectralSection[0][0] << endl;
		for (int i = 1; i < AssignedSpectralSection.size (); i++) {
			cout << "Spectral Section: " << AssignedSpectralSection[i][0] << " to " << AssignedSpectralSection[i][1] << endl; 
		}
		cout << "# of Transponders Used: " << TempNumofTransponders << endl;
		cout << "# of Cores Used: " << '1' << endl;
		cout << "------------------------------------------------------------" << endl;
		#endif

		CircuitRelease * circuitRelease;
		circuitRelease = new CircuitRelease (circuitRequest->EventID, CircuitRoute, AssignedSpectralSection, circuitRequest->StartTime + circuitRequest->Duration, TempNumofTransponders);
		eventQueue->queue_insert (circuitRelease);

		network->NumofTransponders = network->NumofTransponders + TempNumofTransponders;
		network->NumofAllocatedRequests++;
		network->TotalTranspondersUsed += TempNumofTransponders; 
		network->TotalHoldingTime += circuitRequest->Duration;
		network->TotalCoresUsed++;
		network->TotalGBUsed += TempNumofTransponders;
		network->TotalDataSize += circuitRequest->DataSize;
		network->TotalSSUsed += (circuitRequest->OccupiedSpectralSlots) * mfTimes;
		network->TotalSSOccupied += (circuitRequest->OccupiedSpectralSlots + TempNumofTransponders) * mfTimes;

	}

	#ifdef DEBUG_print_resource_state_on_the_path
	for (int i = 0; i < CircuitRoute.size (); i++) {
		cout << CircuitRoute[i] << ' ';
	}
	cout << endl;
	cout << "PRINT resources AFTER Allocation" << endl;
	for (int i = 1; i < CircuitRoute.size (); i++) {
		cout << "On link " << CircuitRoute[i - 1] << " to " << CircuitRoute[i] << endl;
		for (int c = 0; c < network->NumofCores; c++) {
			for (int j = 0; j < NUMOFSPECTRALSLOTS; j++) {
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
			for (int j = 0; j < NUMOFSPECTRALSLOTS; j++) {
				cout <<  network->SpectralSlots[circuitRelease->CircuitRoute[i - 1]][circuitRelease->CircuitRoute[i]][c][j] << ' ';
			}
			cout << endl;
		}
	}
	#endif

	for (int i = 1; i < circuitRelease->CircuitRoute.size (); i++) {
		for (int j = 1; j < circuitRelease->OccupiedSpectralSection.size (); j++) {
			for (int k = circuitRelease->OccupiedSpectralSection[j][0]; k <= circuitRelease->OccupiedSpectralSection[j][1]; k++) {
				network->SpectralSlots[circuitRelease->CircuitRoute[i - 1]][circuitRelease->CircuitRoute[i]][circuitRelease->OccupiedSpectralSection[0][0]][k] = false;	
			}
		}
	}

	
	network->NumofDoneRequests++;
	network->NumofTransponders = network->NumofTransponders - circuitRelease->TranspondersUsed;
	
	#ifdef PRINT_allocation_block_release
	cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
	cout << "Release Event: " << circuitRelease->EventID << "\tTime: " << circuitRelease->EventTime << endl;
	cout << "Core: " << circuitRelease->OccupiedSpectralSection[0][0] << endl;
	for (int i = 1; i < circuitRelease->OccupiedSpectralSection.size (); i++) {
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
			for (int j = 0; j < NUMOFSPECTRALSLOTS; j++) {
				cout <<  network->SpectralSlots[circuitRelease->CircuitRoute[i - 1]][circuitRelease->CircuitRoute[i]][c][j] << ' ';
			}
			cout << endl;
		}
	}
	cout << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" << endl;
	cout << endl;
	#endif
}


