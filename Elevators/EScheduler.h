#pragma once
#include "Elevator.h"
class EScheduler {
public:
	EScheduler(size_t numElevators, size_t nfloors, size_t floorWeight = 1, size_t stopWeight = 1) :
		m_elevs(numElevators, nfloors),
		m_floorWeight(floorWeight),
		m_stopWeight(stopWeight)
	{
		if (!numElevators || !nfloors)
			throw std::invalid_argument("Number of Elevators and Floors Must be POSITIVE");
		//should check if there is an overflow nfloors * weight 
		if (floorWeight > 100 || stopWeight > 100)
			throw std::invalid_argument("Weight must be an integer between [1 , 100]");
	}
	void scheduleOne(size_t fromFloor, size_t destFloor);

	void step()
	{
		for (auto& e : m_elevs)
			e.simulateStep();
	}

	void getStatus(vector< pair<size_t, size_t> >& pos, vector< vector<bool> >& stops);
private:

	size_t m_floorWeight;
	size_t m_stopWeight;

	vector<Elevator> m_elevs;
	struct Status {
		size_t idx;
		size_t current;
		size_t next;
		vector<bool> floors;
	};
	enum Direction { Idle, PickUp, Opposite };
	size_t findMin(size_t fromFloor, size_t destFloor, vector<EScheduler::Status>& s, Direction dir);
	unsigned long computePickUpArrival(size_t fromFloor, size_t destFloor, const Status& s);
	unsigned long computeOppositeArrival(size_t fromFloor, size_t destFloor, const Status& s);
};

