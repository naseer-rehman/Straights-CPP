#ifndef _DECK_H_
#define _DECK_H_
#include <vector>
#include <random>
#include <iostream>
#include "card.h"

class Deck {
    private:
        std::vector<const Card *> cards;
        int seed;
        std::default_random_engine rng;
    public:
        Deck(int seed);
        Deck(Deck && d);
        Deck(const Deck & d);
        void shuffle();
        const Card * getCard(int i);
        std::vector<const Card*> dealCards(int player) const;
        void print(std::ostream & out) const;
        Deck& operator=(Deck && d);
        Deck& operator=(const Deck &d);
        ~Deck();
};

#endif