#include <iostream>
#include "player.h"
#include "card.h"
#include "straights_terminal.h"
#include "straights_move.h"
using namespace std;

Player::Player(const string& name) 
    : name{name}
    , prevRoundScore{0}
    , roundScore{0}
    , hand{vector<const Card*>{}}
    , discards{vector<const Card *>{}}
{
    for (int i = 0; i < discards.size(); ++i) {
        discards[i] = nullptr;
    }
}

std::string Player::getName() const {
    return this->name;
}

std::vector<const Card *> Player::getHand() const {
    return this->hand;
}

std::vector<const Card *> Player::getDiscards() const {
    return this->discards;
}

void Player::setHand(const std::vector<const Card *>& hand) {
    this->hand = hand;
}

bool Player::isCardInHand(const Card* card) const {
    for (const Card* handCard : this->hand) {
        if (*card == *handCard) {
            return true;
        }
    }
    return false;
}

// Removes the card from the player's hand.
// Assumes the card is already in the player's hand.
const Card* Player::removeCardFromHand(const Card* card) {
    int foundIndex = -1;
    for (int i = 0; i < this->hand.size(); ++i) {
        const Card* handCard = hand[i];
        if (*handCard == *card) {
            foundIndex = i;
            break;
        }
    }
    if (foundIndex == -1) {
        std::cerr << "could not remove card from hand cuz idk why" << std::endl;
    } else {
        const Card* oldCard = this->hand[foundIndex];
        this->hand.erase(this->hand.begin() + foundIndex);
        return oldCard;
    }
}

void Player::discard(const Card* card) {
    if (isCardInHand(card)) {
        const Card* oldCard = removeCardFromHand(card);
        discards.emplace_back(oldCard);
    }
}

void Player::resetDiscards() {
    this->discards.clear();
}

int Player::updateRoundScore() {
    int newRoundScore = 0;
    for (int i = 0; i < discards.size(); ++i) {
        const Card * card = discards[i];
        if (card != nullptr) {
            newRoundScore += card->getRank();
        }
    }
    this->roundScore = newRoundScore;
    return newRoundScore;
}

int Player::updatePrevRoundScore() {
    this->prevRoundScore += this->roundScore;
    this->roundScore = 0;
}

int Player::getPrevRoundScore() const {
    return this->prevRoundScore;
}

int Player::getRoundScore() const {
    return this->roundScore;
}

int Player::getTotalScore() const {
    return this->prevRoundScore + this->roundScore;
}

StraightsMove Player::requestMove(StraightsTerminal& terminal) const {
    cerr << "why did it ask for base Player for a move..." << endl;
}

Player::~Player() = default;
