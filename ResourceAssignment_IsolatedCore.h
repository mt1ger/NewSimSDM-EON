#ifndef _RESOURCEASSIGNMENT_H
#define _RESOURCEASSIGNMENT_H


#include "Event.h"
#include "EventQueue.h"
#include "RoutingTable.h"

class ResourceAssignment {
	public:
		ResourceAssignment (Network * net, EventQueue * eq) {network = net; eventQueue = eq;}
		~ResourceAssignment () {}

		vector< vector<int> > SourceAvailableSections;

		void handle_requests (CircuitRequest * circuitRequest);
		void handle_releases (CircuitRelease * circuitRelease);
		void check_availability_source (unsigned int predecessor, unsigned int successor, CircuitRequest * circuitRequest);
		void check_availability_link (unsigned int predecessor, unsigned int successor, int i, bool * AvailableFlag, int * TempCore, int * TempSpecSlot); 

	private:
		Network * network;
		EventQueue * eventQueue;
};

#endif
