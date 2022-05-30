#ifndef RAMKA_H
#define RAMKA_H

#include <vector>
#include "Strona.h"
#include "Proces.h"

class Ramka
{
public:
	Ramka();
	~Ramka() = default;
	Strona getStrona() const;
	void setStrona(Strona);
	void setWlasciciel(int proces);
	int getWlasciciel() const;
private:
	Strona obecna_strona;
	int wlasciciel;
};

#endif /*RAMKA_H*/