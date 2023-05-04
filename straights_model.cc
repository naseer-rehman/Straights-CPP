#include "straights_model.h"
#include "card.h"
#include "player.h"
#include "computer_player.h"
#include "human_player.h"

using namespace std;

StraightsModel::StraightsModel(int seed)
    : deck{Deck{seed}}
    , roundNumber{0}
    , moveNumber{0}
    , turnNumber{0}
    , players{vector<Player *>{}}
    , spadesPile{vector<const Card *>{}}
    , clubsPile{vector<const Card *>{}}
    , heartsPile{vector<const Card *>{}}
    , diamondsPile{vector<const Card *>{}}
{}

void StraightsModel::resetPiles() {
    for (int i = 0; i < spadesPile.size(); ++i)
        if (spadesPile[i] != nullptr)
            delete spadesPile[i];
    for (int i = 0; i < clubsPile.size(); ++i)
        if (clubsPile[i] != nullptr)
            delete clubsPile[i];
    for (int i = 0; i < heartsPile.size(); ++i)
        if (heartsPile[i] != nullptr)
            delete heartsPile[i];
    for (int i = 0; i < diamondsPile.size(); ++i)
        if (diamondsPile[i] != nullptr)
            delete diamondsPile[i];
    spadesPile.clear();
    clubsPile.clear();
    heartsPile.clear();
    diamondsPile.clear();
}

// Checks to see if the card can be added to the given pile.
// I might be able to remove the pileSuit parameter if I assume the card
// is of the same suit as the pile.
bool StraightsModel::canAddCardToPile(const vector<const Card*>& pile, const char pileSuit, const Card* card) const {
    const int rank = card->getRank();
    const char suit = card->getSuit();
    if (suit != pileSuit) return false;
    if (pile.size() == 0) {
        return card->getRank() == 7;
    } else {
        const Card* bottomCard = pile[0];
        const Card* topCard = pile[pile.size() - 1];
        if (suit == bottomCard->getSuit() && rank + 1 == bottomCard->getRank()) {
            return true;
        } else if (suit == topCard->getSuit() && rank - 1 == topCard->getRank()) {
            return true;
        }
    }
    return false;
}

// Guaranteed that the card has the same suit as the pile.
void StraightsModel::addCardToPile(vector<const Card*>& pile, const Card* card) {
    const char suit = card->getSuit();
    const int rank = card->getRank();
    const Card* cardCopied = new Card{card->getSuit(), card->getRank()};
    if (pile.size() == 0) {
        pile.emplace_back(cardCopied);
        return;
    }
    const Card* topCard = pile[pile.size() - 1];
    const Card* bottomCard = pile[0];
    if (topCard->getRank() + 1 == rank) {
        // Insert back
        pile.emplace_back(cardCopied);
    } else if (bottomCard->getRank() - 1 == rank) {
        // Insert front
        pile.insert(pile.begin(), cardCopied);
    } else {
        cerr << "should have been able to add card to pile, what happened?!?!??" << endl;
    }
}

vector<const Card*> StraightsModel::getPlayerLegalMoves(Player* player) const {
    vector<const Card*> legalMoves;
    vector<const Card*> hand = player->getHand();
    if (moveNumber == 1) {
        // If this guy does not have a 7S, something might be wrong.
        for (int i = 0; i < hand.size(); ++i) {
            const Card* card = hand[i];
            if (card->getRank() == 7 && card->getSuit() == 'S') {
                legalMoves.emplace_back(card);
                break;
            }
        }
    } else {
        // Iterate through each card
        // Check if the card is a 7 of any suit (other than spades?)
        // Check if the card is an adjacent rank to any end card for any pile
        for (int i = 0; i < hand.size(); ++i) {
            const Card* card = hand[i];
            const char suit = card->getSuit();
            bool legalCard = false;
            switch (suit) {
                case 'H':
                    legalCard = canAddCardToPile(heartsPile, suit, card);
                    break;
                case 'S':
                    legalCard = canAddCardToPile(spadesPile, suit, card);
                    break;
                case 'D':
                    legalCard = canAddCardToPile(diamondsPile, suit, card);
                    break;
                case 'C':
                    legalCard = canAddCardToPile(clubsPile, suit, card);
                    break;
                default:
                    cerr << "Wrong suit????? -> " << suit << endl; 
            }
            if (legalCard) {
                legalMoves.emplace_back(card);
            }
        }
    }
    return legalMoves;
}

bool StraightsModel::playerHasLegalMove(Player* player) const {
    return this->getPlayerLegalMoves(player).size() != 0;
}

// Function handling the current player playing the given card.
bool StraightsModel::play(const Card* card) {
    Player* plr = this->getCurrentPlayer();
    vector<const Card*> legalMoves = this->getPlayerLegalMoves(plr);
    bool cardIsValid = false;
    for (const Card* legalCard : legalMoves) {
        if (*card == *legalCard) {
            cardIsValid = true;
            break;
        }
    }
    if (!cardIsValid) return false;
    // Play the card since it can be played.
    const char suit = card->getSuit();
    if (suit == 'S')
        this->addCardToPile(this->spadesPile, card);
    else if (suit == 'H')
        this->addCardToPile(this->heartsPile, card);
    else if (suit == 'D')
        this->addCardToPile(this->diamondsPile, card);
    else if (suit == 'C')
        this->addCardToPile(this->clubsPile, card);
    else {
        cerr << "wrong suit when playing a card..." << endl;
        return false;
    }
    plr->removeCardFromHand(card);
    return true;
}

bool StraightsModel::discard(const Card* card) {
    Player* plr = this->getCurrentPlayer();
    if (this->playerHasLegalMove(plr)) return false;
    // Player does not have a legal move.
    plr->discard(card);
    return true;
}

