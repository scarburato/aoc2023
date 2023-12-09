#include <algorithm>
#include <cstddef>
#include <iostream>
#include <numeric>
#include <string>
#include <unordered_map>
#include <vector>

struct node {std::string left, right;};

int main()
{
	// Load steps
	std::string steps_str, tmp;
	std::getline(std::cin, steps_str);
	std::getline(std::cin, tmp); // empty line
	std::vector<char> steps{steps_str.begin(), steps_str.end()};

	// Map
	std::unordered_map<std::string, node> nav_map;
	std::vector<std::string> start_positions;
	bool zzz_exists = false;

	// load map
	while(std::getline(std::cin, tmp))
	{
		const auto n_str = tmp.substr(0, 3);

		nav_map[n_str] = {
			.left = tmp.substr(7, 3),
			.right = tmp.substr(12, 3)
		};

		if(n_str[2] == 'A')
			start_positions.push_back(n_str);

		if(n_str == "ZZZ")
			zzz_exists = true;
	}

	// navigate map
	size_t n_steps = 0;
	std::string curr_node = "AAA";

	if(zzz_exists)
	{
		while(curr_node != "ZZZ")
		{
			if(steps[n_steps++ % steps.size()] == 'R')
				curr_node = nav_map[curr_node].right;
			else
				curr_node = nav_map[curr_node].left;
		}

		std::cout << n_steps << std::endl;
	}

	std::vector<size_t> periods;

	for(const auto& sn : start_positions)
	{
		n_steps = 0;
		std::string curr_node = sn;

		while(not (n_steps % steps.size() == 0 and curr_node[2] == 'Z'))
		{
			if(steps[n_steps++ % steps.size()] == 'R')
				curr_node = nav_map[curr_node].right;
			else
				curr_node = nav_map[curr_node].left;
		}

		periods.push_back(n_steps);
	}

	size_t lcm = 1;
	for(auto period : periods)
		lcm = std::lcm(lcm, period);

	std::cout << *std::max_element(periods.begin(), periods.end()) << '\t' << lcm << std::endl;

	return 0;
}
