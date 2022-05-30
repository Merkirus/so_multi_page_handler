#include "ProcessJoiner.h"
#include <iostream>

ProcessJoiner::ProcessJoiner(std::vector<Proces>& v)
	: procesy{v}, sequence{std::vector<unsigned>()},
	past_sequence{std::vector<unsigned>()}
{
	generateSeq();
}

Strona ProcessJoiner::getLastSeqPage()
{
	unsigned result = sequence.at(0);
	Strona strona;
	for (int i = 0; i < procesy.size(); ++i)
	{
		if (procesy.at(i).ownPage(result))
		{
			strona = procesy.at(i).getLastSeqPage();
			break;
		}
	}
	past_sequence.push_back(strona.getNumerStrony());
	sequence.erase(sequence.begin());
	if (sequence.size() <= 1)
		generateSeq();
	return strona;
}

Proces ProcessJoiner::getLastSeqProcess(Strona strona)
{
	Proces result;
	for_each(procesy.begin(), procesy.end(), [&](Proces& proces)
	{
		if (proces.ownPage(strona.getNumerStrony()))
			result = proces;
	});
	return result;
}

std::vector<unsigned> ProcessJoiner::getSeq()
{
	return sequence;
}

std::vector<unsigned> ProcessJoiner::getPastSeq()
{
	return past_sequence;
}

void  ProcessJoiner::generateSeq()
{
	sequence = std::vector<unsigned>();
	for_each(procesy.begin(), procesy.end(), [&](Proces& proces)
	{
		sequence.reserve(sequence.size() + proces.getSeq().size());
	});
	std::vector<std::vector<unsigned>> temp_seq{};
	for_each(procesy.begin(), procesy.end(), [&](Proces& proces)
	{
		temp_seq.push_back(proces.getSeq());
	});
	while (!temp_seq.empty())
	{
		int index = randnum(0, temp_seq.size()-1);
		sequence.push_back(temp_seq.at(index).at(0));
		temp_seq.at(index).erase(temp_seq.at(index).begin());
		auto match = find_if(temp_seq.begin(), temp_seq.end(), [&](std::vector<unsigned> seq)
		{
			return seq.size() == 0;
		});
		temp_seq.erase(match, temp_seq.end());
	}
	// for_each(procesy.begin(), procesy.end(), [&](Proces& proces)
	// {
	// 	sequence.insert(sequence.end(), proces.getSeq().begin(), proces.getSeq().end());
	// });
	// auto rng = std::default_random_engine{};
	// std::shuffle(std::begin(sequence), std::end(sequence), rng);
}

std::vector<Proces> ProcessJoiner::getProcesy()
{
	return procesy;
}