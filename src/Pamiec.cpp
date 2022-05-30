#include "Pamiec.h"
#include <algorithm>
#include <iostream>

#define PRZEDZIAL_CZASOWY_CZESTOSC_BLEDOW 10

Pamiec::Pamiec(int ramki, std::vector<Proces> procesy)
	: procesy{procesy}
{
	this->ramki = std::vector<Ramka>();
	this->ramki.reserve(ramki);
	for (int i=0; i < ramki; ++i) {
		this->ramki.push_back(Ramka());
	}
	this->memo = std::vector<SecondChance>();
}

std::vector<Ramka> Pamiec::getRamki()
{
	return ramki;
}

bool Pamiec::isFull() const
{
	bool result = true;
	for_each(ramki.begin(), ramki.end(), [&](Ramka ramka)
	{
		if (ramka.getStrona().getNumerStrony() == -1)
		{
			result = false;
		}
	});
	return result;
}

void Pamiec::dodajStroneDoPustejRamki(ProcessJoiner& proces, int& bledy)
{
	for (int i=0; i < ramki.size(); ++i)
	{
		if (ramki.at(i).getStrona().getNumerStrony() == -1) {
			Strona nextPage = proces.getLastSeqPage();
			bool escape = false;
			for (int j=0; j < i; ++j) {
				if (ramki.at(j).getStrona().getNumerStrony() == nextPage.getNumerStrony()) {
					escape = true;
					break;
				}
			}
			if (escape) break;
			++bledy;
			ramki.at(i).setStrona(nextPage);
			break;		
		}
	}
}


void Pamiec::dodajStroneDoPustejRamkiMemoVer(ProcessJoiner& proces, int& bledy)
{
	for (int i=0; i < ramki.size(); ++i)
	{
		if (ramki.at(i).getStrona().getNumerStrony() == -1) {
			Strona nextPage = proces.getLastSeqPage();
			bool escape = false;
			for (int j=0; j < i; ++j) {
				if (ramki.at(j).getStrona().getNumerStrony() == nextPage.getNumerStrony()) {
					escape = true;
					break;
				}
			}
			if (escape) break;
			++bledy;
			updateMemo(nextPage);
			ramki.at(i).setStrona(nextPage);
			break;		
		}
	}
}

bool Pamiec::isAlreadyInFrame(Strona strona) const
{
	for (int i=0; i < ramki.size(); ++i)
	{
		if (ramki.at(i).getStrona().getNumerStrony() == strona.getNumerStrony()) {
			return true;
		}
	}
	return false;
}

void Pamiec::setStronaAt(int n, Strona& strona)
{
	ramki.at(n).setStrona(strona);
}

std::vector<SecondChance> Pamiec::getMemo()
{
	return memo;
}

int Pamiec::get0LifeMemoIndexInFrames()
{	
	unsigned desired;

	int init_size = memo.size();
	memo.reserve(init_size*2);

	auto it = memo.begin();
	while (true)
	{
		if ((*it).life == 0) {
			desired = (*it).numer_strony;
			++it;
			break;
		}
		SecondChance temp{(*it).numer_strony, 0};
		memo.push_back(temp);
		++it;
	}

	memo.erase(memo.begin(), it);

	memo.resize(init_size-1);

	int index = 0;
	for (int j=0; j < ramki.size(); ++j) {
		if (desired == ramki.at(j).getStrona().getNumerStrony()) {
			index = j;
			break;
		}
	}
	return index;
}

void Pamiec::updateMemo(Strona strona)
{
	unsigned numer_strony = strona.getNumerStrony();
	memo.push_back(SecondChance{numer_strony});
}

void Pamiec::przydzialRowny()
{
	int liczbaRamek = ramki.size() / procesy.size();
	int reszta  = ramki.size() - (liczbaRamek*procesy.size());
	int index = 0;
	for_each(procesy.begin(), procesy.end(), [&](Proces& proces)
	{
		for (int i = 0; i < liczbaRamek; ++i)
		{
			ramki.at(index).setWlasciciel(proces.getId());
			++index;
		}
	});
	// for (int i = 0; i < reszta; ++i)
	// {
	// 	ramki.at(index).setWlasciciel(procesy.at(procesy.size()-1).getId());
	// 	++index;
	// }
}

