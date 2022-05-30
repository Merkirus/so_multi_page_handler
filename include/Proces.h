#ifndef PROCES_H
#define PROCES_H

#include <vector>
#include "Strona.h"

class Proces
{
public:
	Proces();
	~Proces()=default;
	void generatePages(int n);
	void generatePages();
	void generateSeq(int n);
	void generateSeq();
	void giveSeq(std::vector<unsigned> v);
	unsigned getLastSeqElem();
	Strona getLastSeqPage();
	void delLastSeqElem();
	bool ownPage(unsigned strona);
	std::vector<unsigned> getSeq();
	std::vector<unsigned> getPastSeq();
	std::vector<Strona> getStrony();
	int getId() const;
private:
	int id;
	std::vector<unsigned> sequence;
	std::vector<Strona> strony;
	std::vector<unsigned> past_sequence;
	static unsigned count;
};

#endif /*PROCES_H*/