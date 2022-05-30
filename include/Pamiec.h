#ifndef PAMIEC_H
#define PAMIEC_H

#include <vector>
#include "Ramka.h"
#include "Proces.h"
#include "ProcessJoiner.h"

struct SecondChance
{
	unsigned numer_strony;
	unsigned life = 1;
};

class Pamiec
{
public:
	Pamiec(int ramki, std::vector<Proces> procesy);
	~Pamiec() = default;
	std::vector<Ramka> getRamki();
	bool isAlreadyInFrame(Strona strona) const;
	bool isFull() const;
	void dodajStroneDoPustejRamki(ProcessJoiner& proces, int& bledy);
	void dodajStroneDoPustejRamkiMemoVer(ProcessJoiner& proces, int& bledy);
	void setStronaAt(int n, Strona& strona);
	/*Do algorytmu drugiej szansy*/
	std::vector<SecondChance> getMemo();
	int get0LifeMemoIndexInFrames();
	void updateMemo(Strona);
	/*Przydzialy ramek*/
	void przydzialRowny();
	void przydzialProporcjonalny();
	void przydzialWSS(std::vector<std::vector<unsigned>> WSS, int D, std::vector<Proces> procesy);
	void zabierzRamke(Proces& proces);
	void oddajRamke(Proces& proces);
	std::vector<int> getIndeksyRamekProcesu(Proces proces);
private:
	std::vector<Ramka> ramki;
	std::vector<Proces> procesy;
	std::vector<SecondChance> memo;
};

#endif /*PAMIEC_H*/