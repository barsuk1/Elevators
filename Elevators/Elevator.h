#pragma once
#include "stdafx.h"
using namespace std;

#define ELEV_IDLE -1 

class Elevator {
public:
	Elevator(size_t nfloors) :m_floors(nfloors, false), m_pending(nfloors), m_currentFloor(0), m_destFloor(ELEV_IDLE) {}
	bool setStop(size_t floor, size_t dest);
	size_t getNextStop();
	size_t getStatus(size_t& curr, size_t& dest, vector<bool>& stops);
	void simulateStep();
private:
	void checkNextFloorUp();
	void checkNextFloorDown();
	void updateFromPendingStops();
private:
	//floors with the current stops on them
	vector<bool> m_floors;
	//pending stops are described by a pair (from, to) . the "to" floor will be set as stop once the elevator will arrive at the "from" floor
	//note also that there can be a number of (from, to) pairs with the same "from" set on an elevator.
	vector< vector<int> > m_pending;
	size_t m_currentFloor;
	size_t m_destFloor;
};
