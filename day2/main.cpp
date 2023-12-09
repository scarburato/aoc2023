#include <cstddef>
#include <iostream>
#include <regex>
#include <string>

static const std::map<std::string, size_t> maximum_cubes = {
	{"red", 12}, {"green", 13}, {"blue", 14}	
};

int main()
{
	std::string line;
	std::regex game_id_regex("Game (\\d+)");
	std::regex cubes_regex("(\\d+) (\\w+)");
	size_t score = 0;
	size_t score2 = 0;

	while(std::getline(std::cin, line))
	{
		std::smatch sm;
		regex_search(line, sm, game_id_regex);
		const unsigned long id = std::stoull(sm[1]);
		std::map<std::string, size_t> minum_cubes;
		bool valid = true;
		size_t power_score = 1;

		for (std::smatch sm; regex_search(line, sm, cubes_regex); line = sm.suffix())
		{
			const size_t cubes_n =  std::stoull(sm[1]);
			if(maximum_cubes.at(sm[2]) < cubes_n)
				valid = false;

			minum_cubes[sm[2]] = std::max(minum_cubes[sm[2]], cubes_n);
		}

		for(const auto& [color, n] : minum_cubes)
			power_score *= n;

		score2 += power_score;

		if (valid)
			score += id;
	}

	std::cout << score << '\t' << score2 << std::endl;

	return 0;
}
