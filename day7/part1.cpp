#include <bits/chrono.h>
#include <cstddef>
#include <cstdlib>
#include <initializer_list>
#include <iostream>
#include <map>
#include <algorithm>
#include <utility>
#include <vector>

typedef char card_kind;

const auto cards = {'2', '3', '4', '5', '6', '7', '8', '9', 'T', 'J', 'Q', 'K', 'A'};

struct card
{
	unsigned weight;
	card_kind c;
	
	card(char c): c(c)
	{
		auto w_it = std::find(cards.begin(), cards.end(), c);
		if(w_it == cards.end())
			abort();

		weight = std::distance(cards.begin(), w_it);		
	}

	bool operator>(const card& b) const {return weight > b.weight;}
	bool operator<(const card& b) const {return weight < b.weight;}
	bool operator==(const card& b) const {return weight == b.weight;}
	bool operator!=(const card& b) const {return not operator==(b);}
};

class hand
{
	enum type
	{
		HIGH_CARD = 0,
		ONE_PAIR, TWO_PAIR, THREE_OF_A_KIND, FULL_HOUSE, FOUR_OF_A_KIND, FIVE_OF_A_KIND
	};
	std::map<card, unsigned> cards;
	std::vector<card> cards_r;
	type t;
public:
	hand(std::vector<card> cs): cards_r(cs.begin(), cs.end())
	{
		if(cs.size() != 5)
			abort();

		for(auto c : cs)
			cards[c]++;

		if(cards.size() == 1)
		{
			t = FIVE_OF_A_KIND;
			return;
		}

		if(cards.size() == 2 and (cards.begin()->second == 4 or cards.begin()->second == 1))
		{
			t = FOUR_OF_A_KIND;
			return;
		}

		if(cards.size() == 2 and (cards.begin()->second == 3 or cards.begin()->second == 2))
		{
			t = FULL_HOUSE;
			return;
		}

		if(cards.size() == 3)
		{
			for(const auto& card : cards)
				if(card.second == 3)
				{
					t = THREE_OF_A_KIND;
					return;
				}
			
			for(const auto& card : cards)
				if(card.second == 1)
				{
					t = TWO_PAIR;
					return;
				}
		}

		if(cards.size() == 4)
		{
			for(const auto& card : cards)
				if(card.second == 2)
				{
					t = ONE_PAIR;
					return;
				}
		}

		if(cards.size() != 5)
			abort();
		t = HIGH_CARD;
	}

	std::string to_string() const
	{
		std::string s;
		for(const auto& c : cards_r)
			s.push_back(c.c);
		return s;
	}

	bool operator>(const hand& b) const
	{
		if(t != b.t)
			return t > b.t;

		for(size_t i = 0; i < 5; ++i)
			if(cards_r[i] != b.cards_r[i])
				return cards_r[i] > b.cards_r[i];
		
		return false;
	}

	bool operator<(const hand& b) const
	{
		return *this != b and not (*this > b);
	}

	bool operator==(const hand& b) const
	{
		if(t != b.t)
			return false;

		for(size_t i = 0; i < 5; ++i)
			if(cards_r[i] != b.cards_r[i])
				return false;
		
		return true;
	}

	bool operator !=(const hand& b) const {return not operator==(b);}
};

int main()
{
	std::string play;
	unsigned long long score;
	std::map<hand, unsigned long long> plays;

	while(std::cin >> play >> score)
	{
		std::vector<card> cards(play.begin(), play.end());
		hand h(cards);
		plays[h] = score;
	}

	unsigned rank = 1;
	unsigned long long fscore1 = 0;
	for(const auto& [hand, score] : plays)
	{
		std::cout << hand.to_string() << '\t' << rank << '\t' << score << '\n';
		fscore1 += rank++ * score;
	}

	std::cout << fscore1 << std::endl;

	return 0;
}
