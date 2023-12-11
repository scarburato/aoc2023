#include <cstddef>
#include <cstdint>
#include <iostream>
#include <algorithm>
#include <iterator>
#include <ostream>
#include <string>
#include <vector>
#include "../coordinate.hpp"

struct ObservedUniverse
{
	static constexpr uint16_t VOID = 0;
	uint16_t galaxies_cardinality = 0;
	std::vector<std::vector<uint16_t>> space;
	std::map<uint16_t, coordinate> stars;

	void add_row_of_space(const std::string& line)
	{
		std::vector<uint16_t> space_row;
		for(const auto c : line)
		{
			space_row.push_back(c == '#' ? ++galaxies_cardinality : VOID);
		}

		space.push_back(std::move(space_row));
	}

	uint16_t at(size_t x, size_t y) const {return space[y][x];}
	size_t width() const {return space.empty() ? 0 : space[0].size();}
	size_t heigth() const {return space.size();}

	void update_stars_pos()
	{
		stars.clear();

		for(size_t x = 0; x < width(); ++x)
		{
			for(size_t y = 0; y < heigth(); ++y)
			{
				auto wat = at(x, y);
				if(wat != VOID)
					stars[wat] = coordinate(x, y);
			}
		}
	}

	void print(std::ostream& tty)
	{
		for(auto& row : space)
		{
			for(auto cell : row)
				tty << (cell == VOID ? '.' : '#');
			tty << '\n';
		}
	}

	auto distance(uint16_t s1, uint16_t s2) const {return stars.at(s1).norm_manhattan(stars.at(s2));}

	void expand(unsigned expansion = 1)
	{
		std::vector<size_t> empty_rows;
		for(size_t y = 0; y < heigth(); ++y)
			if(std::all_of(space[y].begin(), space[y].end(), [](auto n) {return n == VOID;}))
				empty_rows.push_back(y);
		
		std::vector<size_t> empty_cols;
		for(size_t x = 0; x < width(); ++x)
		{
			bool empty = true;
			for(size_t y = 0; y < heigth() and empty; ++y)
				if(at(x, y) != VOID)
					empty = false;
			
			if(empty)
				empty_cols.push_back(x);
		}

		// Expand rows
		size_t curr_off_r = 0;
		std::vector<uint16_t> empty;
		for(size_t x = 0; x < width(); ++x)
			empty.push_back(VOID);

		for(const auto row_index : empty_rows)
		{
			size_t i = row_index + curr_off_r;
			for(size_t j = 0; j < expansion; ++j)
				space.insert(space.begin() + i, empty);

			curr_off_r += expansion;
		}

		// Expand cols
		size_t curr_off_c = 0;
		for(const auto col_index : empty_cols)
		{
			size_t i = col_index + curr_off_c;
			for(size_t j = 0; j < expansion; ++j)
			{
				for(auto& row : space)
					row.insert(row.begin() + i, VOID);
			}

			curr_off_c += expansion;
		}

		update_stars_pos();
	}
};

int main()
{
	ObservedUniverse uni;
	std::string line;

	while(std::getline(std::cin, line))
	{
		uni.add_row_of_space(line);
	}

	uni.expand(1'000'000);

	size_t sum = 0;
	for(auto it_a = uni.stars.begin(); std::next(it_a) != uni.stars.end(); ++it_a)
	{
		for(auto it_b = std::next(it_a); it_b != uni.stars.end(); ++it_b)
			sum += uni.distance(it_a->first, it_b->first);
			//std::cout << it_a->first << ' ' << it_a->second << '\t' << it_b->first << ' ' << it_b->second <<'\t' << uni.distance(it_a->first, it_b->first) << '\n';
	}

	std::cout << sum << '\n';

	return 0;
}
