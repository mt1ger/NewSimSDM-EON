#include "EventQueue.h" 


using namespace std;


void EventQueue::queue_insert (Event * event) {
	list<Event *>::iterator i = ev_Queue.end ();

	i--;
		while (i != ev_Queue.begin ()) {
			if (event->EventTime > (*i)->EventTime) {
				i++;
				ev_Queue.insert (i, event);	
				break;
			}
			i--;
		}

		if (i == ev_Queue.begin ()) {
			if (event->EventTime > (*i)->EventTime) {
				i++;
				ev_Queue.insert (i, event);
			}
			else ev_Queue.push_front (event);
		}
}
