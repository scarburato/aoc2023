#include <algorithm>
#include <cstddef>
#include <iostream>
#include <string>
#include <vector>
#include "../util.hpp"

static std::vector<long> complete_seq(std::vector<long> seq)
{
	if(seq.empty())
		abort();

	// Exit condition: all zero
	if(std::all_of(seq.begin(), seq.end(), [](long n) {return n == 0;}))
	{
		seq.push_back(0);
		seq.insert(seq.begin(), 0);
		return seq;
	}

	std::vector<long> diff;
	diff.reserve(seq.size() + 1);
	for(size_t i = 1; i < seq.size(); ++i)
		diff.push_back(seq[i] - seq[i - 1]);

	auto diff2 = complete_seq(diff);

	diff.push_back(seq.back() + diff2.back());
	diff.insert(diff.begin(), seq.front() - diff2.front());

	return diff;
}

int main()
{
	std::string line;
	long long sum = 0;
	long long sum2 = 0;

	while(std::getline(std::cin, line))
	{
		std::vector<long> seq = parse_ints<long>(line);
		auto r = complete_seq(seq);

		sum += r.back();
		sum2 += r.front();
	}

	std::cout << sum << '\t' << sum2 << std::endl;
}
