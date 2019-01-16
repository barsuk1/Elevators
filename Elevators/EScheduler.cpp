#include "EScheduler.h"

unsigned long EScheduler::computePickUpArrival(size_t fromFloor, size_t destFloor, const Status& s)
{
	unsigned long res = abs((long)(s.current - fromFloor));
	unsigned long cnt = 0;
	//fromFloor is somewhere on the way here
	if (s.current < s.next) {

		for (int i = s.current; i <= (int)fromFloor; i++) {
			if (s.floors[i])
				cnt++;
		}
	}
	else {
		for (int i = s.current; i >= (int)fromFloor; i--) {
			if (s.floors[i])
				cnt++;
		}
	}
	//we can give different weights for number of floors and number of stops 
	//here for simplicity we will just sum them up for now
	return res*m_floorWeight + cnt*m_stopWeight;
}


unsigned long EScheduler::computeOppositeArrival(size_t fromFloor, size_t destFloor, const Status& s)
{
	unsigned long res = abs((long)(s.current - fromFloor));
	unsigned long cnt = 0;
	//count all stops in the opposite direction
	if (s.current < s.next) {

		for (int i = s.current; i < (int)s.floors.size(); i++) {
			if (s.floors[i])
				cnt++;
		}
	}
	else {
		for (int i = s.current; i >= 0; i--) {
			if (s.floors[i])
				cnt++;
		}
	}
	//count all stops in the right direction
	if (s.current < fromFloor) {

		for (int i = s.current; i <= (int)fromFloor; i++) {
			if (s.floors[i])
				cnt++;
		}
	}
	else {
		for (int i = s.current; i >= (int)fromFloor; i--) {
			if (s.floors[i])
				cnt++;
		}
	}

	//we can give different weights for number of floors and number of stops 
	//here for simplicity we will just sum them up for now
	return res*m_floorWeight + cnt*m_stopWeight;
}

size_t EScheduler::findMin(size_t fromFloor, size_t destFloor, vector<EScheduler::Status>& s, Direction dir)
{
	//want to stop at fromFloor, go to destFloor
	auto lambda1 = [&](const Status& s1, const Status& s2)
	{
		return abs((long)(fromFloor - s1.current)) < abs((long)(fromFloor - s2.current));
	};

	auto lambda2 = [&](const Status& s1, const Status& s2)
	{
		return this->computePickUpArrival(fromFloor, destFloor, s2) < this->computePickUpArrival(fromFloor, destFloor, s2);
	};

	auto lambda3 = [&](const Status& s1, const Status& s2)
	{
		return this->computeOppositeArrival(fromFloor, destFloor, s2) < this->computePickUpArrival(fromFloor, destFloor, s2);
	};

	switch (dir) {
	case Idle:
		sort(s.begin(), s.end(), lambda1);
		break;
	case PickUp:
		sort(s.begin(), s.end(), lambda2);
		break;
	case Opposite:
		sort(s.begin(), s.end(), lambda3);
		break;
	}
	return 0;
}

void EScheduler::scheduleOne(size_t fromFloor, size_t destFloor)
{
	bool callUp = (fromFloor < destFloor) ? true : false;
	vector<EScheduler::Status> elevIdle, elevPickUp, elevOthers;
	for (size_t i = 0; i < m_elevs.size(); i++) {

		Status stat;
		stat.idx = i;
		size_t res = m_elevs[i].getStatus(stat.current, stat.next, stat.floors);
		if (stat.next == ELEV_IDLE) {
			elevIdle.push_back(stat);
		}
		else {
			if (stat.current < stat.next) {
				if (callUp && stat.current < fromFloor) {
					elevPickUp.push_back(stat);
				}
				else elevOthers.push_back(stat);
			}
			else {
				if (!callUp && fromFloor < stat.current)
					elevPickUp.push_back(stat);
				else elevOthers.push_back(stat);
			}
		}
	}

	unsigned long minval = ULONG_MAX;
	size_t targetElev = 0;
	if (!elevIdle.empty()) {
		size_t idx = findMin(fromFloor, destFloor, elevIdle, Idle);
		minval = abs((long)(elevIdle[0].current - fromFloor))*m_floorWeight;
		targetElev = elevIdle[0].idx;
	}
	if (!elevPickUp.empty()) {
		size_t idx = findMin(fromFloor, destFloor, elevPickUp, PickUp);
		unsigned long rate = this->computePickUpArrival(fromFloor, destFloor, elevPickUp[0]);
		if (rate < minval) {
			targetElev = elevPickUp[0].idx;
			minval = rate;
		}
	}
	m_elevs[targetElev].setStop(fromFloor, destFloor);
}

void EScheduler::getStatus(vector< pair<size_t, size_t> >& positions, vector< vector<bool> >& stops)
{
	for (auto& e : m_elevs) {
		size_t pos, next;
		vector<bool> floors;
		if (!e.getStatus(pos, next, floors)) //print an error?
			continue;
		positions.push_back(make_pair(pos, next));
		stops.push_back(floors);
	}
}
