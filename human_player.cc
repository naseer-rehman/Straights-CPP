#include "human_player.h"
#include "straights_terminal.h"
#include "straights_move.h"

HumanPlayer::HumanPlayer(const std::string& name)
    : Player{name}
{}

StraightsMove HumanPlayer::requestMove(StraightsTerminal& terminal) const {
    return terminal.handleMoveRequest(*this);
}
