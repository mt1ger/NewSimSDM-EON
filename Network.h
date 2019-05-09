#ifndef _NETWORK_H
#define _NETWORK_H

#include <vector>
// #include "TrafficGenerator.h"
// #include "RoutingTable.h"
#include "EventQueue.h"

#define TESTING

#ifdef TESTING
// #define DEBUG_print_resource_state_on_the_path
// #define DEBUG_print_AvailableSpecSlots
#define DEBUG_print_new_built_Event
#define DEBUG_print_PotentialSections
#define DEBUG_print_SortedSections
#define DEBUG_print_SCinfo // Super Channel info for a specific request 
#define DEBUG_print_SortedSC // Sorted Super Channel based on their performance 
#define DEBUG_collect_EventID_of_blocked_requests //need to collaberate with debug_print_eventid_of_blocked_requests
#define PRINT_allocation_block_release
// #define DEBUG_print_SourceAvailableSections
#endif

#define GB 1 // Guardband
#define NUMOFSPECTRALSLOTS 128
// #define NUMOFSPECTRALSLOTS 16
// #define MAX_DATASIZE_REQUEST 200 
#define MAX_DATASIZE_REQUEST 100 
#define BW_SPECSLOT 12.5

using namespace std;

class RoutingTable;

class Network {
	public:
		/*** Functions ***/  
		void init ();
		void simulation ();


		/*** VARIABLES ***/ 
		// Inputted Variables   
		char FileName[500];
		double Lambda, Mu;
		long long NumofRequests;
		unsigned int NumofCores;
		int SectionNumLimitation;

		// Topology Variables
		unsigned int NumofNodes; 
		vector< vector<double> > NodesWeight;
		vector< vector< vector<int> > > routingTable;

		// Metrics per Request
		double TotalHoldingTime;
		long long TotalTranspondersUsed;
		long long TotalCoresUsed;
		long long TotalGBUsed;
		double AvgHoldingTime;
		double AvgTranspondersUsed;
		double AvgGBUsed;
		double AvgCoresUsed;	
		double SpectrumUtilization;


		// Metrics to Measure Fregmentation 
		double TotalMDataSize; // The total modulated data size.
		long long TotalSS4Data;
		long long TotalSSOccupied;

		double AvgIntFrag;
		double AvgExtFrag;
		double AvgHybridFrag;

		// Timer and counters 
		long long RequestCounter; // Generated Number of Requests
		long long NumofAllocatedRequests;
		long long NumofDoneRequests; // Requests which are successfully allocated and released, or blocked will be considered as DoneRequests
		long long NumofFailedRequests; // Number of Requests that are blocked
		long long NumofTransponders; // Number of TranspondersUsed at a Specific Time
		long long NumofSS4Data;
		long long MaxNumofTransponders; // For one time simulation, Maximum Number of Transponders Used Simultaneously
		long long MaxNumofSS4Data; // For one time simulation, Maximum Number of Data Spectral Slot Occupation
		int NumofSections;
		int MaxNumofSections;
		double SystemClock;

		// Super Channel Allocation Mapping
		long long Numof100SC4; // number of 16QAM 100Gb/s super channel used per simulation
		long long Numof100SC2; // number of QPSK 100Gb/s super channel used per simulation
		long long Numof50SC4; // number of 16QAM 50Gb/s super channel used per simulation
		long long Numof50SC2; // number of QPSK 50Gb/s super channel used per simulation
		long long Numof25SC; // number of 25Gb/s super channel used per simulation
		
		// Resource Variables
		vector< vector< vector< vector<bool> > > > SpectralSlots;

		// Debugging Variables
		vector<int> BlockedRequests;
		
		
	private:
};

#endif
