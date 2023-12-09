#pragma once

#include <set>
#include <string>
#include <vector>

static inline std::set<unsigned> parse_play(std::string play)
{
	std::set<unsigned> numbers;
	std::size_t off;

	while(not play.empty())
	{
		numbers.insert(std::stoi(play, &off));
		play = play.substr(off == play.size() ? off : off + 1);
	}

	return numbers;
}

template<class T>
static inline std::vector<T> parse_ints(std::string play)
{
	std::vector<T> numbers;
	std::size_t off;

	while(not play.empty())
	{
		if constexpr (std::is_unsigned_v<T>)
			numbers.push_back(std::stoull(play, &off));
		else
			numbers.push_back(std::stoll(play, &off));
		
		play = play.substr(off == play.size() ? off : off + 1);
	}

	return numbers;
}

