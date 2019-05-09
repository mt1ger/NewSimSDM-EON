// #define DEBUG_test_sequence_in_queue
// #define DEBUG_probe_NumofDoneRequests_and_NumofRequests
#define DEBUG_print_EventID_of_blocked_requests

#include <iostream> 
#include "RoutingTable.h"
#include "Network.h"
#include "TrafficGenerator.h"
#include "Event.h"

// #include "ResourceAssignment_FuFBF.h"
// #include "ResourceAssignment_FuFVF.h"
#include "ResourceAssignment_FullyFlex.h"
// #include "ResourceAssignment_IsolatedCore.h"
// #include "ResourceAssignment_ICM.h"
// #include "ResourceAssignment_FixedFlex.h"
// #include "ResourceAssignment_FiFM.h"



using namespace std;

void Network::init () {
	vector<bool> SpectralSlotIndex;
	vector< vector<bool> > CoreIndex;
	vector< vector< vector<bool> > > SuccessorIndex;

	RoutingTable routinGTable (this);   
	routinGTable.generate_routing_table ();

	for (int i = 0; i < NumofNodes; i++) {
		for (int j = 0; j < NumofNodes; j++) {
			for (int c = 0; c < NumofCores; c++) {
				for (int k = 0; k < NUMOFSPECTRALSLOTS; k++) {
					SpectralSlotIndex.push_back (false);
				}
				CoreIndex.push_back (SpectralSlotIndex);
				SpectralSlotIndex.clear ();
			}
			SuccessorIndex.push_back (CoreIndex);
			CoreIndex.clear ();
		}
		SpectralSlots.push_back (SuccessorIndex);
		SuccessorIndex.clear ();
	}

	SystemClock = 0;
	RequestCounter = 0;
	NumofDoneRequests = 0;
	NumofFailedRequests = 0;
	NumofAllocatedRequests = 0;
	NumofTransponders = 0;
	NumofSS4Data = 0;
	MaxNumofTransponders = 0;
	MaxNumofSS4Data = 0;

	TotalHoldingTime = 0;
	TotalTranspondersUsed = 0;
	TotalCoresUsed = 0;
	TotalGBUsed = 0;
	TotalSS4Data= 0;
	TotalSSOccupied = 0;
	TotalMDataSize = 0;
	AvgExtFrag = 0;
	AvgIntFrag = 0;
	AvgHybridFrag = 0;
		
	Numof100SC4 = 0;
	Numof100SC2 = 0;
	Numof50SC4 = 0;
	Numof50SC2 = 0;
	Numof25SC = 0;
}

void Network::simulation () {
	EventQueue *eventQueue = new EventQueue ();
	TrafficGenerator trafficGenerator (this, eventQueue);
	ResourceAssignment resourceAssignment (this, eventQueue);


	// Generate the first event
	trafficGenerator.gen_first_request ();

	while (!eventQueue->ev_Queue.empty ()) {
		Event * event;
		event = eventQueue->ev_Queue.front ();

		if (SystemClock <= event->EventTime) {
			SystemClock = event->EventTime;
		}

		#ifdef DEBUG_test_sequence_in_queue
		// The code to test if Events in Queue is in right order
		list<Event*>::iterator iter;
		cout << "Print Event key properties in Queue" << endl;
		for (iter = eventQueue->ev_Queue.begin (); iter != eventQueue->ev_Queue.end (); iter++) {
			cout << (*iter)->EventID << ' ' ;
			if ((*iter)->EventType == c_Release) cout << "Release" << ' ';
			else if ((*iter)->EventType == c_Request) cout << "Request" << ' ';
			cout << (*iter)->EventTime << '\t';
		}
		cout << endl;
		#endif


		if (event->EventType == c_Request) {
			resourceAssignment.handle_requests ((CircuitRequest *) event);
			if (RequestCounter != NumofRequests) 
			trafficGenerator.gen_request (SystemClock);
		}
		else if (event->EventType == c_Release) {
			resourceAssignment.handle_releases ((CircuitRelease *) event);
		}

		eventQueue->ev_Queue.pop_front (); //This will destroy the poped Event *.
		if (NumofTransponders > MaxNumofTransponders) MaxNumofTransponders = NumofTransponders;
		if (NumofSections > MaxNumofSections) MaxNumofSections = NumofSections;
		if (NumofSS4Data > MaxNumofSS4Data) MaxNumofSS4Data = NumofSS4Data;

	#ifdef DEBUG_probe_NumofDoneReqeusts_and_NumofRequests
		cout << " " << NumofDoneRequests << " and " << NumofRequests << endl;
	#endif
		if ((NumofFailedRequests + NumofAllocatedRequests) == NumofRequests) break;
	}

	cout << endl << "************************************************************" << endl;
	#ifdef DEBUG_print_EventID_of_blocked_requests
	cout << "Start to print EventID of blocked reqeusts" << endl;
	for (int i = 0; i < BlockedRequests.size (); i++) {
		cout << BlockedRequests[i] << ' ';
	}
	cout << endl;
	#endif
	
	SpectrumUtilization = (double) MaxNumofSS4Data / NUMOFSPECTRALSLOTS;  
	AvgHoldingTime = TotalHoldingTime / NumofAllocatedRequests;
	AvgTranspondersUsed = (double) TotalTranspondersUsed / NumofAllocatedRequests;
	AvgCoresUsed = (double) TotalCoresUsed / NumofAllocatedRequests;
	AvgGBUsed = (double) TotalGBUsed / NumofAllocatedRequests;
	AvgIntFrag = (1 - ((double) TotalMDataSize / (TotalSS4Data * BW_SPECSLOT)));
	AvgExtFrag = (1 - (double) TotalSS4Data / TotalSSOccupied);
	AvgHybridFrag = (1 - (double) TotalMDataSize / (TotalSSOccupied * BW_SPECSLOT));


	cout << "Spectrum Utilization is: " << SpectrumUtilization << endl;
	cout << "Utilization of different SCs (100: QPSK, 100: 16QAM, 50: QPSK, 50: 16QAM, 25): " << Numof100SC2 << ' ' << Numof100SC4 << ' ' << Numof50SC2 << ' ' << Numof50SC4 << ' ' << Numof25SC << endl;
	cout << "Max # of Transponders used: " << MaxNumofTransponders << endl;
	cout << "Max # of Sections used for each request: " << MaxNumofSections << endl;
	cout << "# of blocked requests is " << NumofFailedRequests << endl;
	cout << "Network Load: " << Lambda / Mu << " Erlang" << endl; 
	cout << "Blocking Probability: " << (double) NumofFailedRequests / (double) NumofRequests << endl;
	cout << "Average Cores Used Used per Request: " << AvgCoresUsed << endl;
	cout << "Average Transponders Used per Request: " << AvgTranspondersUsed << endl;
	cout << "Average Holding Time per Request: " << AvgHoldingTime << endl;
	cout << "Average GuardBand per Request: " << AvgGBUsed << endl;
	cout << "Average Internal Fragmentation: " << AvgIntFrag << endl;
	cout << "Average External Fragmentation: " << AvgExtFrag << endl;
	cout << "Average Hybrid Fragmentation: " << AvgHybridFrag << endl; 
}

