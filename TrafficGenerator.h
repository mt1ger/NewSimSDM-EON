#ifndef _TRAFFICGENERATOR_H
#define _TRAFFICGENERATOR_H


#include "RandomVariable.h"
#include "EventQueue.h"
#include "Network.h"


class TrafficGenerator {
	public:
		TrafficGenerator (Network * net, EventQueue * eq) {network = net; eventQUeue = eq;}
		~TrafficGenerator () {}

		void gen_unicast_sd (unsigned int * src, unsigned int * dest);
		void gen_temporal_parameters (double * time, double LorM); 
		void gen_first_request ();
		void gen_request (double systemTime);

	private:
		Network * network;
		EventQueue * eventQUeue;
};

#endif
