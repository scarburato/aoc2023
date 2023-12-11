#pragma once

#include <cstddef>
#include <cstdlib>
#include <map>
#include <ostream>

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
	long x = 0;
	long y = 0;

	coordinate(long x, long y): x(x), y(y) {}
	coordinate(const directions& d)
	{
		if(d & directions::UP) {x = 0; y = -1; return;}
		if(d & directions::DOWN) {x = 0; y = +1; return;}
		if(d & directions::RIGHT) {x = 1; y = 0; return;}
		if(d & directions::LEFT) {x = -1; y = 0; return;}
		abort();
	}
	coordinate() = default;

	unsigned long norm_manhattan(const coordinate& b) const
	{
		return std::abs(x - b.x) + std::abs(y - b.y);
	}

	coordinate operator+(const coordinate& b) const {return {x + b.x, y + b.y};}
	bool operator==(const coordinate& b) const {return x == b.x and y == b.y;}
	bool operator!=(const coordinate& b) const {return x != b.x or y != b.y;}

	friend std::ostream& operator<<(std::ostream& tty, const coordinate& c)
	{
		tty << "(" << c.x << ", " << c.y << ")";
		return tty;
	}
};
