#include <iostream>
#include <string>
#include "ModulationFormats1.h"

using namespace std;

/****************************************
 * Options for Super Channels: 25 50 100
 * Options for Modulation Formats: 
 *     QPSK, 16QAM, 64QAM  
 ****************************************/


unsigned int ModulationFormats::spectralslots_computation (unsigned int BitsPerSymbol, unsigned int BitRate) {
	unsigned int SpectralSlots;
	SpectralSlots = (double)BitRate / BitsPerSymbol / 12.5; 
	return SpectralSlots;
}


double ModulationFormats::search_link_weight (unsigned int predecessor, unsigned int successor) {
	return network->NodesWeight[predecessor][successor];
}


void ModulationFormats::mf_chosen (vector<int> & shortestPath, unsigned int * occupiedSpectralSlots, unsigned int * dataSize, string *MF, unsigned int *mfTimes) {
	double TotalDist = 0; 
	double Dist = 0;
	unsigned int am_SpectralSlots;
	int SC; // Selected Super Channel Size
	vector<int> SCSizes; // Super Channel Sizes

	/*** Compute the total distance from source to destination ***/
	for (int i = 1; i < shortestPath.size (); i++) {
		Dist = search_link_weight (shortestPath[i - 1], shortestPath[i]);
		TotalDist = TotalDist + Dist;
	}

	SC = * dataSize;
	switch (SC) 
	{
		case 25:		
			if (TotalDist <= 22160)
			{
				m_Format = QPSK;
				*mfTimes = 2;
				am_SpectralSlots = spectralslots_computation (*mfTimes, 25);
				*MF = "QPSK";
			}
			else
			{
				m_Format = Failure;
				*mfTimes = -1;
				*MF = "Fail";
				am_SpectralSlots = -1; 
			}
		break;
		case 50:
			if (TotalDist > 4750 && TotalDist <= 11080) {
				m_Format = QPSK;
				*mfTimes = 2;
				am_SpectralSlots = spectralslots_computation (*mfTimes, 50);
				*MF = "QPSK";
			}
			else if (TotalDist <= 4750) {
				m_Format = QAM16;
				*mfTimes = 4;
				*MF = "16QAM";
				am_SpectralSlots = spectralslots_computation (*mfTimes, 50);
			}
			else
			{
				m_Format = Failure;
				*mfTimes = -1;
				*MF = "Fail";
				am_SpectralSlots = -1; 
			}
		break;
		// case 75:
		// 	if (TotalDist > 3166 && TotalDist <= 7387) {
		// 		m_Format = QPSK;
		// 		*mfTimes = 2;
		// 		am_SpectralSlots = spectralslots_computation (*mfTimes, 75);
		// 		*MF = "QPSK";
		// 	}
		// 	else if (TotalDist > 1221 && TotalDist <= 3166) {
		// 		m_Format = QAM16;
		// 		*mfTimes = 4a
		// 		*MF = "16QAM";
		// 		am_SpectralSlots = spectralslots_computation (*mfTimes, 75);
		// 	}
		// 	else if (TotalDist <= 1221) {
		// 		m_Format = QAM64;
		// 		*mfTimes = 6;
		// 		*MF = "64QAM";
		// 		am_SpectralSlots = spectralslots_computation (*mfTimes, 75);
		// 	}
		// break;
		case 100:
			if (TotalDist > 2375 && TotalDist <= 5540) {
				m_Format = QPSK;
				*mfTimes = 2;
				am_SpectralSlots = spectralslots_computation (*mfTimes, 100);
				*MF = "QPSK";
			}
			else if (TotalDist <= 2375) {
				m_Format = QAM16;
				*mfTimes = 4;
				*MF = "16QAM";
				am_SpectralSlots = spectralslots_computation (*mfTimes, 100);
			}
			else
			{
				m_Format = Failure;
				*mfTimes = -1;
				*MF = "Fail";
				am_SpectralSlots = -1; 
			}
		break;
	}

	*occupiedSpectralSlots = am_SpectralSlots;
}

