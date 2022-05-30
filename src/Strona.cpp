#include "Strona.h"
#include "Random.h"

Strona::Strona()
{
	this->numer_strony = count;
	this->wykonana = false;
	++count;
}

void Strona::mightDone()
{
	if (randnum(0,10) == 1)
		this->wykonana = true;
}

bool Strona::isDone() const
{
	return this->wykonana;
}

int Strona::getNumerStrony() const
{
	return numer_strony;
}

void Strona::setNumerStrony()
{

}

unsigned Strona::count = 1;