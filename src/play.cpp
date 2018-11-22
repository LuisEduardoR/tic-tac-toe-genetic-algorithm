#include "pop.h"
#include "game.h"

#include <thread>
#include <iostream>

#include <utility>

#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"

int main (void) {

    // Selects a option between human vs AI or AI vs AI.
    int selection;
    printf("%sSelect between human vs AI (1) or AI vs AI (2): %s", KCYN, KNRM);
    scanf(" %d", &selection);
    if(selection != 1 && selection != 2) {
        printf("%sInvalid option!\n%s", KRED, KNRM);
        return 0;
    }

    // Pops to be used later.
    Pop p1;
    Pop p2;

    // Name of the AI file.
    char ai_filename[128];
    if(selection == 1) {

        printf("%sEnter the name of the AI file: %s", KCYN, KNRM);
        scanf("%s", ai_filename);

        p1 = Pop(ai_filename);
        
        Game game;
        std::pair<char,char> game_info = game.play(p1, true);

        if(game_info.first == 1 || game_info.first == 2)
            printf("%s%d won!\n%s", KGRN, game_info.first, KNRM);
        else
            printf("%sDraw!\n%s", KGRN, KNRM);

    } else {

        printf("%sEnter the name of the first AI file: %s", KCYN, KNRM);
        scanf("%s", ai_filename);        
        
        p1 = Pop(ai_filename);

        printf("%sEnter the name of the second AI file: %s", KCYN, KNRM);
        scanf("%s", ai_filename);
        
        p2 = Pop(ai_filename);

        Game game;
        std::pair<char,char> game_info = game.play(p1, p2, true);

        if(game_info.first == 1 || game_info.first == 2)
            printf("%s%d won!\n%s", KGRN, game_info.first, KNRM);
        else
            printf("%sDraw!\n%s", KGRN, KNRM);
    }


    return 0;
}
