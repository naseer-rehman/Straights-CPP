#include "card.h"
#include <iostream>

Card::Card(char suit, int rank)
    : suit{suit}, rank{rank} 
{
    this->str = std::string{};
    char rank_char = '-';
    if (rank == 1) {
        rank_char = 'A';
    } else if (2 <= rank && rank <= 9) {
        rank_char = '0' + rank;
    } else if (rank == 10) {
        rank_char = 'T';
    } else if (rank == 11) {
        rank_char = 'J';
    } else if (rank == 12) {
        rank_char = 'Q';
    } else if (rank == 13) {
        rank_char = 'K';
    }
    this->str.push_back(rank_char);
    this->str.push_back(suit);
}


char Card::getSuit() const {
    return this->suit;
}

int Card::getRank() const {
    return this->rank;
}

std::ostream& operator<<(std::ostream& out, const Card & c) {
    out << c.str;
    return out;
}

bool operator==(const Card& lhs, const Card& rhs) {
    return (lhs.getRank() == rhs.getRank() && lhs.getSuit() == rhs.getSuit());
}

