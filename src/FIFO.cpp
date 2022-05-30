#include "FIFO.h"
#include <iostream>

#define PROG_SZAMOTANIA 10
#define OKNO_CZASOWE 10
#define DOLNY_PROG_PPF 2
#define GORNY_PROG_PPF 8

FIFO::FIFO(Pamiec pamiec, ProcessJoiner proces)
: pamiec{pamiec}, proces{proces} {}

void FIFO::run(int n)
{
	int bledy = 0;
	std::vector<int> index_ramek;
	std::vector<int> licznik;
	std::vector<int> szamotanie;

	for (int i = 0; i < proces.getProcesy().size(); ++i)
		index_ramek.push_back(0);

	for (int i = 0; i < proces.getProcesy().size(); ++i)
		licznik.push_back(0);

	for (int i = 0; i < proces.getProcesy().size(); ++i)
		szamotanie.push_back(0);

	for (int i=0; i < n; ++i)
	{
		Strona nextPage = proces.getLastSeqPage();
		Proces nextProces = proces.getLastSeqProcess(nextPage);
		if (pamiec.isAlreadyInFrame(nextPage)) {
			licznik[nextProces.getId()] = 0;
			continue;
		} else {
			++bledy;
			licznik[nextProces.getId()] += 1;
			if (licznik[nextProces.getId()] >= PROG_SZAMOTANIA)
				szamotanie[nextProces.getId()] += 1;
			std::vector<int> index = pamiec.getIndeksyRamekProcesu(nextProces);
			int curr_index = index_ramek.at(nextProces.getId());
			curr_index = curr_index % index.size();
			curr_index = index.at(curr_index);
			pamiec.setStronaAt(curr_index, nextPage);
			index_ramek[nextProces.getId()] += 1;
		}
	}

	std::cout << "Błędy ramek: " << bledy << "\n";
	for (int i = 0; i < szamotanie.size(); ++i)
		std::cout << "Szamotanie: " << szamotanie[i] << " procesu " << i << "\n";
}

void FIFO::runDyn(int n)
{
	int bledy = 0;
	std::vector<int> v_bledy;
	std::vector<int> ppf;
	std::vector<int> index_ramek;
	std::vector<int> licznik;
	std::vector<int> szamotanie;
	int czas = 1;

	for (int i = 0; i < proces.getProcesy().size(); ++i)
		v_bledy.push_back(0);

	for (int i = 0; i < proces.getProcesy().size(); ++i)
		ppf.push_back(0);

	for (int i = 0; i < proces.getProcesy().size(); ++i)
		index_ramek.push_back(0);

	for (int i = 0; i < proces.getProcesy().size(); ++i)
		licznik.push_back(0);

	for (int i = 0; i < proces.getProcesy().size(); ++i)
		szamotanie.push_back(0);

	for (int i=0; i < n; ++i)
	{
		Strona nextPage = proces.getLastSeqPage();
		Proces nextProces = proces.getLastSeqProcess(nextPage);
		if (czas % OKNO_CZASOWE == 0)
		{
			for (Proces x : proces.getProcesy())
			{
				ppf[x.getId()] = v_bledy[x.getId()];
				v_bledy[x.getId()] = 0;	
			}

			for (Proces x : proces.getProcesy())
			{
				if (ppf[x.getId() > GORNY_PROG_PPF])
					pamiec.oddajRamke(x);
				if (ppf[x.getId() < DOLNY_PROG_PPF])
					pamiec.zabierzRamke(x);
			}
		}

		if (pamiec.isAlreadyInFrame(nextPage)) {
			licznik[nextProces.getId()] = 0;
			continue;
		} else {
			++bledy;
			v_bledy[nextProces.getId()] += 1;
			licznik[nextProces.getId()] += 1;
			if (licznik[nextProces.getId()] >= PROG_SZAMOTANIA)
				szamotanie[nextProces.getId()] += 1;
			std::vector<int> index = pamiec.getIndeksyRamekProcesu(nextProces);
			int curr_index = index_ramek.at(nextProces.getId());
			curr_index = curr_index % index.size();
			curr_index = index.at(curr_index);
			pamiec.setStronaAt(curr_index, nextPage);
			index_ramek[nextProces.getId()] += 1;
		}
		++czas;
	}

	std::cout << "Błędy ramek: " << bledy << "\n";
	for (int i = 0; i < szamotanie.size(); ++i)
		std::cout << "Szamotanie: " << szamotanie[i] << " procesu " << i << "\n";
}