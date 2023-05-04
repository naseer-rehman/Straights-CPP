#ifndef _HUMAN_PLAYER_H_
#define _HUMAN_PLAYER_H_
#include <string>
#include <vector>
#include "player.h"

struct StraightsTerminal; // forward declaration
struct StraightsMove; // forward declaration

class HumanPlayer : public Player {
    private:
    public:
        HumanPlayer(const std::string& name);
        virtual StraightsMove requestMove(StraightsTerminal& terminal) const override;
        virtual ~HumanPlayer() override = default;
};

#endif