bool StraightsModel::ragequit() {
    const int playerID = turnNumber;
    Player *oldPlr = this->players[playerID];
    // ok now I have to copy object
    // delete this->players[playerID];
    this->players[playerID] = new ComputerPlayer{oldPlr->getName()};
    *(this->players[playerID]) = *oldPlr;
    delete oldPlr;
    std::cout << "StraightsModel: done ragequitting" << std::endl;
    return true;
}

// This function checks to see if a legal move exists for at least one player in the game.
bool StraightsModel::legalMoveExists() const {
    int playerID = turnNumber;
    const int originalPlayerID = playerID;
    do {
        if (this->playerHasLegalMove(players[playerID])) {
            // std::cout << "StraightsModel: Found a legal move" << std::endl;
            return true;
        } else if (players[playerID]->getHand().size() != 0) {
            return true;
        }
        playerID = (playerID + 1) % 4;
    } while (playerID != originalPlayerID);
    // std::cout << "StraightsModel: Could not find a legal move" << std::endl;
    return false;
}

void StraightsModel::incrementTurn() {
    int playerID = turnNumber;
    const int originalPlayerID = playerID;
    do {
        if (this->playerHasLegalMove(players[playerID])) {
            // std::cout << "StraightsModel: Found a legal move" << std::endl;
            turnNumber = playerID;
            return;
        } else if (players[playerID]->getHand().size() != 0) {
            turnNumber = playerID;
            return;
        }
        playerID = (playerID + 1) % 4;
    } while (playerID != originalPlayerID);
}

void StraightsModel::addPlayer(Player * player) {
    players.emplace_back(player);
}

void StraightsModel::addHumanPlayer(const string& name) {
    this->players.emplace_back(new HumanPlayer{name});
}

void StraightsModel::addComputerPlayer(const string& name) {
    this->players.emplace_back(new ComputerPlayer{name});
}

Player* StraightsModel::getCurrentPlayer() const {
    return players[turnNumber];
}

void StraightsModel::startNewRound() {
    // Reset the round number
    // Reset the move number
    // Shuffle the deck
    // Deal the cards to each player
    // Reset the table
    //      Empty the piles
    // Find which player has 7S and set it to their turn
    cout << "started a new round" << endl;
    ++roundNumber;
    moveNumber = 1;
    deck.shuffle();
    for (int player = 0; player < 4; ++player) {
        players[player]->updatePrevRoundScore();
        players[player]->setHand(deck.dealCards(player));
        players[player]->resetDiscards();
    }
    resetPiles();
    for (int p = 0; p < 4; ++p) {
        vector<const Card*> hand = players[p]->getHand();
        bool hasCard = false;
        for (int i = 0; i < hand.size(); ++i) {
            const Card * card = hand[i];
            if (card->getRank() == 7 && card->getSuit() == 'S') {
                turnNumber = p;
                hasCard = true;
                break;
            }
        }
        if (hasCard) break;
    }
}

vector<const Card*> StraightsModel::getDiamondsPile() const {
    return this->diamondsPile;
}

vector<const Card*> StraightsModel::getSpadesPile() const {
    return this->spadesPile;
}

vector<const Card*> StraightsModel::getHeartsPile() const {
    return this->heartsPile;
}

vector<const Card*> StraightsModel::getClubsPile() const {
    return this->clubsPile;
}

vector<const Card*> StraightsModel::getLegalMoves() const {
    return this->getPlayerLegalMoves(this->getCurrentPlayer());
}

bool StraightsModel::makeMove(const StraightsMove& move) {
    StraightsMoveType moveType = move.getType();
    const Card* card = move.getCard();
    if (moveType == StraightsMoveType::PLAY) {
        if (this->play(card)) {
            this->incrementTurn();
            ++moveNumber;
            return true;
        }
    } else if (moveType == StraightsMoveType::DISCARD) {
        if (this->discard(card)) {
            this->incrementTurn();
            ++moveNumber;
            return true;
        }
    } else if (moveType == StraightsMoveType::RAGEQUIT) {
        if (this->ragequit()) {
            // don't increment moveNumber?
            return true;
        }
    } else if (moveType == StraightsMoveType::QUIT) {
        // This should basically never happen.
        return true;
    }
    return false;
}

bool StraightsModel::hasRoundEnded() const {
    return !this->legalMoveExists();
}

bool StraightsModel::hasPlayerWon() const {
    bool hasWon = false;
    for (int i = 0; i < 4; ++i) {
        players[i]->updateRoundScore();
        if (players[i]->getTotalScore() >= 80)
            hasWon = true;
    }
    return hasWon;
}

vector<Player*> StraightsModel::getWinners() const {
    vector<Player *> winners;
    int minScore = players[0]->getTotalScore();
    for (int i = 0; i < 4; ++i) {
        const int playerScore = players[i]->getTotalScore();
        minScore = playerScore < minScore ? playerScore : minScore; 
    }
    for (int i = 0; i < 4; ++i) {
        const int playerScore = players[i]->getTotalScore();
        if (playerScore == minScore) {
            winners.emplace_back(players[i]);
        }
    }
    return winners;
}

const vector<Player*>& StraightsModel::getPlayers() const {
    return this->players;
}

const Deck& StraightsModel::getDeck() const {
    return this->deck;
}

StraightsModel::~StraightsModel() {
    for (int i = 0; i < players.size(); ++i) {
        delete players[i];
    }
    this->resetPiles();
    // cout << "successfully deleted players in straights model" << endl;
}

/*
To Do:
    - implement discard [done ish]
    - implement [done ish?]
    - implement a destructor [:|]
Memory this class manages:
    - Players
*/