/**************************************************
 * First-Fit  
 **************************************************/
#define DEBUG_in_check_availability_link
// #define DEBUG_print_resource_state_on_the_path
#define DEBUG_print_SourceAvailableSections
// #define DEBUG_collect_EventID_of_blocked_requests //need to collaberate with debug_print_eventid_of_blocked_requests
//
// #define PRINT_allocation_block_release

#include "ModulationFormats.h"
#include <iostream>
#include "ResourceAssignment_IsolatedCore.h"
#include <string>
 

//Used event to represent Event instance
void ResourceAssignment::check_availability_source (unsigned int predecessor, unsigned int successor, CircuitRequest * circuitRequest) {
	bool AvailableFlag = true;
	vector<int> HSourceAvailableSections;

	SourceAvailableSections.clear ();
	
	for (int c = 0; c < network->NumofCores; c++) {
		for (int i = 0; i < (NUMOFSPECTRALSLOTS - circuitRequest->OccupiedSpectralSlots + 1); i++) {
			AvailableFlag = true;
			if (network->SpectralSlots[predecessor][successor][c][i] == false) {
				for (int j = 0; j < circuitRequest->OccupiedSpectralSlots; j++) {
					if (network->SpectralSlots[predecessor][successor][c][i + j] == true) {
						i = i + j;
						AvailableFlag = false;
						break;	
					}
				}

				if (AvailableFlag == true) {
					HSourceAvailableSections.push_back (c);
					HSourceAvailableSections.push_back (i);	
					HSourceAvailableSections.push_back (i + circuitRequest->OccupiedSpectralSlots - 1);
					SourceAvailableSections.push_back (HSourceAvailableSections);
					HSourceAvailableSections.clear ();	
				}
			}
		}
	}
}


//Used event to represent Event instance
void ResourceAssignment::check_availability_link (unsigned int predecessor, unsigned int successor, int i, bool * AvailableFlag, int * TempCore, int * TempSpecSlot) {

	#ifdef DEBUG_in_check_availability_link
	cout << "The checked link is between node " << predecessor << " and " << successor << endl; 
	cout << "The checked SpectralSlots are between  " << SourceAvailableSections[i][1] << " and " << SourceAvailableSections[i][2] << endl;
	#endif 

	for (int j = SourceAvailableSections[i][1]; j < SourceAvailableSections[i][2] + 1; j++) {
		#ifdef DEBUG_in_check_availability_link
		cout << network->SpectralSlots[predecessor][successor][SourceAvailableSections[i][0]][j] << ' ';
		#endif

		if (network->SpectralSlots[predecessor][successor][SourceAvailableSections[i][0]][j] == true) {
			* AvailableFlag = false;
			* TempCore = SourceAvailableSections[i][0]; 
			* TempSpecSlot = j; 
			break;	
		}	
	}

	#ifdef DEBUG_in_check_availability_link
	cout << endl;
	#endif
}


