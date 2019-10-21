#include "TrafficGenerator.h"
#include <cmath>


using namespace std;


void TrafficGenerator::gen_unicast_sd (unsigned int * src, unsigned int * dest) {
	do {
		* src = uniform_rv (network->NumofNodes);
	} while (* src == network->NumofNodes);


	do {
		* dest = uniform_rv (network->NumofNodes);
	} while ((* dest == * src) || (* dest == network->NumofNodes));
	
}


void TrafficGenerator::gen_temporal_parameters (double * time, double LorM) {
	* time = exponential_rv (LorM);
}


/**** Generates uniformly distributed datasize of request from 1 to MAX_DATASIZE_REQUEST ****/ 
// void TrafficGenerator::gen_first_request () {
// 	unsigned int src, dest, occupiedSpectralSlots, datasize;
// 	double startTime = 0;
// 	double duration;
// 	CircuitRequest * request;
//
// 	gen_unicast_sd (&src, &dest);
// 	gen_temporal_parameters (&duration, network->Mu);
// 	do {
// 		datasize = uniform_rv (MAX_DATASIZE_REQUEST);
// 	} while (datasize == 0);
// 	occupiedSpectralSlots = ceil ((double) datasize / BW_SPECSLOT);
//
// 	request = new CircuitRequest (src, dest, startTime, duration, datasize, occupiedSpectralSlots, network->RequestCounter);
//
// 	network->RequestCounter++;
// 	#ifdef DEBUG_print_new_built_Event
// 	cout << "\tNew Built event:" << endl;
// 	cout << '\t' << request->Src << ' ' << request->Dest << ' ' << request->StartTime << ' ' << request->Duration << ' ' << request->DataSize << ' ' << request->OccupiedSpectralSlots << endl;
// 	#endif
//
// 	eventQUeue->ev_Queue.push_back (request);
// }
//releases' generation will be taken care of in "handle_requests" in "ResourceAssignment.cpp" 
// void TrafficGenerator::gen_request (double systemTime) {
// 	unsigned int src, dest, datasize, occupiedSpectralSlots;
// 	double time, startTime, duration;
// 	CircuitRequest * request;
//
// 	gen_unicast_sd (&src, &dest);
// 	gen_temporal_parameters (&duration, network->Mu);
// 	gen_temporal_parameters (&time, network->Lambda);
// 	do {
// 		datasize = uniform_rv (MAX_DATASIZE_REQUEST);
// 	} while (datasize == 0);
// 	occupiedSpectralSlots = ceil ((double) datasize / BW_SPECSLOT);
// 	startTime = systemTime + time;
//
// 	request = new CircuitRequest (src, dest, startTime, duration, datasize, occupiedSpectralSlots, network->RequestCounter);
//
// 	network->RequestCounter++;
//
// 	#ifdef DEBUG_print_new_built_Event
// 	cout << "\tNew Built event:" << endl;
// 	cout << '\t' << request->EventID << ' ' <<request->Src << ' ' << request->Dest << ' ' << request->StartTime << ' ' << request->Duration << ' ' << request->DataSize << ' ' << request->OccupiedSpectralSlots << endl;
// 	#endif
//
// 	eventQUeue->queue_insert (request);
// }


/**** Generates request datasize of the 3 types: 40Gb/s, 100Gb/s and 400Gb/s, AND 20% FEC has been considered ****/
void TrafficGenerator::gen_first_request () {
	unsigned int src, dest, occupiedSpectralSlots, datasize = 0;
	double startTime = 0;
	double duration;
	int type = 0; // 1 represents 40Gb/s, 2 for 100Gb/s, and 3 for 400Gb/s
	CircuitRequest * request;

	gen_unicast_sd (&src, &dest);
	gen_temporal_parameters (&duration, network->Mu);
	do {
		do {
			type = uniform_rv (4);
		} while (type == 0);
		switch (type) 
		{
			case 1: 
				if (network->request_40 > 0)
				{
					network->request_40--;
					datasize = 40;
				}
				else
					datasize = 0;
			break;
			case 2: 
				if (network->request_100 > 0)
				{
					network->request_100--;
					datasize = 100;
				}
				else
					datasize = 0;
			break;
			case 3: 
				if (network->request_400 > 0)
				{
					network->request_400--;
					datasize = 400;
				}
				else
					datasize = 0;
			break;
		}
	} while (datasize == 0);
	int transferBand = (double) datasize / (1 - FEC);
	occupiedSpectralSlots = ceil (((double) datasize / (1 - FEC)) / BW_SPECSLOT);

	request = new CircuitRequest (src, dest, startTime, duration, datasize, occupiedSpectralSlots, network->RequestCounter);

	network->RequestCounter++;
	#ifdef DEBUG_print_new_built_Event
	cout << "\tNew Built event:" << endl;
	// cout << '\t' << request->Src << ' ' << request->Dest << ' ' << request->StartTime << ' ' << request->Duration << ' ' << request->DataSize << ' ' << transferBand << ' ' << request->OccupiedSpectralSlots << endl;
	cout << '\t' << request->Src << ' ' << request->Dest << ' ' << request->StartTime << ' ' << request->Duration << ' ' << request->DataSize << ' ' << request->OccupiedSpectralSlots << endl;
	#endif

	eventQUeue->ev_Queue.push_back (request);
}

void TrafficGenerator::gen_request (double systemTime) {
	unsigned int src, dest, datasize, occupiedSpectralSlots;
	double time, startTime, duration;
	int type = 0; // 1 represents 40Gb/s, 2 for 100Gb/s, and 3 for 400Gb/s
	CircuitRequest * request;

	gen_unicast_sd (&src, &dest);
	gen_temporal_parameters (&duration, network->Mu);
	gen_temporal_parameters (&time, network->Lambda);
	do {
		do {
			type = uniform_rv (4);
		} while (type == 0);
		switch (type) 
		{
			case 1: 
				if (network->request_40 > 0)
				{
					network->request_40--;
					datasize = 40;
				}
				else
					datasize = 0;
			break;
			case 2: 
				if (network->request_100 > 0)
				{
					network->request_100--;
					datasize = 100;
				}
				else
					datasize = 0;
			break;
			case 3: 
				if (network->request_400 > 0)
				{
					network->request_400--;
					datasize = 400;
				}
				else
					datasize = 0;
			break;
		}
	} while (datasize == 0);
	int transferBand = (double) datasize / (1 - FEC); 
	occupiedSpectralSlots = ceil (((double) datasize / (1 - FEC)) / BW_SPECSLOT);
	startTime = systemTime + time;

	request = new CircuitRequest (src, dest, startTime, duration, datasize, occupiedSpectralSlots, network->RequestCounter);
	network->RequestCounter++;

	#ifdef DEBUG_print_new_built_Event
	cout << "\tNew Built event:" << endl;
	// cout << '\t' << request->EventID << ' ' <<request->Src << ' ' << request->Dest << ' ' << request->StartTime << ' ' << request->Duration << ' ' << request->DataSize << ' ' << transferBand << ' ' << request->OccupiedSpectralSlots << endl;
	cout << '\t' << request->EventID << ' ' <<request->Src << ' ' << request->Dest << ' ' << request->StartTime << ' ' << request->Duration << ' ' << request->DataSize << ' ' <<  request->OccupiedSpectralSlots << endl;
	#endif

	eventQUeue->queue_insert (request);
}
