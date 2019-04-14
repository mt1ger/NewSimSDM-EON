#ifndef _EVENTQUEUE_H
#define _EVENTQUEUE_H


#include <vector>
#include <list> 
#include "Event.h" 


class EventQueue {
	public:
		EventQueue () {};
		~EventQueue () {};

		list<Event *> ev_Queue;

		void queue_insert (Event * event);
};

#endif 
