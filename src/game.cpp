#include "game.h"

#include <iostream>

#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"

std::pair<char,char> Game::play(Pop p1, Pop p2, bool print_turns) {

    char turn = 1;
    char winner = 0;

    // Used by loops.
    char i, j;

    // Assign ids from pops.
    p1.assign_id(1);
    p2.assign_id(2);

    // Initializes the board.
    for(i = 0; i < 3; i++)
        for(j = 0; j < 3; j++)
            place[i][j] = 0;

    char decision;

    while(winner == 0) {

        if(print_turns) {
            printf("%s=====%s\n", KYEL, KNRM);
            for(i = 0; i < 3; i++) {
                for(j = 0; j < 3; j++)
                    printf("%d ", place[i][j]);
                putchar('\n');
            }
        }

        if(turn % 2 == 1) {

            decision = p1.decide(place);

            place[decision / 3][decision % 3] = 1;

            turn++;

        } else {  

            decision = p2.decide(place);

            place[decision / 3][decision % 3] = 2;

            turn++;

        }

        winner = won();

    }

    if(print_turns) {
            printf("%s=====%s\n", KYEL, KNRM);
            for(i = 0; i < 3; i++) {
                for(j = 0; j < 3; j++)
                    printf("%d ", place[i][j]);
                putchar('\n');
            }
            printf("%s=====%s\n", KYEL, KNRM);
        }

    return std::make_pair(winner, turn);

}

std::pair<char,char> Game::play(Pop ai, bool print_turns) {

    char turn = 1;
    char winner = 0;

    // Used by loops.
    char i, j;

    // Receives if the player wants to play first.
    printf("%sDo you want to play first (Y/N)?%s ", KCYN, KNRM);
    char play_first;
    scanf(" %c", &play_first);

    // Assign id to the AI.
    if(play_first == 'y' || play_first == 'Y')
        ai.assign_id(2);
    else
        ai.assign_id(1);

    // Initializes the board.
    for(i = 0; i < 3; i++)
        for(j = 0; j < 3; j++)
            place[i][j] = 0;

    char decision;
    int input;

    
    while(winner == 0) {

        if(turn % 2 == 1) {

            if(play_first == 'y' || play_first == 'Y') {
                input = -0x3f3f3f3f;
                while(input == -1 || input < 0 || input > 8 || place[input / 3][input % 3] != 0) {
                    if(input != -0x3f3f3f3f)
                        printf("%sInvalid Input!\n%s", KRED, KNRM);
                    if(print_turns) {
                        printf("%s=====%s\n", KYEL, KNRM);
                        for(i = 0; i < 3; i++) {
                            for(j = 0; j < 3; j++)
                                printf("%d ", place[i][j]);
                            putchar('\n');
                        }
                        printf("%s=====%s\n", KYEL, KNRM);
                    }
                    printf("%sInput place:%s ", KCYN, KNRM);
                    scanf(" %d", &input);
                }
                place[input / 3][input % 3] = 1;
            } else {
                decision = ai.decide(place);
                place[decision / 3][decision % 3] = 1;
            }

            turn++;

        } else {  

            if(play_first == 'y' || play_first == 'Y') {
                decision = ai.decide(place);
                place[decision / 3][decision % 3] = 2;
            } else {
                input = -0x3f3f3f3f;
                while(input == -1 || input < 0 || input > 8 || place[input / 3][input % 3] != 0) {
                    if(input != -0x3f3f3f3f)
                        printf("%sInvalid Input!\n%s", KRED, KNRM);
                    if(print_turns) {
                        printf("%s=====%s\n", KYEL, KNRM);
                        for(i = 0; i < 3; i++) {
                            for(j = 0; j < 3; j++)
                                printf("%d ", place[i][j]);
                            putchar('\n');
                        }
                        printf("%s=====%s\n", KYEL, KNRM);
                    }
                    printf("%sInput place:%s ", KCYN, KNRM);
                    scanf(" %d", &input);
                }
                place[input / 3][input % 3] = 2;
            }

            turn++;

        }

        winner = won();

    }

    if(print_turns) {
            printf("%s=====%s\n", KYEL, KNRM);
            for(i = 0; i < 3; i++) {
                for(j = 0; j < 3; j++)
                    printf("%d ", place[i][j]);
                putchar('\n');
            }
            printf("%s=====%s\n", KYEL, KNRM);
        }

    return std::make_pair(winner, turn);

}

char Game::won() {

    // Used by loops.
    char i, j;

    for(i = 0; i < 3; i++)
        if(place[i][0] != 0 && place[i][0] == place[i][1] && place[i][1] == place[i][2])
            return place[i][0];

    // Checks for lines.
    for(j = 0; j < 3; j++)
        if(place[0][j] != 0 && place[0][j] == place[1][j] && place[1][j] == place[2][j])
            return place[0][j];

    // Checks for the main diagonal.
    if(place[0][0] != 0 && place[0][0] == place[1][1] && place[1][1] == place[2][2])
        return place[0][0];

    // Checks for the reverse diagonal.
    if(place[2][0] != 0 && place[2][0] == place[1][1] && place[1][1] == place[0][2])
        return place[2][0];

    // Check if there are any empty places left and returns 0 if there is.
    for(i = 0; i < 3; i++)
        for(j = 0; j < 3; j++)
            if(place[i][j] == 0)
                return 0;

    // Returns -1 if there are no more empty places.
    return -1;

}

void play_game (std::vector<Pop>* population, unsigned p1, unsigned p2) {

    // Plays the game.
    std::pair<char,char> game_info;
    Game game;
    game_info = game.play((*population)[p1], (*population)[p2], false);

    // Adjust the fit of both pops after the game. 
    if(game_info.first == 1) {

        (*population)[p1].victories++;
        (*population)[p2].defeats++;

    } else if(game_info.first == 2) {
        
        (*population)[p1].defeats++;
        (*population)[p2].victories++;

    } else {

        (*population)[p1].draws++;
        (*population)[p2].draws++;

    }

    (*population)[p1].turns += game_info.second;
    (*population)[p2].turns += game_info.second;

}