#ifndef _PLAYER_H_
#define _PLAYER_H_
#include <vector>
#include <string>

struct Card; // forward declaration
struct StraightsTerminal; // forward declaration
struct StraightsMove; // forward declaration

// Abstract base player class
class Player {
    private:
        std::string name;
        int prevRoundScore;
        int roundScore;
        std::vector<const Card *> hand;
        std::vector<const Card *> discards;
    public:
        Player(const std::string& name);
        std::string getName() const; // Obtain the name of the player.
        std::vector<const Card *> getHand() const; // Get a copy of the cards in the hand of the player.
        std::vector<const Card *> getDiscards() const; // Geta copy of the cards that were discarded by the player.
        void setHand(const std::vector<const Card*>& hand);
        bool isCardInHand(const Card* card) const;
        const Card* removeCardFromHand(const Card* card);
        void discard(const Card* card);
        void resetDiscards();
        int updateRoundScore(); // Update the player's round score and get the new score.
        int updatePrevRoundScore(); // Update the game score for a player.
        int getPrevRoundScore() const; // Obtain the player's previous round score.
        int getRoundScore() const; // Obtain the player's current round score.
        int getTotalScore() const; // Get the player's game score.
        virtual StraightsMove requestMove(StraightsTerminal& terminal) const = 0;
        virtual ~Player() = 0;
};

#endif
