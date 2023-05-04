#ifndef _STRAIGHTS_TERMINAL_H_
#define _STRAIGHTS_TERMINAL_H_
#include <string>
#include <vector>

struct StraightsModel; // forward declaration
struct StraightsMove; // forward declaration
struct HumanPlayer; // forward declaration
struct ComputerPlayer; // forward declaration
struct Card;

class StraightsTerminal {
    private:
        StraightsModel* model;
        std::vector<std::string> promptInput() const;
        void askForPlayerTypes();
        void listPile(const std::vector<const Card*>& pile, const std::string& pileName) const;
        void displayCurrentGameState() const;
        void makeMove(StraightsMove move) const;
        void displayEndOfRoundInfo() const;
    public:
        StraightsMove handleMoveRequest(const HumanPlayer& hp);
        StraightsMove handleMoveRequest(const ComputerPlayer& cp);
        StraightsTerminal(int seed);
        void play();
        ~StraightsTerminal();
};

#endif
