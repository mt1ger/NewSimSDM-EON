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

/***** HANDLE REQUEST *****/
void ResourceAssignment::handle_requests (CircuitRequest * circuitRequest) {
	RoutingTable routingTable (network);	
	ModulationFormats modulationFormats (circuitRequest, network);

	/*** VARIABLE DECLARATION ***/
	vector<int> CircuitRoute;
	bool AvailableFlag = true;
	vector< vector<int> > AssignedSpectralSection;
	vector<int> HAssignedSpectralSection;
	vector<int> HPotentialSections;
	list< vector<int> > PotentialSections;
	vector<int> HAssignedSS;
	vector< vector<int> > AssignedSS;
	vector< vector<int> > TempSS;
	unsigned int NumofGB = 0;
	unsigned int NumofTransponders = 0; // The reasl used number of transponders - number of guardbands
	string MF = "QPSK";
	unsigned int mfTimes = 0, NumofOccupiedSpectralSlots;
	int NumofNeedGB = 0; // See FuFVFM
	double ReminderBitRate = 0; // See FuFVFM
	bool ReminderFlag = true; // See FuFVFM
	int TempCore = -1;
	int TempSpecSlot = -1;
	unsigned int core;


	NumofOccupiedSpectralSlots = circuitRequest->OccupiedSpectralSlots;
	ReminderBitRate = fmod ((circuitRequest->OccupiedSpectralSlots * 12.5), 100);
	if (ReminderBitRate == 0)
		ReminderFlag = false;
	CircuitRoute = routingTable.get_shortest_path (circuitRequest->Src, circuitRequest->Dest);
	modulationFormats.mf_chosen (CircuitRoute, &NumofOccupiedSpectralSlots, &circuitRequest->DataSize, &MF, &mfTimes);

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
	if (circuitRequest->OccupiedSpectralSlots >= AvailableSpecSlots.size ()) {
		AvailableFlag = false;
	}
	else {
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

		/** Pre-Allocation **/
		Index = SortedSections.begin ();
		while (NumofOccupiedSpectralSlots > 0) {
			for (i = SortedSections.begin (); i != SortedSections.end (); i++) {
				SizeSum += i->at (2) - i->at (1) + 1;
			}
			if (SizeSum <=NumofOccupiedSpectralSlots || Index == SortedSections.end () || SortedSections.empty ()) {
				AvailableFlag = false;
				break;
			}
			else {
				int temp = 0;
				if (ReminderFlag == false)
					temp = NumofOccupiedSpectralSlots;
				else
					temp = NumofOccupiedSpectralSlots - 1;
				
				/* Deal with Demands Less Than 100Gb/s */
				if ((temp * 12.5 * mfTimes + ReminderBitRate) <= 100)
				{
					if (Index->at (2) - Index->at (1) + 1 == NumofOccupiedSpectralSlots + GB) {
						HAssignedSpectralSection.push_back (Index->at (0));
						HAssignedSpectralSection.push_back (Index->at (1));
						HAssignedSpectralSection.push_back (Index->at (2));
						AssignedSpectralSection.push_back (HAssignedSpectralSection);
						NumofGB++;
						break;
					}
					else if (Index->at (2) - Index->at (1) + 1 > NumofOccupiedSpectralSlots + GB) {
						HAssignedSpectralSection.push_back (Index->at (0));
						HAssignedSpectralSection.push_back (Index->at (1));
						HAssignedSpectralSection.push_back (Index->at (1) + NumofOccupiedSpectralSlots + GB - 1); 
						AssignedSpectralSection.push_back (HAssignedSpectralSection);
						NumofGB++;
						break;
					}
					else if (Index->at (2) - Index->at (1) + 1 < NumofOccupiedSpectralSlots + GB) {
						HAssignedSpectralSection.push_back (Index->at (0));
						HAssignedSpectralSection.push_back (Index->at (1));
						HAssignedSpectralSection.push_back (Index->at (2));
						AssignedSpectralSection.push_back (HAssignedSpectralSection);
						NumofOccupiedSpectralSlots -= Index->at (2) - Index->at (1) + 1 - GB;
						HAssignedSpectralSection.clear ();
						Index = SortedSections.erase (Index);
						// Index--;
						NumofGB++;
					}
				}
				else
				{
					int N = NumofOccupiedSpectralSlots;
					while (N > 0)
					{
						if (N * 12.5 * mfTimes > 100)
							N--;
						else
							break;
					}
					NumofNeedGB = ceil ((double) NumofOccupiedSpectralSlots / N);

					if (Index->at (2) - Index->at (1) + 1 == NumofOccupiedSpectralSlots + NumofNeedGB * GB) {

						int IntermediateNum = 0;
						int GBcounter = NumofNeedGB;
						while (Index->at (1) + IntermediateNum + N < Index->at (2) && GBcounter > 1)
						{

							HAssignedSpectralSection.push_back (0);
							HAssignedSpectralSection.push_back (Index->at (1) + IntermediateNum);
							HAssignedSpectralSection.push_back (Index->at (1) + IntermediateNum + N);
							AssignedSpectralSection.push_back (HAssignedSpectralSection);
							HAssignedSpectralSection.clear ();
							IntermediateNum += N + 1;
							GBcounter--;
						}
						HAssignedSpectralSection.push_back (Index->at (0));
						HAssignedSpectralSection.push_back (Index->at (1) + IntermediateNum);
						HAssignedSpectralSection.push_back (Index->at (2));
						AssignedSpectralSection.push_back (HAssignedSpectralSection);
						HAssignedSpectralSection.clear ();
						NumofGB += NumofNeedGB * GB;
						break;
					}
					else if (Index->at (2) - Index->at (1) + 1 > NumofOccupiedSpectralSlots + NumofNeedGB * GB) {
						int IntermediateNum = 0;
						int GBcounter = NumofNeedGB;
						while (((Index->at (1) + IntermediateNum + N) < (Index->at (1) + NumofOccupiedSpectralSlots + NumofNeedGB * GB - 1)) && (GBcounter > 1))
						{
							HAssignedSpectralSection.push_back (0);
							HAssignedSpectralSection.push_back (Index->at (1) + IntermediateNum);
							HAssignedSpectralSection.push_back (Index->at (1) + IntermediateNum + N);
							AssignedSpectralSection.push_back (HAssignedSpectralSection);
							HAssignedSpectralSection.clear ();
							IntermediateNum += N + 1;
							GBcounter--;
						}
						
						HAssignedSpectralSection.push_back (Index->at (0));
						HAssignedSpectralSection.push_back (Index->at (1) + IntermediateNum);
						HAssignedSpectralSection.push_back (Index->at (1) + NumofOccupiedSpectralSlots + NumofNeedGB * GB - 1); 
						AssignedSpectralSection.push_back (HAssignedSpectralSection);
						HAssignedSpectralSection.clear ();
						NumofGB += NumofNeedGB * GB;
						break;
					}
					else if (Index->at (2) - Index->at (1) + 1 < NumofOccupiedSpectralSlots + NumofNeedGB * GB) {
						int IntermediateNum = 0;
						while (Index->at (1) + IntermediateNum + N < Index->at (2) - 1)
						{
							HAssignedSpectralSection.push_back (Index->at (0));
							HAssignedSpectralSection.push_back (Index->at (1) + IntermediateNum);
							HAssignedSpectralSection.push_back (Index->at (1) + IntermediateNum + N);
							AssignedSpectralSection.push_back (HAssignedSpectralSection);
							HAssignedSpectralSection.clear ();
							IntermediateNum += N + 1;
							NumofOccupiedSpectralSlots -= Index->at (1) + IntermediateNum + N - Index->at (1) - IntermediateNum + 1 - GB;
							NumofGB++;
						}
						HAssignedSpectralSection.push_back (Index->at (0));
						HAssignedSpectralSection.push_back (Index->at (1) + IntermediateNum);
						HAssignedSpectralSection.push_back (Index->at (2));
						AssignedSpectralSection.push_back (HAssignedSpectralSection);
						HAssignedSpectralSection.clear ();
						NumofGB++;
						NumofOccupiedSpectralSlots -= Index->at (2) - Index->at (1) - IntermediateNum + 1 - GB;
						Index = SortedSections.erase (Index);
						// Index--;
					}
				}
			}
		}
		if (AssignedSpectralSection.size () > network->SectionNumLimitation) AvailableFlag = false;
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
			int NumofSlots = AssignedSpectralSection[i][2] - AssignedSpectralSection[i][1];
			NumofTransponders = ceil (NumofSlots * 12.5 * mfTimes / 100) - 1;
			for (int p = 1; p < CircuitRoute.size (); p++) {
				for (int j = AssignedSpectralSection[i].at (1); j <= AssignedSpectralSection[i].at (2); j++) {
					network->SpectralSlots[CircuitRoute[p -1]][CircuitRoute[p]][AssignedSpectralSection[i].at (0)][j] = true;
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
		cout << "Source: " << circuitRequest->Src << "  Destination: " << circuitRequest->Dest << "  ModulationFormats: " << MF << endl;
		cout << "Path: ";
		for(unsigned int t = 0; t < CircuitRoute.size() - 1; t++)
			cout << CircuitRoute.at(t) << " --> ";
		cout << CircuitRoute.at (CircuitRoute.size() - 1) << endl;

		for (int i = 0; i < AssignedSpectralSection.size (); i++) {
			cout << "Core: " << AssignedSpectralSection[i][0] << "  Spectral Section: " << AssignedSpectralSection[i][1] << " to " << AssignedSpectralSection[i][2] << endl; 
		}

		cout << "# of Guardbands Used: " << NumofGB << endl;
		cout << "# of Transponders Used: " << NumofGB << endl;
		cout << "# of Core Used: " << CoreCnter << endl;
		cout << "------------------------------------------------------------" << endl;
		#endif

		CircuitRelease * circuitRelease;
		circuitRelease = new CircuitRelease (circuitRequest->EventID, CircuitRoute, AssignedSpectralSection, circuitRequest->StartTime + circuitRequest->Duration, NumofTransponders + NumofGB);
		eventQueue->queue_insert (circuitRelease);

		network->NumofAllocatedRequests++;
		network->NumofSections = network->SectionNumLimitation;
		network->TotalHoldingTime += circuitRequest->Duration;
		network->NumofTransponders += NumofGB;
		network->TotalTranspondersUsed += NumofGB;
		network->TotalCoresUsed += CoreCnter;
		network->TotalGBUsed += NumofGB;
		network->TotalDataSize += circuitRequest->DataSize;
		network->TotalSSUsed += NumofOccupiedSpectralSlots * mfTimes;
		network->TotalSSOccupied += (NumofOccupiedSpectralSlots + NumofGB) * mfTimes;
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


