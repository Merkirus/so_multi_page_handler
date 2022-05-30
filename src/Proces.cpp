#include "Proces.h"
#include "Random.h"

#define LOWER_THRESHOLD 10
#define DEFAULT_NUM_OF_PAGES 10

Proces::Proces()
	: sequence{std::vector<unsigned>()}, strony{std::vector<Strona>()},
	past_sequence{std::vector<unsigned>()}
{
	this->id = count;
	++count;
}

void Proces::generatePages(int n)
{
	int i = 0;
	while (i < n) {
		strony.push_back(Strona{});
		++i;
	}
}

void Proces::generatePages()
{
	generatePages(5);
}

void Proces::generateSeq(int n)
{
	if (sequence.size() > LOWER_THRESHOLD) {
		return;
	}

	int i = 0;

	while (i < n) {
		/* odwolanie lokalne */
		if (randnum(0,10) == 1) {
			std::vector<unsigned> strony_lokalne_odwolanie;

			for (int j=0; j < 3; ++j) {
				int random_num = randnum(0, strony.size()-1);
				unsigned strona = strony.at(random_num).getNumerStrony();
				strony_lokalne_odwolanie.push_back(strona);
			}

			for(int zakres = randnum(4,8); zakres >= 0; --zakres) {
				int seq = strony_lokalne_odwolanie.at(randnum(0, strony_lokalne_odwolanie.size()-1));
				sequence.push_back(seq);
			}
		} else {
			Strona temp = strony.at(randnum(0,strony.size()-1));
			sequence.push_back(temp.getNumerStrony());
		}
		++i;
	}
}

void Proces::generateSeq()
{
	generateSeq(5);
}

void Proces::delLastSeqElem()
{
	past_sequence.push_back(sequence.at(0));
	sequence.erase(sequence.begin());
	generateSeq();
}

bool Proces::ownPage(unsigned strona)
{
	auto match = find_if(strony.begin(), strony.end(),
		[&](Strona& var) -> bool
		{
			if (var.getNumerStrony() == strona)
				return true;
			return false;
		});
	if (match != strony.end())
		return true;
	return false;
}

unsigned Proces::getLastSeqElem()
{
	unsigned result = sequence.at(0);
	delLastSeqElem();
	return result;
}

Strona Proces::getLastSeqPage()
{
	unsigned desired_page = getLastSeqElem();
	for (int i=0; i < strony.size(); ++i)
	{
		if (strony.at(i).getNumerStrony() == desired_page)
		{
			return strony.at(i);
		}
	}
	return Strona{};
}

std::vector<unsigned> Proces::getSeq()
{
	return sequence;
}

std::vector<unsigned> Proces::getPastSeq()
{
	return past_sequence;
}

void Proces::giveSeq(std::vector<unsigned> v)
{
	sequence = v;
}

std::vector<Strona> Proces::getStrony()
{
	return strony;
}

int Proces::getId() const
{
	return id;
}

unsigned Proces::count = 0;