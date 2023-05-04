#ifndef _STRAIGHTS_MODEL_H_
#define _STRAIGHTS_MODEL_H_
#include <vector>
#include "deck.h"
#include "straights_move.h"

using namespace std;

struct Card; // forward declaration
struct Player; // forward declaration

// Cards are managed by Deck
// Players are managed by Terminal
class StraightsModel {
    private:
        Deck deck;                      // the deck to deal and shuffle with
        int roundNumber;                // indicates the round number
        int moveNumber;                 // indicates the move number
        int turnNumber;                 // indicates which player's turn it is
        vector<Player*> players;        // stores each of the 4 player objects
        vector<const Card*> spadesPile;
        vector<const Card*> clubsPile;
        vector<const Card*> heartsPile;
        vector<const Card*> diamondsPile;
        
        void incrementTurn();
        bool play(const Card* card);
        bool discard(const Card* card);
        bool ragequit();
        bool canAddCardToPile(const vector<const Card*>& pile, const char pileSuit, const Card* card) const;
        void addCardToPile(vector<const Card*>& pile, const Card* card);
        vector<const Card*> getPlayerLegalMoves(Player* player) const;
        bool playerHasLegalMove(Player* player) const;
        bool legalMoveExists() const;
        void resetPiles();
    public:
        StraightsModel(int seed);
        void startNewRound();
        bool makeMove(const StraightsMove& move);
        void addPlayer(Player*);
        void addHumanPlayer(const string& name);
        void addComputerPlayer(const string& name);
        Player* getCurrentPlayer() const;
        vector<const Card*> getLegalMoves() const;
        vector<const Card*> getDiamondsPile() const;
        vector<const Card*> getSpadesPile() const;
        vector<const Card*> getHeartsPile() const;
        vector<const Card*> getClubsPile() const;
        bool hasRoundEnded() const;
        bool hasPlayerWon() const;
        const vector<Player *>& getPlayers() const;
        vector<Player*> getWinners() const;
        const Deck& getDeck() const;
        ~StraightsModel();
};

#endif
