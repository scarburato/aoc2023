#include <cstddef>
#include <iostream>
#include <numeric>
#include <set>
#include <string>
#include <vector>
#include "../util.hpp"

int main()
{
	std::string line;
	unsigned long score = 0;
	std::vector<unsigned> instances;
	size_t curr_card = 0;

	while(std::getline(std::cin, line))
	{
		size_t pos_col = line.find(':');
		size_t pos_bar = line.find('|');

		std::string sol = line.substr(pos_col + 2, pos_bar - pos_col - 2);
		std::string play = line.substr(pos_bar + 2);

		auto sol_set = parse_play(sol);
		auto play_set = parse_play(play);

		unsigned long card_score = 0;
		unsigned long wins = 0;

		for(const auto n : play_set)
			if(sol_set.contains(n))
			{
				wins += 1;
				card_score = card_score == 0 ? 1 : card_score*2;
			}
		
		if(curr_card + wins + 2 > instances.size())
			instances.resize(curr_card + wins + 2);

		instances[curr_card] += 1;
		std::cout << (curr_card + 1) << '\t' << instances[curr_card] << '\t' << wins << '\n';

		for(size_t i = curr_card + 1; i <= curr_card + wins; ++i)
			instances[i] += instances[curr_card];
		
		score += card_score;
		++curr_card;
	}

	for(auto t : instances)
		std::cout << t << '\t';
	std::cout << std::endl;
	auto total_cards = std::accumulate(instances.begin(), instances.begin() + curr_card, 0u);

	std::cout << score << '\t' << total_cards << '\n';
}
