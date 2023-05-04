#include "straights_move.h"
#include <iostream>

StraightsMove::StraightsMove(const StraightsMoveType& type)
    : type{type}
{
    this->card = nullptr;
}

StraightsMove::StraightsMove(const StraightsMoveType& type, const Card* card)
    : type{type}
{
    this->card = new Card{card->getSuit(), card->getRank()};
}

StraightsMove::StraightsMove(const StraightsMove& sm) {
    // std::cout << "copy ctor invoked" << std::endl;
    this->type = sm.type;
    if (sm.card != nullptr)
        this->card = new Card{sm.card->getSuit(), sm.card->getRank()};
    else if (sm.card == nullptr)
        this->card = nullptr;
}

StraightsMove::StraightsMove(StraightsMove&& sm) {
    // std::cout << "move ctor invoked" << std::endl;
    this->type = sm.type;
    this->card = nullptr;
    std::swap(this->card, sm.card);
}

StraightsMoveType StraightsMove::getType() const {
    return this->type;
}

const Card* StraightsMove::getCard() const {
    return this->card;
}

void StraightsMove::setCard(const Card* card) {
    if (this->card != nullptr) {
        delete this->card;
    }
    this->card = new Card{card->getSuit(), card->getRank()};
}

bool StraightsMove::setCard(const std::string& cardString) {
    if (cardString.size() == 2) {
        const char rank = cardString.at(0);
        const char suit = cardString.at(1);
        int numRank = -1;

        if (suit != 'S' && suit != 'D' && suit != 'H' && suit != 'C')
            return false;
        
        if (rank == 'A') {
            numRank = 1;
        } else if ('2' <= rank && rank <= '9') {
            numRank = rank - '0';
        } else if (rank == 'T') {
            numRank = 10;
        } else if (rank == 'J') {
            numRank = 11;
        } else if (rank == 'Q') {
            numRank = 12;
        } else if (rank == 'K') {
            numRank = 13;
        } else {
            return false;
        }
        const Card * newCard = new Card {suit, numRank};
        std::cout << "Set move with card: " << *newCard << std::endl;
        if (this->card != nullptr)
            delete this->card;
        this->card = newCard;
        return true;
    } else {
        return false;
    }
}

StraightsMove& StraightsMove::operator=(const StraightsMove& sm) {
    // std::cout << "copy asgnmt operator" << std::endl;
    if (this->card != nullptr)
        delete this->card;
    if (sm.card != nullptr)
        this->card = new Card{sm.card->getSuit(), sm.card->getRank()};
    else if (sm.card == nullptr)
        this->card = nullptr;
    this->type = sm.type;
    return *this;
}

StraightsMove& StraightsMove::operator=(StraightsMove&& sm) {
    // std::cout << "move agnmt operator" << std::endl;
    this->type = sm.type;
    std::swap(this->card, sm.card);
    return *this;
}

StraightsMove::~StraightsMove() {
    if (this->card != nullptr) {
        delete this->card;
    }
}
