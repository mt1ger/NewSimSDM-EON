#ifndef _RESOURCEASSIGNMENT_FUFVF_H
#define _RESOURCEASSIGNMENT_FUFVF_H


#include <list>
#include "Event.h"
#include "EventQueue.h"
#include "RoutingTable.h"

class ResourceAssignment {
	public:
		ResourceAssignment (Network * net, EventQueue * eq) {network = net; eventQueue = eq;}
		~ResourceAssignment () {}

		list< vector<int> > AvailableSpecSlots;

		void handle_requests (CircuitRequest * circuitRequest);
		void handle_releases (CircuitRelease * circuitRelease);
		void check_availability_source (unsigned int predecessor, unsigned int successor, CircuitRequest * circuitRequest);
		void check_availability_link (vector<int> * CircuitRoute); 
		vector<int> compute_sort_superchannels ();
		void sort_sections (list< vector<int> > & OriginalList);

	private:
		Network * network;
		EventQueue * eventQueue;
};

#endif


