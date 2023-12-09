#include <cctype>
#include <cstddef>
#include <iostream>
#include <string>
#include <vector>

static unsigned parse(const std::string& s)
{
	static const std::vector<std::string> digit = {
		"one", "two", "three", "four", "five", "six", "seven", "eight", "nine"
	};

	if(std::isdigit(s[0]))
		return std::stoi(s.substr(0, 1));

	for(unsigned d = 1; d <= digit.size(); ++d)
	{
		if(s.starts_with(digit[d - 1]))
			return d;
	}

	return 0;
}

int main()
{
	std::string line;
	unsigned long long sum = 0;

	while(std::getline(std::cin, line))
	{
		unsigned first_digit = 0, last_digit = 0;

		for(size_t i = 0; i < line.size(); ++i)
		{
			auto digit = parse(line.substr(i));
			if(not digit)
				continue;

			if(first_digit == 0)
			{
				first_digit = digit;
				last_digit = first_digit;
			}
			else
				last_digit = digit;
		}

		unsigned number = first_digit*10 + last_digit;
		std::clog << number << '\n';
		sum += number;
	}

	std::cout << sum << std::endl;

	return 0;
}
