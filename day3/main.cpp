#include <cstddef>
#include <iostream>
#include <iterator>
#include <numeric>
#include <set>
#include <vector>

struct number
{
	unsigned n;
	unsigned start_pos, end_pos;
	unsigned y;

	bool within(unsigned pos) const
	{
		return pos >= start_pos and pos <= end_pos;
	}

	bool operator<(const number& b) const {
		if(y != b.y) return y < b.y;
		return start_pos < b.start_pos;
	}

	bool operator==(const number& b) const{ return y == b.y and start_pos == b.start_pos;}
};

struct symbol
{
	char s;
	unsigned pos;
};

struct line
{
	std::vector<number> numbers;
	std::vector<symbol> symbols;
};

static unsigned long long sum_dbg = 0;
static line parse_line(const std::string& l_str, unsigned y)
{
	line l;

	for(auto it = l_str.begin(); it != l_str.end(); ++it)
	{
		if(*it == '.')
			continue;

		if(std::isdigit(*it))
		{
			number n;
			size_t pos = 0;

			// parse number
			n.start_pos = std::distance(l_str.begin(), it);
			n.n = std::stoi(l_str.substr(n.start_pos), &pos);
			n.end_pos = n.start_pos + pos - 1;
			n.y = y;

			sum_dbg += n.n;

			// push back
			l.numbers.push_back(std::move(n));

			// Move iterator foward
			it += pos - 1;
			continue;
		}

		// is symbol
		l.symbols.push_back({
			.s = *it, 
			.pos = (unsigned)std::distance(l_str.begin(), it)
		});
	}

	return l;
}

static std::set<number> numbers_nearby_x(const line& l, unsigned x_centre)
{
	std::set<number> res;
	for(const auto x : {x_centre - 1, x_centre, x_centre + 1})
	{
		for(const auto& number : l.numbers)
			if(x > 0 and number.within(x))
				res.insert(number);
	}

	return res;
}

int main()
{
	std::vector<line> lines;
	std::string tmp;
	size_t y = 0;

	while(std::getline(std::cin, tmp))
		lines.push_back(parse_line(tmp, y++));

	std::cout << sum_dbg << std::endl;

	std::set<number> addenda;
	unsigned long long total_ratio = 0;

	for(size_t y = 0; y < lines.size(); ++y)
	{
		for(const auto& sym : lines[y].symbols)
		{
			std::set<number> adj;

			if(y != 0)
				adj.merge(numbers_nearby_x(lines[y - 1], sym.pos));

			adj.merge(numbers_nearby_x(lines[y], sym.pos));

			if(y != lines.size() - 1)
				adj.merge(numbers_nearby_x(lines[y + 1], sym.pos));

			if(sym.s == '*' and adj.size() == 2)
			{
				std::cout << "hit\n";
				total_ratio += adj.begin()->n * std::next(adj.begin(), 1)->n;
			}

			addenda.merge(adj);
		}
	}

	unsigned sum = 0;
	for(const auto& addendum : addenda)
	{
		//std::cout << addendum.n << '\t';
		sum += addendum.n;
	}
	//std::cout << std::endl;

	std::cout << sum << '\t' << total_ratio << std::endl;

	return 0;
}
