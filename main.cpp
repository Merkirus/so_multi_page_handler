#include <time.h>
#include <random>
#include <vector>
#include <iostream>
#include "Random.h"
#include "Proces.h"
#include "ProcessJoiner.h"
#include "FIFO.h"
#include "Pamiec.h"

using namespace std;

int main(int argc, char const *argv[])
{
	srand(time(NULL));
	
	Proces p1{};
	Proces p2{};
	// Proces p3{};
	// Proces p4{};

	/*Liczba stron dla procesu*/
	p1.generatePages(2);
	p2.generatePages(4);
	// p3.generatePages(5);
	// p4.generatePages(5);

	/*Sekwencja odwołań*/
	p1.generateSeq(100);
	p2.generateSeq(100);
	// p3.generateSeq(200);
	// p4.generateSeq(200);

	/*Liczba ramek*/

	std::vector<Proces> v{p1, p2};
	ProcessJoiner p{v};

	/*Ilość analizowanych procesów*/
	int n = 0;
	for (int i = 0; i < v.size(); ++i)
		n += v.at(i).getSeq().size();


	Pamiec pam{3, v};

	FIFO fifo{pam, p};

	cout << "FIFO: " << '\n';
	fifo.runDyn(n);

	return 0;
}