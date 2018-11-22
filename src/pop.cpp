#include "pop.h"

#include <thread>
#include <cstdlib>
#include <iostream>

// Initializes the weights with random values.
Weight::Weight (void) {

    // Used by loops.
    int i, j;

    for(i = 0; i < 4; i++)
        for(j = 0; j < 3; j++)
            wght[i][j] = ((float)(rand()%100001))/100000.0;

}

Weight::Weight (Weight w1, Weight w2) {

    // Used by loops.
    int i, j;

    int rand_factor;
    for(i = 0; i < 4; i++)
        for(j = 0; j < 3; j++) {

            if(rand()%2)
                wght[i][j] = w1.wght[i][j];
            else
                wght[i][j] = w1.wght[i][j];
        
        }    

}


Weight::Weight(FILE* file) {

    for(int i = 0; i < 4; i++)
        fscanf(file, " %f, %f, %f", &wght[i][0], &wght[i][1], &wght[i][2]);

}

void Weight::mutate(unsigned mutation_chance, float max_mutation) {

    // Used by loops.
    int i, j;

    for(i = 0; i < 4; i++)
        for(j = 0; j < 3; j++)
            if(rand()%101 < mutation_chance) {
                wght[i][j] += max_mutation * 2 * ((float)(rand()%100001)/100000.0 - 0.5);
                if(wght[i][j] > 1)
                    wght[i][j] = 1;
                else if(wght[i][j] < 0)
                    wght[i][j] = 0;
            }

}

// Initializes a pop by assigning random weights to it.
Pop::Pop (void) {

    fit = 0;

    victories = 0;
    draws = 0;
    defeats = 0;
    turns = 0;

    weight = Weight();

}

Pop::Pop(Pop p1, Pop p2) {

    fit = 0;

    victories = 0;
    draws = 0;
    defeats = 0;
    turns = 0;

    weight = Weight(p1.weight, p2.weight);

}

Pop::Pop(char* filename) {

    FILE* file = fopen(filename, "r");

    char temp = '#';
    while(temp != '\n')
        temp = fgetc(file);

    weight = Weight(file);

    fclose(file);

}

// Assigns a id during a game.  
void Pop::assign_id(char i) { id = i; }

