// Elevators.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"







#include "EScheduler.h"

void printStatus(EScheduler& sched)
{
	vector< pair<size_t, size_t> > pos;
	vector<vector<bool> > floors;
	sched.getStatus(pos, floors);

	if (floors.empty() ) 
		return;

	printf("\t\tElevators where C - Current Floor, N - Dest Floor, * - Stop \n");
	printf("\t\t\t");
	for (size_t i = 0; i < pos.size(); i++)
		printf("%-5lu", i);
	printf("\n");
	printf("Floor\n");
	for (int i = floors[0].size() - 1; i >= 0; i--) {
		printf("\t\t%-5d", i );
		for (size_t f = 0; f < floors.size(); f++) {
			if (!f)
				printf("\t");
			if (pos[f].first == (size_t)i) {
				printf("%-5c", 'C');
			}
			else if (pos[f].second == (size_t)i)
				printf("%-5c", 'N');
			else if (floors[f][i])
				printf("%-5c", '*');
			else
				printf("     ");
		}
		printf("\n");
	}
}


int main()
{
	string s;
	size_t elevators = 0, floors = 0;
	cout << "Number of elevators? :\n";
	cin >> elevators;
	cout << "Number of floors? :\n";
	cin >> floors;
	getline(cin, s);
	try {
		EScheduler sched(elevators, floors);
		while (true) {
			
			cout << "Step - s, Call An Elevator - c, Print - p, Quit - q\n";
			getline(cin, s);
			switch (tolower(s[0])) {
			case 's':
				sched.step();
				break;
			case 'c':
			{
				size_t floor, dest;
				cout << "From floor :\n";
				cin >> floor;
				cout << "Go to floor :\n";
				cin >> dest;
				getline(cin, s);
				sched.scheduleOne(floor, dest);
			}
			break;
			case 'p':
				printStatus(sched);
				break;
			case 'q':
				exit(0);
			}
		}
	}
	catch (std::exception& e) {
		cout << "exception: " << e.what() << endl;
	}
    return 0;
}

