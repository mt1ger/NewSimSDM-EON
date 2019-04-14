#include "Event.h"


CircuitRequest::CircuitRequest (unsigned int src, unsigned int dest, double startTime, double duration, unsigned int dataSize, unsigned int occupiedSpectralSlots, long long eventID) {
	Src = src;
	Dest = dest;
	StartTime = startTime;
	Duration = duration;
	EventTime = startTime;
	EventID = eventID;
	EventType = c_Request;	
	OccupiedSpectralSlots = occupiedSpectralSlots;
	DataSize = dataSize;
}


CircuitRelease::CircuitRelease (long long eventID, vector<int> &circuitRoute, vector< vector<int> > &occupiedSpectralSection, double releaseTime, unsigned int transpondersUsed) {
	CircuitRoute = circuitRoute;
	OccupiedSpectralSection = occupiedSpectralSection;
	ReleaseTime = releaseTime;
	EventTime = releaseTime;
	EventType = c_Release;
	EventID = eventID;
	TranspondersUsed = transpondersUsed;
}