// Decide wich place to mark.
char Pop::decide(char game_places[3][3]) {

    // Used by loops.
    int i, j;

    // Calculate the weight of each place.
    char game_weights[3][3];
    
    //(0,0) =================================================
    game_weights[0][0] = (weight.wght[0][game_places[0][0]] 
                       + weight.wght[2][game_places[0][1]]
                       + weight.wght[0][game_places[0][2]]
                       + weight.wght[2][game_places[1][0]]
                       + weight.wght[1][game_places[1][1]]
                       //+ weight.wght[][game_places[1][2]]
                       + weight.wght[0][game_places[2][0]]
                       //+ weight.wght[][game_places[2][1]]
                       + weight.wght[1][game_places[2][2]]) * (9.0/7.0);

    //(0,1) =================================================
    game_weights[0][1] = (weight.wght[0][game_places[0][0]] 
                       + weight.wght[2][game_places[0][1]]
                       + weight.wght[0][game_places[0][2]]
                       //+ weight.wght[][game_places[1][0]]
                       + weight.wght[3][game_places[1][1]]
                       //+ weight.wght[][game_places[1][2]]
                       //+ weight.wght[][game_places[2][0]]
                       + weight.wght[3][game_places[2][1]]) * (9.0/5.0);
                       //+ weight.wght[][game_places[2][2]];

    //(0,2) =================================================
    game_weights[0][2] = (weight.wght[0][game_places[0][0]] 
                       + weight.wght[2][game_places[0][1]]
                       + weight.wght[0][game_places[0][2]]
                       //+ weight.wght[][game_places[1][0]]
                       + weight.wght[1][game_places[1][1]]
                       + weight.wght[2][game_places[1][2]]
                       + weight.wght[1][game_places[2][0]]
                       //+ weight.wght[][game_places[2][1]]
                       + weight.wght[0][game_places[2][2]]) * (9.0/7.0);

    //(1,0) =================================================
    game_weights[1][0] = (weight.wght[0][game_places[0][0]] 
                       //+ weight.wght[][game_places[0][1]]
                       + weight.wght[0][game_places[0][2]]
                       + weight.wght[2][game_places[1][0]]
                       + weight.wght[3][game_places[1][1]]
                       + weight.wght[3][game_places[1][2]]) * (9.0/5.0);
                       //+ weight.wght[][game_places[2][0]]
                       //+ weight.wght[][game_places[2][1]]
                       //+ weight.wght[][game_places[2][2]];

    //(1,1) =================================================
    if(weight.wght[4][game_places[1][1]] >= + weight.wght[5][game_places[1][1]])
        game_weights[1][1] = weight.wght[3][game_places[1][1]];
    else
        game_weights[1][1] = weight.wght[1][game_places[1][1]];

    game_weights[1][1] += weight.wght[1][game_places[0][0]] 
                       + weight.wght[3][game_places[0][1]]
                       + weight.wght[1][game_places[0][2]]
                       + weight.wght[3][game_places[1][0]]
                       //[1][1]
                       + weight.wght[3][game_places[1][2]]
                       + weight.wght[1][game_places[2][0]]
                       + weight.wght[3][game_places[2][1]]
                       + weight.wght[1][game_places[2][2]];

    //(1,2) =================================================
    game_weights[1][2] //= weight.wght[0][game_places[0][0]] 
                       //+ weight.wght[2][game_places[0][1]]
                       = (weight.wght[0][game_places[0][2]]
                       + weight.wght[3][game_places[1][0]]
                       + weight.wght[3][game_places[1][1]]
                       + weight.wght[2][game_places[1][2]]
                       //+ weight.wght[][game_places[2][0]]
                       //+ weight.wght[][game_places[2][1]]
                       + weight.wght[0][game_places[2][2]]) * (9.0/5.0);

    //(2,0) =================================================
    game_weights[2][0] = (weight.wght[0][game_places[0][0]] 
                       //+ weight.wght[][game_places[0][1]]
                       + weight.wght[1][game_places[0][2]]
                       + weight.wght[2][game_places[1][0]]
                       + weight.wght[1][game_places[1][1]]
                       //+ weight.wght[][game_places[1][2]]
                       + weight.wght[0][game_places[2][0]]
                       + weight.wght[2][game_places[2][1]]
                       + weight.wght[0][game_places[2][2]]) * (9.0/7.0);

    //(2,1) =================================================
    game_weights[2][1] //+ weight.wght[][game_places[0][0]] 
                       //+ weight.wght[][game_places[0][1]]
                       = (weight.wght[0][game_places[0][2]]
                       + weight.wght[3][game_places[1][0]]
                       + weight.wght[3][game_places[1][1]]
                       + weight.wght[2][game_places[1][2]]
                       //+ weight.wght[][game_places[2][0]]
                       //+ weight.wght[][game_places[2][1]]
                       + weight.wght[0][game_places[2][2]]) * (9.0/5.0);

    //(2,2) =================================================
    game_weights[2][2] = (weight.wght[1][game_places[0][0]] 
                       //+ weight.wght[][game_places[0][1]]
                       + weight.wght[0][game_places[0][2]]
                       //+ weight.wght[][game_places[1][0]]
                       + weight.wght[1][game_places[1][1]]
                       + weight.wght[2][game_places[1][2]]
                       + weight.wght[0][game_places[2][0]]
                       + weight.wght[2][game_places[2][1]]
                       + weight.wght[0][game_places[2][2]]) * (9.0/7.0);

    // Returns the place with the most weight.
    float max_weight = -0x3f3f3f3f;
    char max_place;
    for(i = 0; i < 3; i++)
        for(j = 0; j < 3; j++)
            if(game_places[i][j] == 0)
                if(game_weights[i][j] > max_weight) {
                    max_place = (i * 3) + j;
                    max_weight = game_weights[i][j];
                }

    return max_place;
}

void Pop::get_fit() {

    fit = (VIC_POINTS * victories) - (DEF_POINTS * defeats) + (DRW_POINTS * draws); 

}

void Pop::mutate(unsigned mutation_chance, float max_mutation) {

    weight.mutate(mutation_chance, max_mutation);

}

void Pop::save_data(unsigned gen, bool best_pop) {

    char filename[32];

    if(best_pop)
        sprintf(filename, "%u_b.csv", gen + 1);
    else
        sprintf(filename, "%u_w.csv", gen + 1);

    FILE* output = fopen(filename, "w+");

    fprintf(output, "\"My\",\"Opponent\",\"Empty\"\n");
    for(int i = 0; i < 4; i++)
        fprintf(output, "%f, %f, %f\n", weight.wght[i][0], weight.wght[i][1], weight.wght[i][2]);

    fprintf(output, "\"Victories\",\"Defeats\",\"Draws\",\"Total Turns\"\n");
    fprintf(output, "%u,%u,%u,%u\n", victories, defeats, draws, turns);

    fclose(output);
}

bool fit_sort (Pop p1, Pop p2) {

    if(p1.fit != p2.fit)
        return (p1.fit > p2.fit);
    else {
        if(p1.fit >= 0)
            return (p1.turns < p2.turns);
        else
            return (p1.turns > p2.turns);
    }
}

void crossover (std::vector<Pop>* population, unsigned p1, unsigned p2, unsigned target) {

    (*population)[target] = Pop((*population)[p1],(*population)[p2]);

}

void mutate (std::vector<Pop>* population, unsigned target, unsigned mutation_chance, float max_mutation) {

    (*population)[target].mutate(mutation_chance, max_mutation);

}