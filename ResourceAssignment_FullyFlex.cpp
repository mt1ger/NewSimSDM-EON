/**************************************************
 * First-Fit  
 **************************************************/

#include "ModulationFormats.h"
#include <iostream>
#include <string>
#include <list>
 
#include "ResourceAssignment_FullyFlex.h"


void ResourceAssignment::check_availability_source (unsigned int predecessor, unsigned int successor, CircuitRequest * circuitRequest) {
	vector<int> HAvailableSpecSlots;

	AvailableSpecSlots.clear ();
	for (int c = 0; c < network->NumofCores; c++) {
		for (int i = 0; i < NUMOFSPECTRALSLOTS; i++) {
			if (network->SpectralSlots[predecessor][successor][c][i] == false) {
				HAvailableSpecSlots.push_back (c);
				HAvailableSpecSlots.push_back (i);
				AvailableSpecSlots.push_back (HAvailableSpecSlots);
				HAvailableSpecSlots.clear ();	
			}
		}
	}
}


void ResourceAssignment::check_availability_link (vector<int> * CircuitRoute) {

	list< vector<int> >::iterator i;

	for (int r = 2; r < CircuitRoute->size (); r++) {
		for (i = AvailableSpecSlots.begin (); i != AvailableSpecSlots.end (); i++) {
			if (network->SpectralSlots[CircuitRoute->at (r - 1)][CircuitRoute->at (r)][i->at (0)][i->at (1)] == true) {
				i = AvailableSpecSlots.erase (i);
				i--;
				// AvailableSpecSlots.erase (i); // This line will only work on Mac, will cause Seg Fault (dump core) on ubuntu.
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
	vector<int> HAssignedSS;
	vector< vector<int> > AssignedSS;
	vector< vector<int> > TempSS;
	unsigned int TempNumofTransponders = 1;
	string MF = "BPSK";
	unsigned int mfTimes = 0;
	int TempCore = -1;
	int TempSpecSlot = -1;
	unsigned int core;

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
			cout << "On Core " << c << endl;
			for (int j = 0; j < NUMOFSPECTRALSLOTS; j++) {
				cout <<  network->SpectralSlots[CircuitRoute[i - 1]][CircuitRoute[i]][c][j] << ' ';
			}
			cout << endl;
		}
	}
	#endif

	#ifdef LOCK_use_Modulation_Formats
	ModulationFormats modulationFormats (circuitRequest, network);
	modulationFormats.mf_chosen (CircuitRoute, &circuitRequest->OccupiedSpectralSlots, &circuitRequest->DataSize, &MF, &mfTimes);
	#endif

	// Calculate possible SpectralSlotSections on the link between source and its successor
	check_availability_source (CircuitRoute[0], CircuitRoute[1], circuitRequest);
	check_availability_link (&CircuitRoute);

	#ifdef DEBUG_print_AvailableSpecSlots
	cout << "PRINT Available Spectral Slots" << endl;
	list< vector<int> >::iterator i;
	for (i = AvailableSpecSlots.begin (); i != AvailableSpecSlots.end (); i++) {
		for (int j = 0; j < i->size (); j++) {
			cout << i->at (j) << ' ';
		}
		cout << "    ";
	}
	cout << endl;
	#endif

	if (circuitRequest->OccupiedSpectralSlots >= AvailableSpecSlots.size ()) {
		AvailableFlag = false;
	}
	else {
		list< vector<int> >::iterator i;
		for (i = AvailableSpecSlots.begin (); i != AvailableSpecSlots.end (); i++) {
			if (AssignedSS.empty ()) {
				if (i != (--AvailableSpecSlots.end ())) {
					AssignedSS.push_back (*i);
					HAssignedSpectralSection.push_back (i->at (0));
					HAssignedSpectralSection.push_back (i->at (1));
					HAssignedSpectralSection.push_back (i->at (1));
					AssignedSpectralSection.push_back (HAssignedSpectralSection);
				}
			}
			else {
				vector<int> Temp; 
				Temp = AssignedSS.back ();
				if ((i->at (1) == Temp[1] + 1) && (i->at (0) == Temp[0])) {
					AssignedSS.push_back (*i);
					HAssignedSpectralSection.at (2) = i->at (1);
					AssignedSpectralSection.at (AssignedSpectralSection.size () - 1) = HAssignedSpectralSection;
				}
				else {
					if ((HAssignedSpectralSection[2] != HAssignedSpectralSection[1])) {
						HAssignedSpectralSection.clear ();
						if (i != (--AvailableSpecSlots.end ())) {
							TempNumofTransponders++;
							AssignedSS.push_back (*i);
							HAssignedSpectralSection.push_back (i->at (0));
							HAssignedSpectralSection.push_back (i->at (1));
							HAssignedSpectralSection.push_back (i->at (1));
							AssignedSpectralSection.push_back (HAssignedSpectralSection);
						}
					}
					else {
						HAssignedSpectralSection.clear ();
						AssignedSS.erase (AssignedSS.end () - 1);
						AssignedSpectralSection.pop_back (); 
						if (i != (--AvailableSpecSlots.end ())) {
							AssignedSS.push_back (*i);
							HAssignedSpectralSection.push_back (i->at (0));
							HAssignedSpectralSection.push_back (i->at (1));
							HAssignedSpectralSection.push_back (i->at (1));
							AssignedSpectralSection.push_back (HAssignedSpectralSection);
						}
					}
				}

				if ((circuitRequest->OccupiedSpectralSlots + TempNumofTransponders) == AssignedSS.size ()) {
					AvailableFlag = true;
					break;
				} 
				else {
					AvailableFlag = false;
				}
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
		int temp = AssignedSpectralSection[0][0];
		int CoreCnter = 1;
		for (int i = 0; i < AssignedSpectralSection.size (); i++) {
			for (int j = AssignedSpectralSection[i][1]; j <= AssignedSpectralSection[i][2]; j++) {
				for (int cr = 1; cr < CircuitRoute.size (); cr ++) 
				network->SpectralSlots[CircuitRoute[cr - 1]][CircuitRoute[cr]][AssignedSpectralSection[i][0]][j] = true;
			}
			if (AssignedSpectralSection[i][0] != temp) {
				temp = AssignedSpectralSection[i][0];
				CoreCnter++;
			}
		}

		#ifdef PRINT_allocation_block_release
		cout << "------------------------------------------------------------" << endl;
		cout << "Request ID: " << circuitRequest->EventID << "\tStart: " << circuitRequest->EventTime << "\tEnd: " << circuitRequest->StartTime + circuitRequest->Duration << endl;
		cout << "Source: " << circuitRequest->Src << "  Destination: " << circuitRequest->Dest << "  ModulationFormats: " << MF << endl;
		cout << "Path: ";
		for(unsigned int t = 0; t < CircuitRoute.size() - 1; t++)
			cout << CircuitRoute.at(t) << " --> ";
		cout << CircuitRoute.at (CircuitRoute.size() - 1) << endl;

		for (int i = 0; i < AssignedSpectralSection.size (); i++) {
			cout << "Core: " << AssignedSpectralSection[i][0] << "  Spectral Section: " << AssignedSpectralSection[i][1] << " to " << AssignedSpectralSection[i][2] << endl; 
		}

		cout << "# of Transponders Used: " << TempNumofTransponders << endl;
		cout << "# of Core Used: " << CoreCnter << endl;
		cout << "------------------------------------------------------------" << endl;
		#endif

		CircuitRelease * circuitRelease;
		circuitRelease = new CircuitRelease (circuitRequest->EventID, CircuitRoute, AssignedSpectralSection, circuitRequest->StartTime + circuitRequest->Duration, TempNumofTransponders);
		eventQueue->queue_insert (circuitRelease);

		network->NumofAllocatedRequests++;
		network->NumofSections = TempNumofTransponders;
		network->TotalHoldingTime += circuitRequest->Duration;
		network->NumofTransponders = network->NumofTransponders + TempNumofTransponders;
		network->TotalTranspondersUsed += TempNumofTransponders;
		network->TotalCoresUsed += CoreCnter;
		network->TotalGBUsed += TempNumofTransponders;
		network->TotalDataSize += circuitRequest->DataSize;
		network->TotalSSUsed += circuitRequest->OccupiedSpectralSlots * mfTimes;
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
		for (int j = 0; j < circuitRelease->OccupiedSpectralSection.size (); j++) {
			for (int k = circuitRelease->OccupiedSpectralSection[j][1]; k <= circuitRelease->OccupiedSpectralSection[j][2]; k++) {
				network->SpectralSlots[circuitRelease->CircuitRoute[i - 1]][circuitRelease->CircuitRoute[i]][circuitRelease->OccupiedSpectralSection[j][0]][k] = false;	
			}
		}
	}

	network->NumofDoneRequests++;
	network->NumofTransponders = network->NumofTransponders - circuitRelease->TranspondersUsed;

	#ifdef PRINT_allocation_block_release
	cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
	cout << "Release Event: " << circuitRelease->EventID << "\tTime: " << circuitRelease->EventTime << endl;
	for (int i = 0; i < circuitRelease->OccupiedSpectralSection.size (); i++) {
		cout << "Core: " << circuitRelease->OccupiedSpectralSection[i][0] << "  Spectral Section: " << circuitRelease->OccupiedSpectralSection[i][1] << " to " << circuitRelease->OccupiedSpectralSection[i][2] << endl;
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