void ResourceAssignment::handle_requests (CircuitRequest * circuitRequest) {
	RoutingTable routingTable (network);	

	vector<int> CircuitRoute;
	bool AvailableFlag = true;
	vector< vector<int> > AssignedSpectralSection;
	string MF = "BPSK";
	int TempCore = -1;
	int TempSpecSlot = -1;
	unsigned int mfTimes = 0;
	unsigned int core;

	CircuitRoute = routingTable.get_shortest_path (circuitRequest->Src, circuitRequest->Dest);

	#ifdef DEBUG_print_resource_state_on_the_path
	cout << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" << endl;
	for (int i = 0; i < CircuitRoute.size (); i++) {
		cout << CircuitRoute[i] << ' ';
	}
	cout << endl;
	cout << "PRINT resources BEFORE Allocation" << endl;
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

	#ifdef LOCK_use_Modulation_Formats
	ModulationFormats modulationFormats (circuitRequest, network);
	modulationFormats.mf_chosen (CircuitRoute, &circuitRequest->OccupiedSpectralSlots, &circuitRequest->DataSize, &MF, &mfTimes);
	circuitRequest->OccupiedSpectralSlots = circuitRequest->OccupiedSpectralSlots + GB; // For GB
	#endif

	// Calculate possible SpectralSlotSections on the link between source and its successor
	check_availability_source (CircuitRoute[0], CircuitRoute[1], circuitRequest);

	#ifdef DEBUG_print_SourceAvailableSections
	cout << "PRINT Source Available Sections" << endl;
	for (int i = 0; i < SourceAvailableSections.size (); i++) {
		for (int j = 0; j < SourceAvailableSections[i].size (); j++) {
			cout << SourceAvailableSections[i][j] << ' ';
		}
		cout << '\t';
	}
	cout << endl;
	#endif
 
	// Loop for all the possible SpectralSlotSections on the link between source and its successor
	if (SourceAvailableSections.empty ()) AvailableFlag = false;
	else {
		for (int i = 0; i < SourceAvailableSections.size (); i++) {
			AvailableFlag = true;

			if (TempCore != SourceAvailableSections[i][0]) {
				TempSpecSlot = -1;
			}
			else {
				if (TempSpecSlot >= SourceAvailableSections[i][1]) {
					AvailableFlag = false;
					continue;
				}
			}

			// Loop to check if the selected SpectalSlotSection on source is available for all the following links on the path
			for (int j = 2; j < CircuitRoute.size (); j++) {
				// For each link on the path, check if the selected SpectralSlotSection available on the selected link
				check_availability_link (CircuitRoute[j - 1], CircuitRoute[j], i, &AvailableFlag, &TempCore, &TempSpecSlot);
				// if the AvailableFlag is false, the selected SpectralSlotSecton is not available on the selected link
				if (AvailableFlag == false) break;
			}
			if (AvailableFlag == true) {
				AssignedSpectralSection.push_back (SourceAvailableSections[i]);
				for (int i = 1; i < CircuitRoute.size (); i++) {
					for (int k = AssignedSpectralSection[0][1]; k < AssignedSpectralSection[0][2] + 1; k++) {
						network->SpectralSlots[CircuitRoute[i - 1]][CircuitRoute[i]][AssignedSpectralSection[0][0]][k] = true;
					}
				}
				break;
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
		for(unsigned int t = 0; t < CircuitRoute.size()-1; t++)
			cout << CircuitRoute.at(t) << " --> ";
		cout << CircuitRoute.at (CircuitRoute.size() - 1) << endl;
		cout << "Core: " << AssignedSpectralSection[0][0] << "  Spectral Section: " << AssignedSpectralSection[0][1] << " to " << AssignedSpectralSection[0][2] << endl; 
		cout << "# of Cores Used: " << '1' << endl;
		cout << "# of Transponders Used: " << '1' << endl;
		cout << "------------------------------------------------------------" << endl;
		#endif

		CircuitRelease * circuitRelease;
		circuitRelease = new CircuitRelease (circuitRequest->EventID, CircuitRoute, AssignedSpectralSection, circuitRequest->StartTime + circuitRequest->Duration, 1);
		eventQueue->queue_insert (circuitRelease);

		network->NumofAllocatedRequests++;
		network->NumofTransponders++;
		network->TotalTranspondersUsed++;
		network->TotalHoldingTime += circuitRequest->Duration;
		network->TotalCoresUsed++;
		network->TotalGBUsed++;
		network->TotalDataSize += circuitRequest->DataSize;
		network->TotalSSUsed += (circuitRequest->OccupiedSpectralSlots - GB) * mfTimes; // GB is added into circuitRequest->OccupiedSpectralSlots when line 115.
		network->TotalSSOccupied += (circuitRequest->OccupiedSpectralSlots - GB + GB) * mfTimes ;
	}

	#ifdef DEBUG_print_resource_state_on_the_path
	for (int i = 0; i < CircuitRoute.size (); i++) {
		cout << CircuitRoute[i] << ' ';
	}
	cout << endl;
	cout << "RRINT resources AFTER Allocation: " << endl;
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
		for (int k = circuitRelease->OccupiedSpectralSection[0][1]; k < circuitRelease->OccupiedSpectralSection[0][2] + 1; k++) {
			network->SpectralSlots[circuitRelease->CircuitRoute[i - 1]][circuitRelease->CircuitRoute[i]][circuitRelease->OccupiedSpectralSection[0][0]][k] = false;	
		}
	}
	
	network->NumofDoneRequests++;
	network->NumofTransponders--;
	
	#ifdef PRINT_allocation_block_release
	cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
	cout << "Release Event: " << circuitRelease->EventID << "\tTime: " << circuitRelease->EventTime << endl;
	cout << "Core: " << circuitRelease->OccupiedSpectralSection[0][0] << "  Spectral Section: " << circuitRelease->OccupiedSpectralSection[0][1] << " to " << circuitRelease->OccupiedSpectralSection[0][2] << endl;
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
	cout << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" << endl << endl;
	#endif
}

	
	

