#include <iostream>
#include <stdexcept>
#include "deck.h"
#include "player.h"
#include "human_player.h"
#include "computer_player.h"
#include "straights_terminal.h"
#define STUDENT_ID 20888920

using namespace std;

int main(int argc, char **argv) {
    int SEED = STUDENT_ID; // My student id
    if ( argc == 2 ) {
        try {
			SEED = stoi( string{ argv[1] } );
		} catch( invalid_argument & e ) {
			cerr << e.what() << endl;
			return 1;
		} catch( out_of_range & e ) {
			cerr << e.what() << endl;
			return -1;
		}
    } else if ( argc > 2 ) {
        cerr << "Too many CLI arguments: " << (argc-1) << " arguments" << endl;
        return 2;
    }
    cout << "Seed: " << SEED << endl;
    StraightsTerminal terminal = StraightsTerminal{SEED};
    terminal.play();
}
