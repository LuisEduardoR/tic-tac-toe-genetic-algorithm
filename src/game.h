#ifndef GAME_H
#define GAME_H
#include "pop.h"
#include <vector>
#include <utility>

class Game {

    public:
        char place[3][3];

        std::pair<char,char> play(Pop p1, Pop p2, bool print_turns);
        std::pair<char,char> play(Pop ai, bool print_turns);
        char won();

};

void play_game (std::vector<Pop>* population, unsigned p1, unsigned p2);

#endif
