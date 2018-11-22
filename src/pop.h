#ifndef POP_H
#define POP_H

#include <vector>
#include <iostream>

#define VIC_POINTS 125
#define DEF_POINTS 125
#define DRW_POINTS 100

class Weight {

    public:
        // 0 = +--, 1 = \  , 2 = |  , 3 =    
        //     |         \       |        ---
        //     |          \      |
        float wght[4][3];

        Weight(void);
        Weight(Weight w1, Weight w2);
        Weight(FILE* file);
        void mutate(unsigned mutation_chance, float max_mutation);

};

class Pop {
    
    private:
        char id;
    public:

        int fit;

        unsigned victories;
        unsigned draws;
        unsigned defeats;
        unsigned turns;
        
        Weight weight;

        Pop();
        Pop(Pop p1, Pop p2);
        Pop(char* filename);

        char decide(char game_places[3][3]);
        void assign_id(char i);
        void get_fit();
        void mutate(unsigned mutation_chance, float max_mutation);
        void save_data(unsigned gen, bool best_pop);

};

bool fit_sort (Pop p1, Pop p2);
void crossover (std::vector<Pop>* population, unsigned p1, unsigned p2, unsigned target);
void mutate (std::vector<Pop>* population, unsigned target, unsigned mutation_chance, float max_mutation);

#endif