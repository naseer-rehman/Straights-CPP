#include "computer_player.h"
#include "straights_terminal.h"
#include "straights_move.h"
using namespace std;

ComputerPlayer::ComputerPlayer(const string& name) 
    : Player{name}
{}

StraightsMove ComputerPlayer::requestMove(StraightsTerminal& terminal) const {
    return terminal.handleMoveRequest(*this);
}
