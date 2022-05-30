#ifndef FIFO_H
#define FIFO_H

#include "Pamiec.h"
#include "Proces.h"
#include "ProcessJoiner.h"

class FIFO
{
public:
	FIFO(Pamiec pamiec, ProcessJoiner proces);
	~FIFO() = default;
	void run(int n);
	void runDyn(int n);
	void runWSS(int n);
private:
	std::vector<std::vector<unsigned>> makeWSS();
	Pamiec pamiec;
	ProcessJoiner proces;
};

#endif /*FIFO_H*/