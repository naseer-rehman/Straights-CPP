#ifndef _CARD_H_
#define _CARD_H_
#include <string>

class Card {
    private:
        char suit;
        int rank;
        std::string str;
    public:
        Card(char suit, int rank);
        char getSuit() const;
        int getRank() const;
        friend std::ostream & operator<<(std::ostream & out, const Card & c);
};

std::ostream & operator<<(std::ostream & out, const Card & c);
bool operator==(const Card& lhs, const Card& rhs);

#endif 