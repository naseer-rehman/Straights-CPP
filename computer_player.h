#ifndef _COMPUTER_PLAYER_H_
#define _COMPUTER_PLAYER_H_
#include "player.h"
#include <string>

struct StraightsTerminal; // forward declaration
struct StraightsMove; // forward declaration

class ComputerPlayer : public Player {
    private:
    public:
        ComputerPlayer(const std::string& name);
        virtual StraightsMove requestMove(StraightsTerminal& terminal) const override;
        virtual ~ComputerPlayer() override = default;
};

#endif
