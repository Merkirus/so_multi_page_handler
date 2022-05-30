#include "FIFO.h"
#include <iostream>
#include <algorithm>

#define PROG_SZAMOTANIA 10
#define OKNO_SZAMOTANIA 20
#define OKNO_CZASOWE 10
#define DOLNY_PROG_PPF 2
#define GORNY_PROG_PPF 4
#define WIELKOSC_ZBIORU_ROBOCZEGO 10
#define OKNO_ZBIORU_ROBOCZEGO 5

FIFO::FIFO(Pamiec pamiec, ProcessJoiner proces)
: pamiec{pamiec}, proces{proces} {}

void FIFO::run(int n)
{

	/*WYBIERAM PRZYDZIAL*/
	pamiec.przydzialProporcjonalny();

	int bledy = 0;
	std::vector<int> index_ramek;
	std::vector<int> licznik;
	std::vector<int> szamotanie;
	int czas = 1;

	for (int i = 0; i < proces.getProcesy().size(); ++i)
		index_ramek.push_back(0);

	for (int i = 0; i < proces.getProcesy().size(); ++i)
		licznik.push_back(0);

	for (int i = 0; i < proces.getProcesy().size(); ++i)
		szamotanie.push_back(0);

	for (int i=0; i < n; ++i)
	{
		if (czas % OKNO_SZAMOTANIA == 0)
		{
			for (int i = 0; i < licznik.size(); ++i)
			{
				if (licznik[i] >= PROG_SZAMOTANIA)
					szamotanie[i] += 1;
			}
			for (int i = 0; i < licznik.size(); ++i)
			{
				licznik[i] = 0;
			}
		}

		Strona nextPage = proces.getLastSeqPage();
		std::cout << nextPage.getNumerStrony() << '\n';
		Proces nextProces = proces.getLastSeqProcess(nextPage);
		if (pamiec.isAlreadyInFrame(nextPage)) {
			++czas;
			continue;
		} else {
			++bledy;
			licznik[nextProces.getId()] += 1;
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

void FIFO::runDyn(int n)
{

	/*WYBIERAM PRZYDZIAL*/
	pamiec.przydzialProporcjonalny();

	int bledy = 0;
	std::vector<int> v_bledy;
	std::vector<int> ppf;
	std::vector<int> index_ramek;
	std::vector<int> licznik;
	std::vector<int> szamotanie;
	std::vector<bool> przerwane;
	int czas = 1;

	for (int i = 0; i < proces.getProcesy().size(); ++i)
		przerwane.push_back(false);

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
		if (czas % OKNO_SZAMOTANIA == 0)
		{
			for (int i = 0; i < licznik.size(); ++i)
			{
				if (licznik[i] >= PROG_SZAMOTANIA)
					szamotanie[i] += 1;
			}
			for (int i = 0; i < licznik.size(); ++i)
			{
				licznik[i] = 0;
			}
		}
		if (czas % OKNO_CZASOWE == 0)
		{
			for (Proces x : proces.getProcesy())
			{
				ppf[x.getId()] = v_bledy[x.getId()];
				v_bledy[x.getId()] = 0;	
			}

			for (Proces x : proces.getProcesy())
			{
				if (ppf[x.getId()] > GORNY_PROG_PPF)
				{
					std::cout << "HERE" << '\n';
					pamiec.oddajRamke(x);
				}
				if (ppf[x.getId()] < DOLNY_PROG_PPF)
					pamiec.zabierzRamke(x);
			}
		}

		Strona nextPage = proces.getLastSeqPage();
		Proces nextProces = proces.getLastSeqProcess(nextPage);

		std::vector<int> index = pamiec.getIndeksyRamekProcesu(nextProces);

		if (index.size() == 0)
		{
			przerwane[nextProces.getId()] = true;
			++czas;
			continue;
		}

		if (pamiec.isAlreadyInFrame(nextPage)) {
			++czas;
			continue;
		} else {
			++bledy;
			v_bledy[nextProces.getId()] += 1;
			licznik[nextProces.getId()] += 1;
			/*Do pokazania, że trudno uzyskać szamotanie
			dla nawet jedej ramki na proces z moją lokalnością*/
			// std::cout << nextPage.getNumerStrony() << '\n';
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
	for (int i = 0; i < przerwane.size(); ++i)
		std::cout << "Proces: " << i << " przerwany?: " << przerwane[i] << '\n';
}

void FIFO::runWSS(int n)
{
	int bledy = 0;
	std::vector<int> index_ramek;
	std::vector<int> licznik;
	std::vector<int> szamotanie;
	std::vector<bool> przerwane;
	std::vector<std::vector<unsigned>> WSS;
	int D = 0;
	int czas = 1;

	WSS = makeWSS();

	for (std::vector<unsigned> v : WSS)
	{
		D += v.size();
	}

	pamiec.przydzialWSS(WSS, D, proces.getProcesy());

	for (int i = 0; i < proces.getProcesy().size(); ++i)
		przerwane.push_back(false);

	for (int i = 0; i < proces.getProcesy().size(); ++i)
		index_ramek.push_back(0);

	for (int i = 0; i < proces.getProcesy().size(); ++i)
		licznik.push_back(0);

	for (int i = 0; i < proces.getProcesy().size(); ++i)
		szamotanie.push_back(0);

	for (int i=0; i < n; ++i)
	{
		if (czas % OKNO_SZAMOTANIA == 0)
		{
			for (int i = 0; i < licznik.size(); ++i)
			{
				if (licznik[i] >= PROG_SZAMOTANIA)
					szamotanie[i] += 1;
			}
			for (int i = 0; i < licznik.size(); ++i)
			{
				licznik[i] = 0;
			}
		}
		if (czas % OKNO_ZBIORU_ROBOCZEGO == 0)
		{
			WSS = makeWSS();
			D = 0;
			for (std::vector<unsigned> v : WSS)
			{
				D += v.size();
			}
			pamiec.przydzialWSS(WSS, D, proces.getProcesy());
		}
		
		Strona nextPage = proces.getLastSeqPage();
		Proces nextProces = proces.getLastSeqProcess(nextPage);

		std::vector<int> index = pamiec.getIndeksyRamekProcesu(nextProces);

		/*Proces zhaltowany*/
		if (index.size() == 0)
		{
			przerwane[nextProces.getId()] = true;
			++czas;
			continue;
		}

		if (pamiec.isAlreadyInFrame(nextPage)) {
			licznik[nextProces.getId()] = 0;
			++czas;
			continue;
		} else {
			++bledy;
			licznik[nextProces.getId()] += 1;
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
	for (int i = 0; i < przerwane.size(); ++i)
		std::cout << "Proces: " << i << " przerwany?: " << przerwane[i] << '\n';
}

std::vector<std::vector<unsigned>> FIFO::makeWSS()
{
	std::vector<std::vector<unsigned>> WSS{};

	for (int i = 0; i < proces.getProcesy().size(); ++i)
	WSS.push_back(std::vector<unsigned>{});

	std::vector<unsigned> seq_processes = proces.getSeq();

	for (auto it = seq_processes.begin(); it != seq_processes.end(); ++it)
	{
		std::vector<Proces> procesy = proces.getProcesy();
		for (auto it2 = procesy.begin(); it2 != procesy.end(); ++it2)
		{
			if ((*it2).ownPage(*it))
			{
				if (WSS[(*it2).getId()].size() < WIELKOSC_ZBIORU_ROBOCZEGO)
				{
					WSS[(*it2).getId()].push_back(*it);
				}
				break;
			}
		}

		int isDone = 0;
		for_each(WSS.begin(), WSS.end(), [&](std::vector<unsigned> v)
		{
			if (v.size() == 5)
				++isDone;
		});
		if (isDone == WSS.size())
			break;
	}

	for (auto it = WSS.begin(); it != WSS.end(); ++it)
	{
		auto end = (*it).end();
		for (auto it2 = (*it).begin(); it2 != end; ++it2)
			end = std::remove(it2+1,end,*it2);
		(*it).erase(end, (*it).end());
	}
	return WSS;
}