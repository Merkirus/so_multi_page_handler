#include "Ramka.h"
#include "BlankStrona.h"

Ramka::Ramka()
{
	obecna_strona = BlankStrona{};
	this->wlasciciel = -1;
}

Strona Ramka::getStrona() const
{
	return obecna_strona;
}

void Ramka::setStrona(Strona strona)
{
	this->obecna_strona = strona;
}

void Ramka::setWlasciciel(int proces)
{
	this->wlasciciel = proces;
}

int Ramka::getWlasciciel() const
{
	return wlasciciel;
}