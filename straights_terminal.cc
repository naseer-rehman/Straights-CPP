#include <iostream>
#include <sstream>
#include "straights_terminal.h"
#include "straights_model.h"
#include "straights_move.h"
#include "straights_move_type.h"
#include "human_player.h"
#include "computer_player.h"
#include "card.h"

using namespace std;

StraightsTerminal::StraightsTerminal(int seed) {
    this->model = new StraightsModel{seed};
}

vector<string> StraightsTerminal::promptInput() const {
    cout << ">";
    istringstream iss{};
    string line;
    vector<string> arguments;
    if (getline(cin, line)) {
        iss.clear();
        iss.str(line);
        iss >> skipws;
        while ( true ) {
            string word;
            if ( iss >> word ) {
                arguments.emplace_back(word);
            } else if ( iss.eof() ) {
                break;
            } else if ( iss.fail() ) {
                cerr << "failed to write to a string when prompted input????" << endl;
            }
        }
    } else {
        cerr << "could not fetch line when prompted input : EOF(" << (cin.eof() ? "true" : "false") << ")" << endl;
    }
    return arguments;
}

void StraightsTerminal::askForPlayerTypes() {
    for (char n = '1'; n <= '4'; ++n) {
        string playerName{"Player"};
        playerName.push_back(n);
        cout << "Is " << playerName << " a human (h) or a computer (c)?" << endl;
        while (true) {
            vector<string> arguments = promptInput();
            if (cin.eof()) break;
            if (arguments.size() == 0) continue;
            if (arguments[0].length() == 0) continue;
            const char type = arguments[0].at(0);
            if (type == 'h') {
                this->model->addHumanPlayer(playerName);
                break;
            } else if (type == 'c') {
                this->model->addComputerPlayer(playerName);
                break;
            }
        }
    }
}

StraightsMove StraightsTerminal::handleMoveRequest(const HumanPlayer& hp) {
    cout << "Got request from Human" << endl;
    while (true) {
        vector<string> arguments = promptInput();
        if (arguments.size() == 0) continue;
        string command = arguments[0];
        if (command == "deck") {
            model->getDeck().print(cout);
        } else if (command == "play") {
            if (arguments.size() != 2) continue; // not enough arguments
            string card = arguments[1];
            if (card.size() != 2) continue;
            StraightsMoveType type = StraightsMoveType::PLAY;
            StraightsMove move{type};
            if (move.setCard(card) == false) continue;
            return move;
        } else if (command == "ragequit") {
            StraightsMoveType type = StraightsMoveType::RAGEQUIT;
            StraightsMove move{type};
            return move;
        } else if (command == "discard") {
            std::cout << "got discard" << std::endl;
            if (arguments.size() != 2) continue; // not enough arguments
            string card = arguments[1];
            std::cout << "got discard card" << std::endl;
            if (card.size() != 2) continue;
            StraightsMoveType type = StraightsMoveType::DISCARD;
            StraightsMove move{type};
            std::cout << "setting discard card" << std::endl;
            if (move.setCard(card) == false) continue;
            std::cout << "set discard card" << std::endl;
            return move;
            // cout << "discarding something..." << endl;
        } else if (command == "quit") {
            // cout << "quitting" << endl;
            StraightsMoveType type = StraightsMoveType::QUIT;
            StraightsMove move{type};
            return move;
        }
    }
}

StraightsMove StraightsTerminal::handleMoveRequest(const ComputerPlayer& cp) {
    // If the player has no hand cards,
    // and no legal plays, then something prob went wrong
    cout << ">";
    vector<const Card*> legalMoves = model->getLegalMoves();
    if (legalMoves.size() > 0) {
        return StraightsMove{StraightsMoveType::PLAY, legalMoves[0]};
    } else {
        vector<const Card*> hand = cp.getHand();
        if (hand.size() == 0) {
            cerr << "uh empty hands for the computer bro" << endl;
            return StraightsMove{StraightsMoveType::QUIT};
        }
        return StraightsMove{StraightsMoveType::DISCARD, hand[0]};
    }
}

void StraightsTerminal::listPile(const std::vector<const Card*>& pile, const std::string& pileName) const {
    
    cout << pileName << " [" << pile.size() << "]:";
    for (int i = 0; i < pile.size(); ++i) {
        const Card* c = pile[i];
        cout << " " << *c;
    }
    cout << endl;
}

void StraightsTerminal::displayCurrentGameState() const {
    cout << "Cards on the table:" << endl;
    listPile(model->getClubsPile(), "Clubs");
    listPile(model->getDiamondsPile(), "Diamonds");
    listPile(model->getHeartsPile(), "Hearts");
    listPile(model->getSpadesPile(), "Spades");
    listPile(model->getCurrentPlayer()->getHand(), "Your hand");
    listPile(model->getLegalMoves(), "Legal plays");
}

void StraightsTerminal::makeMove(StraightsMove move) const {
    Player* plr = model->getCurrentPlayer();
    string plrName = plr->getName();
    StraightsMoveType type = move.getType();
    const Card* moveCard = move.getCard();
    bool successful = model->makeMove(move);
    if (type == StraightsMoveType::PLAY) {
        if (successful)
            cout << plrName << " plays " << *moveCard << "." << endl;
        else
            cout << "That is not a legal play." << endl;
    } else if (type == StraightsMoveType::DISCARD) {
        if (successful)
            cout << plrName << " discards " << *moveCard << "." << endl;
        else
            cout << "You have a legal play. You may not discard." << endl;
    } else if (type == StraightsMoveType::RAGEQUIT) {
        if (successful)
            cout << plrName << " ragequits. A computer will now take over." << endl;
        else
            cerr << "Could not replace with a computer?!?!!?!?!? uh oh!!!!!" << endl;
    } else if (type == StraightsMoveType::QUIT) {
        // probably do nothing.
    }
}

void StraightsTerminal::displayEndOfRoundInfo() const {
    const vector<Player *>& players = model->getPlayers();
    for (int i = 0; i < players.size(); ++i) {
        Player* plr = players[i];
        plr->updateRoundScore();
        vector<const Card*> discards = plr->getDiscards();
        listPile(discards, plr->getName() + "'s discards");
        cout << plr->getName() << "'s score: ";
        cout << plr->getPrevRoundScore() << " + " << plr->getRoundScore();
        cout << " = " << plr->getTotalScore() << endl;
    }
}

void StraightsTerminal::play() {
    // Generate players
    askForPlayerTypes();
    // Begin the game
    bool hasQuit = false;
    while (model->hasPlayerWon() == false && hasQuit == false) {
        model->startNewRound();
        while (model->hasRoundEnded() == false && hasQuit == false) {
            displayCurrentGameState();
            Player* plr = model->getCurrentPlayer();
            StraightsMove move = plr->requestMove(*this);
            if (move.getType() == StraightsMoveType::QUIT) {
                hasQuit = true;
                break;
            }
            this->makeMove(move);
            // cout << "Round ended:  " << (model->hasRoundEnded() ? "true" : "false") << endl;
        }
        this->displayEndOfRoundInfo();
    }
}

StraightsTerminal::~StraightsTerminal() {
    delete this->model;
    // cout << "successfully deleted model in straights terminal" << endl;
}