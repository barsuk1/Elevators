#include "Elevator.h"

void Elevator::checkNextFloorUp()
{
	for (int i = m_currentFloor; i < m_floors.size(); i++) {
		if (m_floors[i]) {
			m_destFloor = i;
			break;
		}
	}
}

void Elevator::checkNextFloorDown()
{
	for (int i = m_currentFloor; i >= 0; i--) {
		if (m_floors[i]) {
			m_destFloor = i;
			break;
		}
	}
}

void Elevator::updateFromPendingStops()
{
	for (auto& i : m_pending[m_currentFloor])
		m_floors[i] = true;
	m_pending[m_currentFloor].clear();
}

bool Elevator::setStop(size_t floor, size_t dest)
{
	//sanity check(s)
	if (floor >= m_floors.size() || dest >= m_floors.size()) {
		cout << "Call to/from an invalid floor" << endl;
		return false;
	}
	//already stopping on these floors?
	if (m_floors[floor] && m_floors[dest])
		return true;

	
	//we already on this floor - update and return
	if (floor == m_currentFloor) { 
		updateFromPendingStops();
		if (m_destFloor == -1)
			m_destFloor = dest;
		return true;
	}
	//first set a stop on the pikcup floor and destination
	m_floors[floor] = true;
	m_pending[floor].push_back(dest);
	//the new target is above the current pos
	if (floor > m_currentFloor) {
		//wich way we are heading?
		if (m_destFloor < m_currentFloor) { //we are still on a way down , just set another floor above current 
			return true;
		}
		//else we are also going up
		if (floor < m_destFloor) { //new dest floor
			m_destFloor = floor;
		}
		//otherwise the floor is above destFloor we are done
		return true;
	}
	if (floor < m_currentFloor) {
		if (m_destFloor > m_currentFloor) { //we are still on a way up , just set another floor below current 
			return true;
		}
		//else we are also going down
		if (floor > m_destFloor) { //new dest floor
			m_destFloor = floor;
		}
		//
		return true;
	}
	return true;
}

size_t Elevator::getNextStop()
{
	return m_destFloor;
}

size_t Elevator::getStatus(size_t& curr, size_t& next, vector<bool>& stops)
{
	curr = m_currentFloor;
	next = m_destFloor;

	stops = m_floors;
	//count all stops - both already set and still pending
	for (auto& v : m_pending)
		if(!v.empty() )
			for (auto& i : v)
				stops[i] = true;
	return m_floors.size();
}



void Elevator::simulateStep()
{
	if (m_destFloor == -1)//while idle - stay at the same floor
		return;
	bool goingUp = false;
	if (m_currentFloor <= m_destFloor)
		goingUp = true;
	
	
	//should check border conditions - just for sanity
	if (goingUp) {
		if (m_currentFloor == m_floors.size() - 1)
			cerr << "Invalid upper floor state" << endl;
		else
			m_currentFloor++;
	}
	else {
		if (m_currentFloor == 0)
			cerr << "Invalid lower floor state" << endl;
		else
			m_currentFloor--;
	}

	if (m_floors[m_currentFloor]) {
		m_floors[m_currentFloor] = false;
	}

	if (m_currentFloor == m_destFloor) {
		updateFromPendingStops();
		m_destFloor = -1;
		if (goingUp) {
			checkNextFloorUp();
			if (m_destFloor == -1)
				checkNextFloorDown();

		}
		else {
			checkNextFloorDown();
			if (m_destFloor == -1)
				checkNextFloorUp();
		}
	}
}