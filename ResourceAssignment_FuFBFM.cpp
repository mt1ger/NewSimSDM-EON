/**************************************************
 * Largest-First  
 **************************************************/
#include <iostream>
#include <string>
#include <climits>
#include <cfloat>
#include <cmath>
#include <list>
#include <map>

#include "ResourceAssignment_FuFBF.h"
#include "ModulationFormats1.h"


/***** MERGE SORTING *****/
void merge (vector<int> * ar, int low, int mid, int high)
{
	int n, m, i, j, k;
	n = mid - low + 1;
	m = high - mid;
	vector<int> R, L;

	for (i = 0; i < n; i++)
	{
		L.push_back (ar->at (low + i));
	}
	for (j = 0; j < m; j++)
	{
		R.push_back (ar->at (mid + j + 1));
	}
	for (i = 0, j = 0, k = low; k <= high; k++) //
	{
		if (i < n && j < m)
		{
			if (L[i] <= R[j])
			{
				ar->at (k) = L[i];
				i++;
			}
			else
			{
				ar->at (k) = R[j];
				j++;
			}
		}
		else if (i < n && j >= m)
		{
			ar->at (k) = L[i];
			i++;
		}
		else if (i >= n && j < m)
		{
			ar->at (k) = R[j];
			j++;
		}
	}
}


void merge_sort(vector<int> * ar, int low, int high)
{
	int mid;
	if (low < high)
	{
		mid = ((high + low) / 2);
		merge_sort (ar, low, mid);
		merge_sort (ar, mid+1, high);
		merge (ar, low, mid, high);
	}
}

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
				i--; // Because i++ for each end of loop
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

/***** SORT SECTIONS *****/
void ResourceAssignment::sort_sections ( list< vector<int> > &OriginalList) 
{
	list< vector<int> >::iterator i;
	list< vector<int> > TempList;
	int SizeSum = 0;
	int temp = 0;
	vector<int> Size;
	int SizeCnt = 0;

	TempList = OriginalList;
	OriginalList.clear ();
	for (i = TempList.begin (); i != TempList.end (); i++) {
		Size.push_back (i->at (2) - i->at (1) + 1); 
		SizeCnt++;
	}
	merge_sort (&Size, 0, SizeCnt - 1);
	for (int s = SizeCnt - 1; s >= 0; s--) {
		for (i = TempList.begin (); i != TempList.end (); i++) {
			if ((i->at (2) - i->at (1) + 1) == Size[s]) {
				OriginalList.push_back (*i);
				TempList.erase (i);
				break;
			}
		}
	}

    #ifdef DEBUG_print_SortedSections
	cout << "\033[0;32mPRINT\033[0m " << "Sorted Sections" << endl;
	for (i = OriginalList.begin (); i != OriginalList.end (); i++) {
		for (int j = 0; j < i->size (); j++) {
			cout << i->at (j) << ' ';
		}
		cout << "    ";
	}
	cout << endl;
    #endif 
}

