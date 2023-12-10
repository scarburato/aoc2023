#include <cstddef>
#include <cstdint>
#include <iostream>
#include <iterator>
#include <map>
#include <memory>
#include <string>
#include <sys/types.h>
#include <vector>
#include <bit>

enum directions {
	NONE = 0,
	UP =	0b0001,
	DOWN =	0b0010,
	RIGHT =	0b0100,
	LEFT =	0b1000,
	START = 0b1111,
};

static const std::map<directions, directions> flip_dir{
	{UP, DOWN}, {DOWN, UP}, {RIGHT, LEFT}, {LEFT, RIGHT}
};

struct coordinate {
	long x;
	long y;

	coordinate(long x, long y): x(x), y(y) {}
	coordinate(const directions& d)
	{
		if(d & directions::UP) {x = 0; y = -1; return;}
		if(d & directions::DOWN) {x = 0; y = +1; return;}
		if(d & directions::RIGHT) {x = 1; y = 0; return;}
		if(d & directions::LEFT) {x = -1; y = 0; return;}
		abort();
	}

	coordinate operator+(const coordinate& b) const {return {x + b.x, y + b.y};}
	bool operator==(const coordinate& b) const {return x == b.x and y == b.y;}
	bool operator!=(const coordinate& b) const {return x != b.x or y != b.y;}
};

struct polygon
{
	std::vector<std::vector<char>> shape;
	size_t h, w;

	polygon(size_t heigth, size_t width): h(heigth), w(width)
	{
		shape.resize(heigth);
		for(size_t i = 0; i < heigth; ++i)
			for(size_t j = 0; j < width; ++j)
				shape[i].push_back('.');
	}

	void flood()
	{
		for(auto& row : shape)
		{
			for(size_t c = 0; c < w and row[c] != '*'; ++c)
				row[c] = '#';

			for(long c = w - 1; c >= 0 and row[c] != '*'; --c)
				row[c] = '#';
		}

		for(size_t c = 0; c < w; ++c)
		{
			for(size_t y = 0; y < h and shape[y][c] != '*'; ++y)
				shape[y][c] = '#';

			for(long y = h - 1 ; y >= 0 and shape[y][c] != '*'; --y)
				shape[y][c] = '#';
		}

		bool all_stop = false;
		while(not all_stop)
		{
			all_stop = true;

			for(size_t y = 0; y < h; ++y)
				for(size_t x = 0; x < w; ++x)
				{
					if(shape[y][x] != '#')
						continue;

					if(x > 0 and shape[y][x-1] == '.')
					{
						all_stop = false;
						shape[y][x-1] = '#';
					}

					if(y > 0 and shape[y - 1][x] == '.')
					{
						all_stop = false;
						shape[y-1][x] = '#';
					}

					if(x > 0 and y > 0 and shape[y - 1][x-1] == '.')
					{
						all_stop = false;
						shape[y-1][x-1] = '#';
					}

					if(x < w - 1 and shape[y][x+1] == '.')
					{
						all_stop = false;
						shape[y][x+1] = '#';
					}

					if(y < h - 1 and shape[y + 1][x] == '.')
					{
						all_stop = false;
						shape[y + 1][x] = '#';
					}

					if(x < w - 1 and y < h - 1 and shape[y + 1][x + 1] == '.')
					{
						all_stop = false;
						shape[y + 1][x + 1] = '#';
					}

					if(x > 0 and y < h - 1 and shape[y + 1][x - 1] == '.')
					{
						all_stop = false;
						shape[y + 1][x - 1] = '#';
					}	

					if(x < w - 1 and y > 0 and shape[y - 1][x + 1] == '.')
					{
						all_stop = false;
						shape[y - 1][x + 1] = '#';
					}				
				}
		}
	}

	size_t area() const
	{
		size_t area = 0;
		for(size_t y = 0; y < h; ++y)
		{
			bool inside = false;
			for(size_t x = 0; x < w; ++x)
			{
				if(shape[y][x] == '*')
					inside = not inside;

				if(shape[y][x] == '.' and inside)
					area += 1;

			}
		}
		
		return area;
	}

	void print(std::ostream& tty)
	{
		for(const auto& row : shape)
		{
			for(char s : row)
				tty.put(s);
			tty.put('\n');
		}
	}
};

struct pipe_map
{
	std::vector<std::vector<uint8_t>> map;
	coordinate start = {0, 0};

	uint8_t at(size_t x, size_t y) const {return map[y][x];}
	uint8_t at(const coordinate& c) const {return at(c.x, c.y);}
	size_t navigate_loop(directions dir, polygon *p)
	{
		size_t len = 1;

		// poly's boundaries
		p->shape[start.y][start.x] = '*'; 

		coordinate cur_pos = start + dir;
		directions coming_dir = flip_dir.at(dir);

		// No navigable pipe
		if(at(cur_pos) == directions::NONE or (at(cur_pos) & coming_dir) == 0)
			return 0;

		while(cur_pos != start)
		{
			// poly's boundaries
			p->shape[cur_pos.y][cur_pos.x] = '*'; 

			len += 1;
			directions out_dir = (directions)((uint8_t)at(cur_pos) & ~(uint8_t)coming_dir);
			if(std::popcount((uint8_t)out_dir) != 1)
				abort();

			cur_pos = cur_pos + out_dir;
			coming_dir = flip_dir.at(out_dir);

			if(at(cur_pos) == directions::NONE)
				abort();
		}

		p->shape[start.y][start.x] = '*'; 

		return len;
	}
};

static const std::map<char, uint8_t> look_up_table{
	{'.', directions::NONE}, {'|', directions::UP | directions::DOWN},
	{'-', directions::LEFT | directions::RIGHT}, {'L', directions::UP | directions::RIGHT},
	{'J', directions::UP | directions::LEFT}, {'7', directions::DOWN | directions::LEFT},
	{'F', directions::DOWN | directions::RIGHT}, {'.', directions::NONE},
	{'S', directions::START}
};

int main()
{
	std::string line;
	pipe_map pm;

	while(std::getline(std::cin, line))
	{
		std::vector<uint8_t> pline;
		for(auto it = line.begin(); it != line.end(); ++it)
		{
			pline.push_back(look_up_table.at(*it));
			if(*it == 'S')
				pm.start = {
					(long)std::distance(line.begin(), it),
					(long)pm.map.size()
				};
		}

		pm.map.push_back(std::move(pline));
	}

	std::unique_ptr<polygon> poly = std::make_unique<polygon>(pm.map.size(), pm.map[0].size());
	std::pair<size_t, directions> res;
	for(auto d : {directions::UP, directions::DOWN, directions::LEFT, directions::RIGHT})
		res = std::max(res, {pm.navigate_loop(d, poly.get()), d});

	std::cout << res.first/2 << std::endl;

	poly->flood();
	poly->print(std::cout);
	std::cout << poly->area() << std::endl;
}
