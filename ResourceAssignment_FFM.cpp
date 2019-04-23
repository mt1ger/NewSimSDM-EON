/**************************************************
 * First-Fit  
 **************************************************/

#include <iostream>
#include <string>
#include <list>
#include <cmath>
#include <map>
 
#include "ModulationFormats1.h"
#include "ResourceAssignment_FullyFlex.h"

/***** CHECK RESOURCE AVAILABILITY *****/
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

	#ifdef DEBUG_print_AvailableSpecSlots
	cout << "\033[0;32mPRINT\033[0m " << "available spectral slots:" << endl;
	for (i = AvailableSpecSlots.begin (); i != AvailableSpecSlots.end (); i++) {
		for (int j = 0; j < i->size (); j++) {
			cout << i->at (j) << ' ';
		}
		cout << "    ";
	}
	cout << endl;
	#endif
}


void ResourceAssignment::handle_requests (CircuitRequest * circuitRequest) {
	RoutingTable routingTable (network);	
	ModulationFormats modulationFormats (circuitRequest, network);

	vector<int> CircuitRoute;
	bool AvailableFlag = true;
	vector< vector<int> > AssignedSpectralSection;
	vector<int> HAssignedSpectralSection;
	vector<int> HPotentialSections;
	list< vector<int> > PotentialSections;
	list< vector<int> > SortedSections, SortedSections1;
	unsigned int BitRate = 0;
	unsigned int NumofGB = 0;
	unsigned int NumofTransponders = 0; // The real number of transponders - number of guardbands 
	string MF = "QPSK";
	unsigned int mfTimes = 0;
	int TempCore = -1;
	int TempSpecSlot = -1;
	unsigned int core;

	CircuitRoute = routingTable.get_shortest_path (circuitRequest->Src, circuitRequest->Dest);

	#ifdef DEBUG_print_resource_state_on_the_path
	cout << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" << endl;
	cout << "\033[0;32mPRINT\033[0m " << "resources BEFORE allcation: " << endl;
	for (int i = 0; i < CircuitRoute.size (); i++) {
		cout << CircuitRoute[i] << ' ';
	}
	cout << endl;
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
	
	// Calculate possible SpectralSlotSections on the link between source and its successor
	check_availability_source (CircuitRoute[0], CircuitRoute[1], circuitRequest);
	check_availability_link (&CircuitRoute);

	#ifdef DEBUG_print_AvailableSpecSlots
	cout << "\033[0;32mPRINT\033[0m " << "Available Spectral Slots" << endl;
	list< vector<int> >::iterator i;
	for (i = AvailableSpecSlots.begin (); i != AvailableSpecSlots.end (); i++) {
		for (int j = 0; j < i->size (); j++) {
			cout << i->at (j) << ' ';
		}
		cout << "    ";
	}
	cout << endl;
	#endif

	/*** Use the Available Spectral Slots to form Potential SEctions ***/
	list< vector<int> >::iterator i;
	for (i = AvailableSpecSlots.begin (); i != AvailableSpecSlots.end (); i++) {
		if (HPotentialSections.empty ()) {
			if (i != (--AvailableSpecSlots.end ())) {
				HPotentialSections.push_back (i->at (0));
				HPotentialSections.push_back (i->at (1));
				HPotentialSections.push_back (i->at (1));
				PotentialSections.push_back (HPotentialSections);
			}
		}
		else {
			if ((i->at (1) == (HPotentialSections.at (2) + 1)) && (i->at (0) == HPotentialSections.at (0))) {
				HPotentialSections.at (2) = i->at(1);
				*(--PotentialSections.end ()) = HPotentialSections;
			}
			else {
				if (HPotentialSections.at (2) != HPotentialSections.at (1)) {
					HPotentialSections.clear ();
					if (i != (--AvailableSpecSlots.end ())) {
						HPotentialSections.push_back (i->at (0));
						HPotentialSections.push_back (i->at (1));
						HPotentialSections.push_back (i->at (1));
						PotentialSections.push_back (HPotentialSections);
					}
				}
				else {
					HPotentialSections.clear ();
					PotentialSections.erase (--PotentialSections.end ());
					if (i != (--AvailableSpecSlots.end ())) {
						HPotentialSections.push_back (i->at (0));
						HPotentialSections.push_back (i->at (1));
						HPotentialSections.push_back (i->at (1));
						PotentialSections.push_back (HPotentialSections);
					}
				}
			}
		}
	}

	#ifdef DEBUG_print_PotentialSections
	cout << "\033[0;32mPRINT\033[0m " << "Potential Sections" << endl;
	for (i = PotentialSections.begin (); i != PotentialSections.end (); i++) {
		for (int j = 0; j < i->size (); j++) {
			cout << i->at (j) << ' ';
		}
		cout << "    ";
	}
	cout << endl;
	#endif

		SortedSections = PotentialSections;

	/*** Sort SC Options by their performance form high to low ***/
	vector<int> SCSSs; // The needed SS for non-modulated Super Channel options
	vector<int> SCSizes; // The bit rate for each super channel
	vector<int> SCMSSs; // The needed SS for each modulated super channel

	map<int, int> RMSSs; // The needed SS for modulated request 
	vector<string> MFormat; // The allowed modulation format for each super channel 
	vector<int> SortedSCSizes; // Sorted index of performance of super channel options
	list<int> TempRMSSs;

	SCSSs.push_back (2);
	SCSSs.push_back (4);
	// SCSSs.push_back (6);
	SCSSs.push_back (8);
	SCSizes.push_back (25);
	SCSizes.push_back (50);
	// SCSizes.push_back (75);
	SCSizes.push_back (100);

	for (int i = 0; i < SCSSs.size (); i++)
	{
		unsigned int TempSCSS = SCSSs.at (i);
		unsigned int TempSCSize = SCSizes.at (i);
		modulationFormats.mf_chosen (CircuitRoute, &TempSCSS, &TempSCSize, &MF, &mfTimes);
		SCMSSs.push_back (TempSCSS);
		MFormat.push_back (MF);
		int temp = (TempSCSS + 1) * ceil ((double)circuitRequest->DataSize / SCSizes[i]);
		RMSSs[SCSizes.at (i)] = temp;
	}

	#ifdef DEBUG_print_SCinfo
	cout << "\033[0;32mPRINT\033[0m " << "SC related info" << endl;
	for (int i = 0; i < SCSizes.size (); i++) {
		cout << "SC: " << SCSizes.at (i) << "    MF: " << MFormat.at (i) << "    SS: " << SCMSSs.at (i) << endl;  
	}
	#endif 

	map<int, int>::iterator RMjter;
	list<int>::iterator RMiter;
	for (RMjter = RMSSs.begin (); RMjter != RMSSs.end(); RMjter++)
	{
		TempRMSSs.push_back (RMjter->second);
	}

	#ifdef DEBUG_print_SCinfo
	cout << "\033[0;32mPRINT\033[0m " << "The # of SSs each request needs for each modulated SC (GB included)" << endl;
	for (RMjter = RMSSs.begin (); RMjter != RMSSs.end (); RMjter++)
	{
		cout << "SC " << RMjter->first << " uses " << RMjter->second << " SSs for the request" << endl;  
	}
	#endif 

	TempRMSSs.sort ();
	TempRMSSs.unique ();

	for (RMiter = TempRMSSs.begin (); RMiter != TempRMSSs.end (); RMiter++)
	{
		int TempCnt = 0; // The number of equalities of (* RMiter == RMjter->second)
		int TempIndex = -1; // The number of TempRMSSs iterations
		vector<int> TempIndexs;


		for (RMjter = RMSSs.begin (); RMjter != RMSSs.end (); RMjter++)
		{
			TempIndex++;  
			if (*RMiter == RMjter->second)
			{
				TempCnt++;
				TempIndexs.push_back (TempIndex);
			}
		}

		if (TempCnt > 1)
		{
			list<int> TempSCSizesforSorting;
			for (int i = 0; i < TempIndexs.size (); i++)
			{
				RMjter = RMSSs.begin ();
				advance (RMjter, TempIndexs.at (i));
				TempSCSizesforSorting.push_back (RMjter->first);
			}
			list<int>::iterator k;
			TempSCSizesforSorting.sort ();
			TempSCSizesforSorting.reverse ();
			for (k = TempSCSizesforSorting.begin (); k != TempSCSizesforSorting.end (); k++)
			{
				SortedSCSizes.push_back (*k);
			}
		}
		else 
		{
			RMjter = RMSSs.begin ();
			advance (RMjter, TempIndexs.at (0));
			SortedSCSizes.push_back (RMjter->first);
		}

	}
	#ifdef DEBUG_print_SortedSC
	cout << "\033[0;32mPRINT\033[0m " << "Sorted Super Channels" << endl;
	for (int i = 0; i < SortedSCSizes.size (); i++) {
		cout << SortedSCSizes.at (i) << ' ';
	}
	cout << endl;
	#endif 
	
	/*** Pre-Allocation ***/
	int MainLoopIndex; // The index of the SC that has the highest performance
	list< vector<int> >::iterator Index;
	for (int i = 0; i < SCSizes.size (); i++)
	{
		if (SortedSCSizes.at (0) == SCSizes.at (i))
		{
			MainLoopIndex = i;
			break;
		}
	}

	BitRate = circuitRequest->DataSize;
	bool NextSC = false;
	
	for (int i = MainLoopIndex; i >= 0; i--)
	{
		// AssignedSpectralSection.clear ();
		// BitRate = circuitRequest->DataSize;
		// SortedSections = PotentialSections;
		Index = SortedSections.begin ();

		if (SortedSections.empty ())
		{
			AvailableFlag = false;
			break;
		}
		while (BitRate > 0)
		{
			// for (Index = SortedSections.begin (); Index != SortedSections.end (); Index++) {
				if (Index == SortedSections.end ())
				{
					NextSC = true;
					break;
				}
				else if (Index->at (2) - Index->at (1) == SCMSSs.at (i)) {
					HAssignedSpectralSection.push_back (Index->at (0));
					HAssignedSpectralSection.push_back (Index->at (1));
					HAssignedSpectralSection.push_back (Index->at (2));
					HAssignedSpectralSection.push_back (SCSizes.at (i));
					AssignedSpectralSection.push_back (HAssignedSpectralSection);
					HAssignedSpectralSection.clear ();
					if (BitRate >= SCSizes.at (i))
						BitRate -= SCSizes.at (i);
					else
						BitRate = 0;
					NumofGB++;
					Index = SortedSections.erase (Index);

					#ifdef DEBUG_print_PotentialSections
					list< vector<int> >::iterator ProbeIndex;
					cout << "\033[0;32mPRINT\033[0m " << "Sorted Sections" << endl;
					for (ProbeIndex = SortedSections.begin (); ProbeIndex != SortedSections.end (); ProbeIndex++) {
						for (int j = 0; j < ProbeIndex->size (); j++) {
							cout << ProbeIndex->at (j) << ' ';
						}
						cout << "    ";
					}
					cout << endl;
					#endif
					break;
				}
				else if (Index->at (2) - Index->at (1) > SCMSSs.at (i)) {
					HAssignedSpectralSection.push_back (Index->at (0));
					HAssignedSpectralSection.push_back (Index->at (1));
					HAssignedSpectralSection.push_back (Index->at (1) + SCMSSs.at (i) + GB - 1);
					HAssignedSpectralSection.push_back (SCSizes.at (i));
					AssignedSpectralSection.push_back (HAssignedSpectralSection);
					HAssignedSpectralSection.clear ();
					if (BitRate >= SCSizes.at (i))
						BitRate -= SCSizes.at (i);
					else
						BitRate = 0;
					if (Index->at (2) - (Index->at (1) + SCMSSs.at (i) + GB) + 1 >= 2)
					{
						HAssignedSpectralSection.push_back (Index->at (0));
						HAssignedSpectralSection.push_back (Index->at (1) + SCMSSs.at (i) + GB);
						HAssignedSpectralSection.push_back (Index->at (2)); 
						*Index = HAssignedSpectralSection;
						HAssignedSpectralSection.clear ();
					}
					else
						Index = SortedSections.erase (Index);
					NumofGB++;
					#ifdef DEBUG_print_PotentialSections
					list< vector<int> >::iterator ProbeIndex;
					cout << "\033[0;32mPRINT\033[0m " << "Sorted Sections" << endl;
					for (ProbeIndex = SortedSections.begin (); ProbeIndex != SortedSections.end (); ProbeIndex++) {
						for (int j = 0; j < ProbeIndex->size (); j++) {
							cout << ProbeIndex->at (j) << ' ';
						}
						cout << "    ";
					}
					cout << endl;
					#endif
					break;
				}
				else if (Index->at (2) - Index->at (1) < SCMSSs.at (i))
					Index++;
			// }
		}
		if (BitRate == 0)
			break;
		if (AvailableFlag == false)
			break;
		else if (NextSC == true)
			continue;
		else 
		{
			if (BitRate > 0)
			{
				if (BitRate >= SCSizes.at (i))
				{
					i++;
				}
				else if (BitRate < SCSizes.at (i) && SCSizes.at (i) - BitRate < SCSizes.at (i) / 2) 
				{
					i++;
				}
				else if (BitRate <= 25)
					i = 1;
			}
		}
	}
	if (AssignedSpectralSection.size () > network->SectionNumLimitation)
	{
		cout << "Seg # exceeds limitation" << endl;

		AssignedSpectralSection.clear ();
		BitRate = circuitRequest->DataSize;
		SortedSections1 = PotentialSections;
		Index = SortedSections1.begin ();
		int i;

		#ifdef DEBUG_print_PotentialSections
		list< vector<int> >::iterator ProbeIndex;
		cout << "\033[0;32mPRINT\033[0m " << "Sorted Sections" << endl;
		for (ProbeIndex = SortedSections1.begin (); ProbeIndex != SortedSections1.end (); ProbeIndex++) {
			for (int j = 0; j < ProbeIndex->size (); j++) {
				cout << ProbeIndex->at (j) << ' ';
			}
			cout << "    ";
		}
		cout << endl;
		#endif

		if (circuitRequest->DataSize > 50 && circuitRequest->DataSize <= 100)
			i = SCSizes.size () - 1;
		else if (circuitRequest->DataSize > 25 && circuitRequest->DataSize <= 50)
			i = 1;
		else if (circuitRequest->DataSize <= 25)
			i = 0;

		while (BitRate > 0)
		{
			// for (Index = SortedSections.begin (); Index != SortedSections.end (); Index++) {
			if (Index == SortedSections1.end ())
			{
				AvailableFlag = false;
				break;
			}
			else if (Index->at (2) - Index->at (1) == SCMSSs.at (i)) {
				HAssignedSpectralSection.push_back (Index->at (0));
				HAssignedSpectralSection.push_back (Index->at (1));
				HAssignedSpectralSection.push_back (Index->at (2));
				HAssignedSpectralSection.push_back (SCSizes.at (i));
				AssignedSpectralSection.push_back (HAssignedSpectralSection);
				HAssignedSpectralSection.clear ();
				if (BitRate >= SCSizes.at (i))
					BitRate -= SCSizes.at (i);
				else
					BitRate = 0;
				NumofGB++;
			}
			else if (Index->at (2) - Index->at (1) > SCMSSs.at (i)) {
				HAssignedSpectralSection.push_back (Index->at (0));
				HAssignedSpectralSection.push_back (Index->at (1));
				HAssignedSpectralSection.push_back (Index->at (1) + SCMSSs.at (i) + GB - 1);
				HAssignedSpectralSection.push_back (SCSizes.at (i));
				AssignedSpectralSection.push_back (HAssignedSpectralSection);
				HAssignedSpectralSection.clear ();
				if (BitRate >= SCSizes.at (i))
					BitRate -= SCSizes.at (i);
				else
					BitRate = 0;
				if (Index->at (2) - (Index->at (1) + SCMSSs.at (i) + GB) + 1 >= 2)
				{
					HAssignedSpectralSection.push_back (Index->at (0));
					HAssignedSpectralSection.push_back (Index->at (1) + SCMSSs.at (i) + GB);
					HAssignedSpectralSection.push_back (Index->at (2)); 
					*Index = HAssignedSpectralSection;
					HAssignedSpectralSection.clear ();
				}
				else
					Index = SortedSections1.erase (Index);
				NumofGB++;
			}
			else if (Index->at (2) - Index->at (1) < SCMSSs.at (i))
				Index++;
			// }
		}
	}
	if (AssignedSpectralSection.size () > network->SectionNumLimitation) AvailableFlag = false;

	if (AvailableFlag == false) {
		network->NumofDoneRequests++;

		#ifdef DEBUG_collect_EventID_of_blocked_requests
		network->BlockedRequests.push_back (circuitRequest->EventID);
		#endif

		#ifdef PRINT_allocation_block_release
		cout << "^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^" << endl;
		cout << "Request " << circuitRequest->EventID << " is blocked" << endl;
		cout << "Modulation Format: " << MF << endl;
		cout << "# of Sections: " << NumofGB << endl;
		cout << "^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^" << endl;
		#endif

			network->NumofFailedRequests++;
	}
	else if (AvailableFlag == true) {
		int temp = AssignedSpectralSection[0][0];
		int CoreCnter = 1;
		for (int i = 0; i < AssignedSpectralSection.size (); i++) {
			int NumofSlots = AssignedSpectralSection[i][2] - AssignedSpectralSection[i][1];
			NumofTransponders = ceil (NumofSlots * 12.5 * mfTimes / 100) - 1;
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
		cout << "Source: " << circuitRequest->Src << "  Destination: " << circuitRequest->Dest << endl;
		cout << "Path: ";
		for(unsigned int t = 0; t < CircuitRoute.size() - 1; t++)
			cout << CircuitRoute.at(t) << " --> ";
		cout << CircuitRoute.at (CircuitRoute.size() - 1) << endl;

		for (int i = 0; i < AssignedSpectralSection.size (); i++) {
			cout << "Core: " << AssignedSpectralSection[i][0] << "  Spectral Section: " << AssignedSpectralSection[i][1] << " to " << AssignedSpectralSection[i][2] << "  SC: " << AssignedSpectralSection[i][3];   
			for (int j = 0; j < SCSizes.size (); j++)
			{
				if (AssignedSpectralSection[i][3] == SCSizes.at (j))
					cout << "  MF: " << MFormat.at (j) << endl;
			}
		}

		cout << "# of Guardbands Used: " << NumofGB << endl;
		cout << "# of Transponders Used: " << NumofGB << endl;
		cout << "# of Core Used: " << CoreCnter << endl;
		cout << "------------------------------------------------------------" << endl;
		#endif

		CircuitRelease * circuitRelease;
		circuitRelease = new CircuitRelease (circuitRequest->EventID, CircuitRoute, AssignedSpectralSection, circuitRequest->StartTime + circuitRequest->Duration, NumofGB);
		eventQueue->queue_insert (circuitRelease);

		for (int i = 0; i < AssignedSpectralSection.size (); i++) {
			if (AssignedSpectralSection[i][3] == 25)
				network->Numof25SC++;
			else if (AssignedSpectralSection[i][3] == 50)
				network->Numof50SC++;
			else if (AssignedSpectralSection[i][3] == 100)
				network->Numof100SC++;
			network->NumofSS4Data += AssignedSpectralSection[i][2] - AssignedSpectralSection[i][1];
			network->TotalSS4Data += AssignedSpectralSection[i][2] - AssignedSpectralSection[i][1];
			network->TotalSSOccupied += AssignedSpectralSection[i][2] - AssignedSpectralSection[i][1] + 1;
		}

		network->NumofAllocatedRequests++;
		network->NumofSections = network->SectionNumLimitation;
		network->TotalHoldingTime += circuitRequest->Duration;
		network->NumofTransponders += NumofGB;
		network->TotalTranspondersUsed += NumofGB;
		network->TotalCoresUsed += CoreCnter;
		network->TotalGBUsed += NumofGB;
		network->TotalDataSize += circuitRequest->DataSize;
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

	for (int i = 0; i < circuitRelease->OccupiedSpectralSection.size (); i++)
	{
		network->NumofSS4Data -= circuitRelease->OccupiedSpectralSection[i][2] - circuitRelease->OccupiedSpectralSection[i][1] + 1;
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

