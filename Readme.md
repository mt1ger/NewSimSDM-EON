**************************************************
* Simulator of SDM-EON
* Author: Yue Wang
* Version: 1.1
**************************************************


**************************************************
#Global instruction:
1. When compile:
	a. Make sure in Network.cpp, choosing right header of ResourceAssignment
	b. Make sure using right makefile
	c. Make sure in Plot.cpp, choosing right executable of Sim
**************************************************


**************************************************
# Version Information
## Version 1.0:
1. Time slot did not considered.
2. First-fit applied.
3. Core continuity constraint applied.
4. Spectral Slot continuity constraint applied.
5. Spectral Slot contiguity constraint applied.
6. Unicast applied.



## Version 1.1:
1. Time slot did not considered.
2. First-fit applied.
3. Core continuity constraint applied.
4. Spectral Slot continuity constraint applied.

5. Modified from Version 1.0
	a. Flex-grid SDM allocation scheme applied
	b. Fully flexible allocation scheme applied
	c. Isolated Core SDM allocation scheme applied 

6. Unicast applied.



## Version 1.2:
1. Time slot did not considered.
2. First-fit applied.
3. Core continuity constraint applied.
4. Spectral Slot continuity constraint applied.
5. Allocation Schemes:
	a. Flex-grid SDM allocation scheme implemented. 
	b. Fully Flexible allocation scheme implemented. 
	c. Isolated Core SDM allocation scheme implemented.
	d. Isolated Core SDM without contiguity constraint applied  alloction scheme implemented.
	e. Fixed Flexible without contiguity constraint alloction scheme implemented.
6. Unicast applied.
7. Metrics Measured  
	a. Tranponders Used per Request
	b. Cores Used per Request
	c. Guardbands Used per Request
	d. HoldingTime per Request
	e. Average Internal Fragmentation
	f. Average External Fragmentation
	g. Average Hybrid Fragmentaion



## Version 1.2.5:
1. Time slot did not considered.
2. First-fit applied.
3. Core continuity constraint applied.
4. Spectral Slot continuity constraint applied.
5. Allocation Schemes:
	a. Flex-grid SDM allocation scheme implemented. 
	b. Fully Flexible allocation scheme implemented. 
	c. Isolated Core SDM allocation scheme implemented.
	d. Isolated Core SDM without contiguity constraint applied  alloction scheme implemented.
	e. Fixed Flexible without contiguity constraint alloction scheme implemented.
6. Unicast applied.
7. Metrics Measured  
	a. Tranponders Used per Request
	b. Cores Used per Request
	c. Guardbands Used per Request
	d. HoldingTime per Request
	e. Average Internal Fragmentation
	f. Average External Fragmentation
	g. Average Hybrid Fragmentaion

/* Modified from Version 1.2 */
Make Plot scripts more advanced.




## Version 1.3:
1. Time Slot did not considered.
2. We have:
	a. First-Fit
		(a) Fixed Flex: "FixedFlex" & "FiFM"
		(b) Fully Flex: "FullyFlex" & "FFM"
		(c) Isolated Core: "IsolatedCore" & "ICM"
	b. Bandwidth-Fit="FuFVF" & "FuFVFM"
		both of the 2 are Fully Flex
	c. Largest-Fit="FuFBF" & "FuFBFM"
		both of the 2 are Fully Flex
3. Continuity constrait applied.
4. If an algorithm ends with 'M', it means it is number-of-seg-variable
5. Unicast applied.
6. Metrics Measured  
	a. Tranponders Used per Request
	b. Cores Used per Request
	c. Guardbands Used per Request
	d. HoldingTime per Request
	e. Average Internal Fragmentation
	f. Average External Fragmentation
	g. Average Hybrid Fragmentaion
/* Modified from Version 1.25 */ 
1. New build a modulation Table
2. New way to count number of transponders
	The max bit rate of each sub-BVT is 100Gb/s
**************************************************




## 04/19/2019
1. Now the Max Bit Rate of request is 100.
2. SC options are: 100, 50, 25
3. MF options are: QPSK, 16QAM 


## 10/14/2019
1. Now the traffic generator only generate 3 types of request: 40Gb/s, 100Gb/s, 400Gb/s
2. SC options are: 200, 100, 50, 25 
3. MF options are: QPSK, 16QAM, and 64QAM


