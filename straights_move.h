#ifndef _STRAIGHTS_MOVE_H_
#define _STRAIGHTS_MOVE_H_
#include <string>
#include "straights_move_type.h"
#include "card.h"

class StraightsMove {
    private:
        StraightsMoveType type;
        const Card* card;
    public:
        StraightsMove(const StraightsMoveType& type);
        StraightsMove(const StraightsMoveType& type, const Card* card);
        StraightsMove(const StraightsMoveType& type, const std::string& cardString);
        StraightsMove(const StraightsMove& sm);
        StraightsMove(StraightsMove&& sm);
        StraightsMoveType getType() const;
        const Card* getCard() const;
        void setCard(const Card* card);
        bool setCard(const std::string& cardString);
        StraightsMove& operator=(const StraightsMove& sm);
        StraightsMove& operator=(StraightsMove&& sm);
        ~StraightsMove();
};

#endif