/***** HANDLE REQUEST *****/
void ResourceAssignment::handle_requests (CircuitRequest * circuitRequest) {
	RoutingTable routingTable (network);	
	ModulationFormats modulationFormats (circuitRequest, network);

	switch (circuitRequest->DataSize)
	{
		case 40:
			network->probe_40++;
			break;
		case 100:
			network->probe_100++;
			break;
		case 400:
			network->probe_400++;
			break;
	}

	/*** VARIABLE DECLARATION ***/
	vector<int> CircuitRoute;
	bool AvailableFlag = true;
	vector< vector<int> > AssignedSpectralSection;
	vector<int> HAssignedSpectralSection;
	vector<int> HPotentialSections;
	list< vector<int> > PotentialSections;
	unsigned int NumofGB = 0;
	string MF = "QPSK";
	unsigned int mfTimes = 0;
	unsigned int BitRate;
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

	/*** Check Available Resource, Make and Sort Sections ***/
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

	/*** Use the Available Spectral Slots to form Potential Sections ***/
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

	/** Sort the Sections in PotentialSections by its size from bigger to smaller **/
	int SizeSum = 0;
	int temp = 0;
	vector<int> Size;
	int SizeCnt = 0;
	list< vector<int> > SortedSections;
	list< vector<int> > SortedSections1;
	list< vector<int> >::iterator Index;
	for (i = PotentialSections.begin (); i != PotentialSections.end (); i++) {
		Size.push_back (i->at (2) - i->at (1) + 1); 
		SizeCnt++;
	}
	merge_sort (&Size, 0, SizeCnt - 1);
	for (int s = SizeCnt - 1; s >= 0; s--) {
		for (i = PotentialSections.begin (); i != PotentialSections.end (); i++) {
			if ((i->at (2) - i->at (1) + 1) == Size[s]) {
				SortedSections.push_back (*i);
				PotentialSections.erase (i);
				break;
			}
		}
	}
	SortedSections1 = SortedSections;

	#ifdef DEBUG_print_SortedSections
	cout << "\033[0;32mPRINT\033[0m " << "Sorted Sections" << endl;
	for (i = SortedSections.begin (); i != SortedSections.end (); i++) {
		for (int j = 0; j < i->size (); j++) {
			cout << i->at (j) << ' ';
		}
		cout << "    ";
	}
	cout << endl;
	#endif 

	/*** Sort SC Options by their performance form high to low ***/
	vector<int> CandidateSCs; // The needed SS for non-modulated Super Channel options
	vector<int> SCSizes; // The bit rate for each super channel

	map<int, int> SegNum4request;
	map<int, int> SCMSSs; // The needed SS for each modulated super channel
	map<int, int> RMSSs; // The needed SS for modulated request 
	vector<string> MFormat; // The allowed modulation format for each super channel 
	vector<int> mfTimesArray;
	vector<int> SortedSCSizes; // Sorted index of performance of super channel options
	list<int> TempRMSSs;

	CandidateSCs.push_back (25);
	CandidateSCs.push_back (50);
	// CandidateSCs.push_back (75);
	CandidateSCs.push_back (100);
	CandidateSCs.push_back (200);
	CandidateSCs.push_back (400);

	for (int i = 0; i < CandidateSCs.size (); i++)
	{
		unsigned int TempSCSS;
		unsigned int TempSCSize = CandidateSCs.at (i);
		modulationFormats.mf_chosen (CircuitRoute, &TempSCSS, &TempSCSize, &MF, &mfTimes);
		if (TempSCSS != -1)
		{
			SCMSSs[CandidateSCs[i]] = TempSCSS;
			MFormat.push_back (MF);
			mfTimesArray.push_back (mfTimes);
			int tempNum = ceil ((double)circuitRequest->DataSize / CandidateSCs[i]);
			SegNum4request[CandidateSCs[i]] = tempNum;
			int temp = (TempSCSS + 1) * ceil ((double)circuitRequest->DataSize / CandidateSCs[i]);
			RMSSs[CandidateSCs.at (i)] = temp;
			SCSizes.push_back (CandidateSCs[i]);
		}
	}

	#ifdef DEBUG_print_SCinfo
	cout << "\033[0;32mPRINT\033[0m " << "SC related info" << endl;
	for (int i = 0; i < SCSizes.size (); i++) {
		cout << "SC: " << SCSizes.at (i) << "    MF: " << MFormat.at (i) << "    SS: " << SCMSSs.at (SCSizes[i]) << endl;  
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

	SortedSections1 = SortedSections;

	#ifdef DEBUG_print_SortedSC
	cout << "\033[0;32mPRINT\033[0m " << "Sorted Super Channels" << endl;
	for (int i = 0; i < SortedSCSizes.size (); i++) {
		cout << SortedSCSizes.at (i) << ' ';
	}
	cout << endl;
	#endif 



	/*** Pre-Allocation ***/

	/** Normal Mode **/
	int MainLoopIndex; // The index of the SC that has the highest performance
	vector<int> AllocatedIndex;
	for (int i = 0; i < SCSizes.size (); i++)
	{
		if (SortedSCSizes.at (0) == SCSizes.at (i))
		{
			MainLoopIndex = i;
			break;
		}
	}
	BitRate = circuitRequest->DataSize;

	for (int i = MainLoopIndex; i >= 0; i--)
	{
		Index = SortedSections.begin ();
		unsigned int TempSCSS = SCMSSs.at (SCSizes[i]);
		unsigned int TempSCSize = SCSizes.at (i);
		if (SortedSections.empty ()) 
		{
			AvailableFlag = false;
			break;
		}
		while (BitRate > 0)
		{
			if (Index->at (2) - Index->at (1) + 1 == TempSCSS + GB) {
				HAssignedSpectralSection.push_back (Index->at (0));
				HAssignedSpectralSection.push_back (Index->at (1));
				HAssignedSpectralSection.push_back (Index->at (2));
				HAssignedSpectralSection.push_back (i);
				AssignedSpectralSection.push_back (HAssignedSpectralSection);
				Index = SortedSections.erase (Index);
				HAssignedSpectralSection.clear ();
				if (BitRate >= SCSizes.at (i))
					BitRate -= SCSizes.at (i);
				else
					BitRate = 0;
				NumofGB++;
				break;
			}
			else if (Index->at (2) - Index->at (1) + 1 > TempSCSS + GB) {
				HAssignedSpectralSection.push_back (Index->at (0));
				HAssignedSpectralSection.push_back (Index->at (1));
				HAssignedSpectralSection.push_back (Index->at (1) + TempSCSS + GB - 1); 
				HAssignedSpectralSection.push_back (i);
				AssignedSpectralSection.push_back (HAssignedSpectralSection);
				HAssignedSpectralSection.clear ();
				if (BitRate >= SCSizes.at (i))
					BitRate -= SCSizes.at (i);
				else
					BitRate = 0;
				if (Index->at (2) - (Index->at (1) + TempSCSS + GB) + 1 >= 2)
				{
					HAssignedSpectralSection.push_back (Index->at (0));
					HAssignedSpectralSection.push_back (Index->at (1) + TempSCSS + GB);
					HAssignedSpectralSection.push_back (Index->at (2)); 
					SortedSections.push_back (HAssignedSpectralSection);
					Index = SortedSections.erase (Index);
					sort_sections (SortedSections);
					HAssignedSpectralSection.clear ();
				}
				else
					Index = SortedSections.erase (Index);
				NumofGB++;
				break;
			}
			else if (Index->at (2) - Index->at (1) + 1 < TempSCSS + GB) {
				AvailableFlag = false;
				break;
			}
		}
		if (BitRate == 0)
			break;
		if (AvailableFlag == false)
			break;
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
			}
		}
	}

	/** Limited Segment Mode **/
	if (AssignedSpectralSection.size () > network->SectionNumLimitation) 
	{
		int i = -1; // Index of SCMSSs and SCSizes

		for (int i = 1; i < SortedSCSizes.size (); i++) 
		{

			list< vector<int> > TempSortedSections;
			TempSortedSections = SortedSections1;
			AssignedSpectralSection.clear ();
			BitRate = circuitRequest->DataSize;
			NumofGB = 0;

			int SCindex;
			for (int j = 0; j < SCSizes.size (); j++)
				if (SortedSCSizes[i] == SCSizes[j])
					SCindex = j;
			unsigned int TempSCSS = SCMSSs.at (SortedSCSizes[i]);

			if (SCSizes[MainLoopIndex] > SortedSCSizes[i] || SegNum4request.at (SortedSCSizes[i]) > network->SectionNumLimitation)
				continue;

			#ifdef DEBUG_print_SortedSections
			cout << "\033[0;32mThe SC is \033[0m " << SortedSCSizes[i] << endl;
			#endif 

			for (int segNum = 0; segNum < SegNum4request.at (SortedSCSizes[i]); segNum++)
			{
				Index = TempSortedSections.begin (); 
				if (TempSortedSections.empty ())
				{
					AvailableFlag = false;
					break;
				}
				while (BitRate > 0)
				{
					if (Index->at (2) - Index->at (1) + 1 == TempSCSS + GB) {
						HAssignedSpectralSection.push_back (Index->at (0));
						HAssignedSpectralSection.push_back (Index->at (1));
						HAssignedSpectralSection.push_back (Index->at (2));
						HAssignedSpectralSection.push_back (SCindex);
						AssignedSpectralSection.push_back (HAssignedSpectralSection);
						Index = TempSortedSections.erase (Index);
						HAssignedSpectralSection.clear ();
						if (BitRate >= SortedSCSizes.at (i))
							BitRate -= SortedSCSizes.at (i);
						else
							BitRate = 0;
						NumofGB++;
						break;
					}
					else if (Index->at (2) - Index->at (1) + 1 > TempSCSS + GB) {
						HAssignedSpectralSection.push_back (Index->at (0));
						HAssignedSpectralSection.push_back (Index->at (1));
						HAssignedSpectralSection.push_back (Index->at (1) + TempSCSS + GB - 1); 
						HAssignedSpectralSection.push_back (SCindex);
						AssignedSpectralSection.push_back (HAssignedSpectralSection);
						HAssignedSpectralSection.clear ();
						if (BitRate >= SortedSCSizes.at (i))
							BitRate -= SortedSCSizes.at (i);
						else
							BitRate = 0;
						if (Index->at (2) - (Index->at (1) + TempSCSS + GB) + 1 >= 2)
						{
							HAssignedSpectralSection.push_back (Index->at (0));
							HAssignedSpectralSection.push_back (Index->at (1) + TempSCSS + GB);
							HAssignedSpectralSection.push_back (Index->at (2)); 
							TempSortedSections.push_back (HAssignedSpectralSection);
							Index = TempSortedSections.erase (Index);
							sort_sections (TempSortedSections);
							HAssignedSpectralSection.clear ();
						}
						else
							Index = TempSortedSections.erase (Index);
						NumofGB++;
						break;
					}
					else if (Index->at (2) - Index->at (1) + 1 < TempSCSS + GB) {
						AvailableFlag = false;
						break;
					}
				}
			}
			if (BitRate == 0)
				break;
			if (AvailableFlag == false)
			{
				AssignedSpectralSection.clear();
				AvailableFlag = true;
				continue;
			}
		}
	}

	if (AssignedSpectralSection.empty () == true) AvailableFlag = false;
	
	if (AvailableFlag == false) {
		network->NumofDoneRequests++;

		#ifdef DEBUG_collect_EventID_of_blocked_requests
		network->BlockedRequests.push_back (circuitRequest->EventID);
		#endif

		#ifdef PRINT_allocation_block_release
		cout << "^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^" << endl;
		cout << "Request " << circuitRequest->EventID << " is blocked" << endl;
		cout << "^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^" << endl;
		#endif

		network->NumofFailedRequests++;
		switch (circuitRequest->DataSize)
		{
			case 40:	
				network->block_40++;
				break;
			case 100:	
				network->block_100++;
				break;
			case 400:	
				network->block_400++;
				break;
		}

	}
	else if (AvailableFlag == true) {
		int temp = AssignedSpectralSection[0][0];
		int CoreCnter = 1;
		for (int i = 0; i < AssignedSpectralSection.size (); i++) {
			for (int p = 1; p < CircuitRoute.size (); p++) {
				for (int j = AssignedSpectralSection[i].at (1); j <= AssignedSpectralSection[i].at (2); j++) {
					network->SpectralSlots[CircuitRoute[p - 1]][CircuitRoute[p]][AssignedSpectralSection[i].at (0)][j] = true;
				}
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
			cout << "Core: " << AssignedSpectralSection[i][0] << "  Spectral Section: " << AssignedSpectralSection[i][1] << " to " << AssignedSpectralSection[i][2] << "  SC: " << SCSizes.at (AssignedSpectralSection[i].at (3)) << "  MF: " << MFormat.at (AssignedSpectralSection[i][3]) << endl;
		}

		cout << "# of Guardbands Used: " << NumofGB << endl;
		cout << "# of Transponders Used: " << NumofGB << endl;
		cout << "# of Core Used: " << CoreCnter << endl;
		cout << "------------------------------------------------------------" << endl;
		#endif

		CircuitRelease * circuitRelease;
		circuitRelease = new CircuitRelease (circuitRequest->EventID, CircuitRoute, AssignedSpectralSection, circuitRequest->StartTime + circuitRequest->Duration, NumofGB);
		eventQueue->queue_insert (circuitRelease);


		int TempData = circuitRequest->DataSize;
		for (int i = 0; i < AssignedSpectralSection.size (); i++) {
			if (SCSizes.at (AssignedSpectralSection[i][3]) == 25)
				network->Numof25SC++;
			else if (SCSizes.at (AssignedSpectralSection[i][3]) == 50)
			{
				if (MFormat.at (AssignedSpectralSection[i][3]) == "QPSK")
					network->Numof50SC2++;
				else if (MFormat.at (AssignedSpectralSection[i][3]) == "16QAM")
					network->Numof50SC4++;
			}
			else if (SCSizes.at (AssignedSpectralSection[i][3]) == 100)
			{
				if (MFormat.at (AssignedSpectralSection[i][3]) == "QPSK")
					network->Numof100SC2++;
				else if (MFormat.at (AssignedSpectralSection[i][3]) == "16QAM")
					network->Numof100SC4++;
			}			if (SCSizes.at (AssignedSpectralSection[i][3]) == 25)
				network->Numof25SC++;
			else if (SCSizes.at (AssignedSpectralSection[i][3]) == 50)
			{
				if (MFormat.at (AssignedSpectralSection[i][3]) == "QPSK")
					network->Numof50SC2++;
				else if (MFormat.at (AssignedSpectralSection[i][3]) == "16QAM")
					network->Numof50SC4++;
				else if (MFormat.at (AssignedSpectralSection[i][3]) == "64QAM")
					network->Numof50SC6++;
			}
			else if (SCSizes.at (AssignedSpectralSection[i][3]) == 100)
			{
				if (MFormat.at (AssignedSpectralSection[i][3]) == "QPSK")
					network->Numof100SC2++;
				else if (MFormat.at (AssignedSpectralSection[i][3]) == "16QAM")
					network->Numof100SC4++;
				else if (MFormat.at (AssignedSpectralSection[i][3]) == "64QAM")
				network->Numof100SC6++;
			}
			else if (SCSizes.at (AssignedSpectralSection[i][3]) == 200)
			{
				if (MFormat.at (AssignedSpectralSection[i][3]) == "QPSK")
					network->Numof200SC2++;
				else if (MFormat.at (AssignedSpectralSection[i][3]) == "16QAM")
					network->Numof200SC4++;
				else if (MFormat.at (AssignedSpectralSection[i][3]) == "64QAM")
					network->Numof200SC6++;
			}
	
			if (i != AssignedSpectralSection.size () - 1)
			{
				network->TotalMDataSize += (AssignedSpectralSection[i][2] - AssignedSpectralSection[i][1]) * BW_SPECSLOT;
				TempData -= SCSizes.at (AssignedSpectralSection[i][3]);
			}
			else
			{
				network->TotalMDataSize += (double) TempData / mfTimesArray.at (AssignedSpectralSection[i][3]); 
			}

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
	}

	#ifdef DEBUG_print_resource_state_on_the_path
	for (int i = 0; i < CircuitRoute.size (); i++) {
		cout << CircuitRoute[i] << ' ';
	}
	cout << endl;
	cout << "\033[0;32mPRINT\033[0m " << "resources AFTER Allocation" << endl;
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
	cout << "\033[0;32mPRINT\033[0m " << "resources BEFORE Release" << endl;
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
	cout << "\033[0;32mPRINT\033[0m " << "resources AFTER Release" << endl;

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


