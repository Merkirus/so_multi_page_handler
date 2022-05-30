#ifndef PROCESSJOINER_H
#define PROCESSJOINER_H

#include <vector>
#include <algorithm>
#include <random>
#include "Proces.h"
#include "Strona.h"
#include "Random.h"

class ProcessJoiner
{
public:
	ProcessJoiner(std::vector<Proces>& v);
	~ProcessJoiner()=default;
	Strona getLastSeqPage();
	Proces getLastSeqProcess(Strona strona);
	std::vector<unsigned> getSeq();
	std::vector<unsigned> getPastSeq();
	std::vector<Proces> getProcesy();
private:
	void generateSeq();
	std::vector<Proces> procesy;
	std::vector<unsigned> sequence;
	std::vector<unsigned> past_sequence;
};

#endif