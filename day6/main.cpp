#include <cstddef>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include "../util.hpp"

struct race
{
	unsigned long long time;
	unsigned long long record_distance;
};

unsigned solve(race& r)
{
	unsigned wins = 0;
	for(unsigned long long press_time = 0; press_time <= r.time; ++press_time)
	{
		unsigned long long race_time = r.time - press_time;
		unsigned long long velocity = press_time;
		if(race_time * velocity > r.record_distance)
			wins += 1;
	}

	return wins;
}

int main()
{
	std::vector<race> races;
	race final_race;
	std::string line;

	std::getline(std::cin, line);
	line = line.substr(std::string("Time: ").length() + 1);
	auto times = parse_ints(line);
	for(auto time : times)
		races.push_back({time, 0});

	line.erase(std::remove_if(line.begin(), line.end(), ::isspace), line.end());
	final_race.time = std::stoull(line);

	std::getline(std::cin, line);
	line = line.substr(std::string("Distance: ").length() + 1);
	auto dists = parse_ints(line);
	for(size_t i = 0; i < dists.size(); ++i)
		races[i].record_distance = dists[i];

	line.erase(std::remove_if(line.begin(), line.end(), ::isspace), line.end());
	final_race.record_distance = std::stoull(line);

	unsigned long long result1 = 1;
	for(auto r : races)
		result1 *= solve(r);

	std::cout << result1 << std::endl;

	std::cout << solve(final_race) << std::endl;


	return 0;
}
