#include "deck.h"
#include <string>
#include <vector>
#include <algorithm>
#include <chrono>

Deck::Deck(int seed) : seed{seed} {
    // create a vector containing 52 cards, with each 
    this->rng = std::default_random_engine{seed};
    this->cards = std::vector<const Card *>{52};
    const std::string suits = "HDSC";
    for (int i = 0; i < suits.length(); ++i) {
        char suit = suits.at(i);
        for (int rank = 1; rank <= 13; ++rank) {
            this->cards[i * 13 + (rank - 1)] = new Card{suit, rank};
        }
    }
}

Deck::Deck(const Deck& d) {
    this->seed = d.seed;
    this->rng = d.rng;
    this->cards = std::vector<const Card *>{52};
    for (int i = 0; i < 52; ++i) {
        const Card* otherCard = d.cards[i];
        const char suit = otherCard->getSuit();
        const int rank = otherCard->getRank(); 
        this->cards[i] = new Card{suit, rank};
    }
}

Deck::Deck(Deck&& d) {
    this->seed = d.seed;
    this->rng = d.rng;
    this->cards = std::vector<const Card *>{52};
    for (int i = 0; i < 52; ++i) {
        this->cards[i] = d.cards[i];
        d.cards[i] = nullptr;
    }
}

Deck::~Deck() {
    // Free the memory of each card.
    for (int i = 0; i < this->cards.size(); ++i) {
        if (this->cards[i] != nullptr) {
            delete this->cards[i];
        }
    }
}

void Deck::shuffle() {
    std::shuffle( this->cards.begin(), this->cards.end(), this->rng );
}

const Card* Deck::getCard(int i) {
    return this->cards[i];
}

// 0 <= player <= 3 (4 players)
std::vector<const Card*> Deck::dealCards(int player) const {
    std::vector<const Card*> dealt;
    for (int i = 0; i < 13; ++i)
        dealt.emplace_back(cards[player * 13 + i]);
    return dealt;
}

void Deck::print(std::ostream & out) const {
    for (int r = 0; r < 4; ++r ) {
        for (int c = 0; c < 13; ++c) {
            out << *(this->cards[r * 13 + c]) << " ";
        }
        out << std::endl;
    }
}

Deck& Deck::operator=(Deck&& d) {
    this->seed = d.seed;
    this->rng = d.rng;
    for (int i = 0; i < 52; ++i) {
        if (this->cards[i] != nullptr) {
            delete this->cards[i];
        }
        this->cards[i] = d.cards[i];
        d.cards[i] = nullptr;
    }
    return *this;
}

Deck& Deck::operator=(const Deck& d) {
    this->seed = d.seed;
    this->rng = d.rng;
    for (int i = 0; i < 52; ++i) {
        const Card* otherCard = d.cards[i];
        const char suit = otherCard->getSuit();
        const int rank = otherCard->getRank();
        if (this->cards[i] != nullptr) {
            delete this->cards[i];
        }
        this->cards[i] = new Card{suit, rank};
    }
    return *this;
}
