CXX = g++
CXXFLAGS = -std=c++14 -Wall -Wextra -Wconversion -pedantic -MMD -g
EXEC = straights
OBJECTS = main.o card.o deck.o straights_move_type.o straights_move.o player.o human_player.o computer_player.o straights_model.o straights_terminal.o
DEPENDS = ${OBJECTS:.o=.d}

${EXEC}: ${OBJECTS}
	${CXX} ${CXXFLAGS} ${OBJECTS} -o ${EXEC}

%.o: %.cc
	${CXX} ${CXXFLAGS} -c $<

-include ${DEPENDS}

.PHONY: clean

clean:
	rm ${OBJECTS} ${EXEC} ${DEPENDS}
