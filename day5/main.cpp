#include <cstddef>
#include <iostream>
#include <map>
#include <mutex>
#include <ostream>
#include <string>
#include <vector>
#include "../util.hpp"
#include "../thread_pool.hpp"

using pos_t = unsigned long long;

struct range
{
    pos_t lb;
    pos_t size;

    bool operator>(const range& b) const {return lb > b.lb;}
};

class Map
{
private:
    std::map<pos_t, range, std::greater<>> map;
public:
    void add(pos_t lb, pos_t lb2, pos_t range)
    {
        map[lb] = {lb2, range};
    }

    pos_t trasform(pos_t n) const
    {
        auto find_it = map.lower_bound(n);
        
        // base case: perform identity trasform
        if(find_it == map.end() or n >= find_it->first + find_it->second.size)
            return n;

        return find_it->second.lb + (n - find_it->first);
    }

    std::vector<range> trasform(const range& r)
    {
        std::vector<range> tranges;
	/** @todo*/

	return tranges;
    }
};

int main()
{
    // Read seeds
    std::string seeds_line, tmp;
    std::getline(std::cin, seeds_line);
    auto seeds = parse_ints(seeds_line.substr(seeds_line.find(':') + 1));
    
    std::vector<range> seeds_ranges;
    for(size_t i = 0; i < seeds.size(); i += 2)
    {
        seeds_ranges.push_back({seeds[i], seeds[i + 1]});
    }

    // trasforms
    std::vector<Map> maps;

    while(std::getline(std::cin, tmp))
    {
        if(tmp.empty())
            continue;

        if(not std::isdigit(tmp[0]))
        {
            maps.emplace_back();
            continue;
        }

        auto& map = *maps.rbegin();

        auto numbers = parse_ints(tmp);
        map.add(numbers[1], numbers[0], numbers[2]);
    }

    pos_t best_post = 0xffffffff;
    for(const auto seed : seeds)
    {
        pos_t e = seed;
        for(const auto& map : maps)
        {
            e = map.trasform(e);
        }
        best_post = std::min(best_post, e);
    }

    thread_pool pl(16);
    std::mutex bm;
    pos_t best_post2 = 0xffffffff;

    // brute force method
    for(const auto seeds_range : seeds_ranges)
    {
	pl.add_job([&bm, &best_post2, seeds_range, &maps](){
		std::cout << "Processing " << seeds_range.lb << std::endl;
		pos_t best_post3 = 0xffffffff;
		for(pos_t i = 0; i < seeds_range.size; ++i)
		{
			pos_t e = seeds_range.lb + i;
			for(const auto& map : maps)
			{
				e = map.trasform(e);
			}
			best_post3 = std::min(best_post3, e);
		}

		std::unique_lock bm_lock(bm);
		best_post2 = std::min(best_post2, best_post3);
	});
        
    }
    
    pl.wait_all_jobs();


    std::cout << best_post << '\t' << best_post2 << std::endl;
}
