#ifndef STRONA_H
#define STRONA_H

#include <vector>

class Strona
{
public:
	Strona();
	~Strona()=default;
	int getNumerStrony() const;
	void setNumerStrony();
	void mightDone();
	bool isDone() const;
protected:
	int numer_strony;
	bool wykonana;
	static unsigned count;
};

#endif /*STRONA_H*/