void Pamiec::przydzialProporcjonalny()
{
	int index = 0;
	int calkowita_liczba_strona = 0;
	int liczba_zajetych_ramek = 0;
	for_each(procesy.begin(), procesy.end(), [&](Proces& proces)
	{
		calkowita_liczba_strona += proces.getStrony().size();
	});
	for_each(procesy.begin(), procesy.end(), [&](Proces& proces)
	{
		double wspolczynnik = (double)proces.getStrony().size() / calkowita_liczba_strona;
		int liczbaRamek = wspolczynnik * ramki.size();
		if (liczbaRamek < 1)
			liczbaRamek = 1;
		liczba_zajetych_ramek += liczbaRamek;
		for (int i = 0; i < liczbaRamek; ++i)
		{
			ramki.at(index).setWlasciciel(proces.getId());
			++index;
		}
	});
	// int pozostale_ramki = ramki.size() - liczba_zajetych_ramek;
	// for (int i = 0; i < pozostale_ramki; ++i)
	// {
	// 	ramki.at(index).setWlasciciel(procesy.at(procesy.size()-1).getId());
	// 	++index;
	// }
}

void Pamiec::przydzialWSS(std::vector<std::vector<unsigned>> WSS, int D, std::vector<Proces> procesy)
{
	int index = 0;
	if (D <= ramki.size())
	{
		for (Proces x : procesy)
		{
			for (int i = 0; i < WSS[x.getId()].size(); ++i)
			{
				ramki.at(index).setWlasciciel(x.getId());
				++index;
			}
		}
	}
	else
	{
		int max = 0;
		int index_max = 0;
		for (int i = 0; i < WSS.size(); ++i)
		{
			if (WSS.at(i).size() > max)
			{
				max = WSS.at(i).size();
				index_max = i;
			}
		}
		WSS.erase(WSS.begin()+index_max);
		auto match = std::find_if(procesy.begin(), procesy.end(), [&](Proces& proces)
		{
			return proces.getId() == index_max;
		});
		procesy.erase(match, procesy.end());

		int index_local = 0;
		int calkowita_liczba_strona = 0;
		int liczba_zajetych_ramek = 0;
		for_each(procesy.begin(), procesy.end(), [&](Proces& proces)
		{
			calkowita_liczba_strona += proces.getStrony().size();
		});
		for_each(procesy.begin(), procesy.end(), [&](Proces& proces)
		{
			double wspolczynnik = (double)proces.getStrony().size() / calkowita_liczba_strona;
			int liczbaRamek = wspolczynnik * ramki.size();
			if (liczbaRamek < 1)
				liczbaRamek = 1;
			liczba_zajetych_ramek += liczbaRamek;
			for (int i = 0; i < liczbaRamek; ++i)
			{
				ramki.at(index).setWlasciciel(proces.getId());
				++index_local;
			}
		});
	}
}

void Pamiec::zabierzRamke(Proces& proces)
{
	if (getIndeksyRamekProcesu(proces).size() <= 1)
		return;
	for (int i = 0; i < ramki.size(); ++i)
	{
		if (ramki.at(i).getWlasciciel() == proces.getId())
		{
			ramki.at(i).setWlasciciel(-1);
			return;
		}
	}
}

void Pamiec::oddajRamke(Proces& proces)
{
	for (int i = 0; i < ramki.size(); ++i)
	{
		if (ramki.at(i).getWlasciciel() == -1)
		{
			ramki.at(i).setWlasciciel(proces.getId());
			return;
		}
	}
	for (int i = 0; i < ramki.size(); ++i)
	{
		if (ramki.at(i).getWlasciciel() == proces.getId())
			ramki.at(i).setWlasciciel(-1);
	}
	proces.setHalted(true);
}

std::vector<int> Pamiec::getIndeksyRamekProcesu(Proces proces)
{
	std::vector<int> temp;
	for (int i = 0; i < ramki.size(); ++i)
	{
		if (ramki.at(i).getWlasciciel() == proces.getId())
			temp.push_back(i);
	}
	return temp;
}