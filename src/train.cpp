#include "pop.h"
#include "game.h"

#include <cmath>

#include <chrono>
#include <thread>
#include <cstdlib>
#include <iostream>

#include <vector>
#include <utility>
#include <algorithm>

#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"

int main (void) {

    // Configurations to be used.
    unsigned pop_count;
    unsigned number_of_gens;
    char record_fit_info;
    unsigned record_frequency;
    unsigned save_frequency;
    unsigned mutation_chance;
    float max_mutation;

    // Receives the desired population.
    printf("%sEnter population amount:%s ", KCYN, KNRM);
    scanf(" %u", &pop_count);
    if(pop_count < 2) {
        printf("%sCan't proceed with less than 2 pops!%s\n", KRED, KNRM);
        return 0;
    }

    // Receives the desired number of generations.
    printf("%sEnter the number of generations:%s ", KCYN, KNRM);
    scanf(" %u", &number_of_gens);
    if(number_of_gens == 0) {
        printf("%sAt least 1 generation is necessary!%s\n", KRED, KNRM);
        return 0;
    }

    // Receives the mutation chance.
    printf("%sEnter the mutation chance (0 to 100 per cent):%s ", KCYN, KNRM);
    scanf(" %d", &mutation_chance);

    // Receives the max change of mutation.
    printf("%sEnter the maximum change mutation can cause:%s ", KCYN, KNRM);
    scanf(" %f", &max_mutation);

    // Receives if information about the fit of each generation should be recorded.
    printf("%sDo you want generation fit info to be recorded (Y/N)?%s ", KCYN, KNRM);
    scanf(" %c", &record_fit_info);

    // Receives the gap between recordings.
    if(record_fit_info == 'Y' || record_fit_info == 'y') {
        printf("%sEnter the generation gap between recordings:%s ", KCYN, KNRM);
        scanf(" %u", &record_frequency);
        if(record_frequency == 0) {
            printf("%sRecord frequency must be equal or greater than 1!%s\n", KRED, KNRM);
            return 0;
        }
    }

    // Receives the gap between saves of the best AI.
    printf("%sEnter the generation gap between saves of the best AI:%s ", KCYN, KNRM);
    scanf(" %u", &save_frequency);
    if(save_frequency == 0) {
        printf("%sSave frequency must be equal or greater than 1!%s\n", KRED, KNRM);
        return 0;
    }

    // Used do iterate in most loops.
    int i, j;

    // Seed the random number generation.
    srand(time(NULL));

    // Creates the population.
    printf("%sInitializing population...\n", KYEL);
    std::vector<Pop> population;
    for(i = 0; i < pop_count; i++) {
        Pop new_pop = Pop();
        population.push_back(new_pop);
    }

    // If selected, creates a file to store fit output.
    FILE* output_file = NULL;
    if(record_fit_info == 'Y' || record_fit_info == 'y'){
        printf("Creating fit output file...\n");
        output_file = fopen("output.csv", "w+");
        if(output_file == NULL) {
            fprintf(stderr, "%sError creating output file!%s\n", KRED, KNRM);
            return 1;
        }
    }

    // Manages the threads of the application.
    std::vector<std::thread> thread_manager;

    // Used to calculate info about a generation.
    unsigned min_vic, max_vic;
    unsigned min_def, max_def;
    unsigned min_drw, max_drw;
    unsigned min_tur, max_tur;
    float average_vd, average_drw, average_tur;

    // Saves the initial time.
    std::chrono::steady_clock::time_point t_start = std::chrono::steady_clock::now();
    // Used to store the current time.
    std::chrono::steady_clock::time_point t_now;
    // Used to store the elapsed running time.
    std::chrono::duration<double> time_span;

    // Used to print a progress message approximately every minute.
    double time_watcher = 60;

    // Runs the generations.
    printf("Running generations...\n");
    for(int gen = 0; gen < number_of_gens; gen++) {

        // Gets the current time.
        t_now = std::chrono::steady_clock::now();
        // Calculates the elapsed run time.
        time_span = std::chrono::duration_cast<std::chrono::duration<double>>(t_now - t_start);
        // Prints a message approximately every minute about the current generation and
        // elapsed time.
        if(time_span.count() > time_watcher) {

            if(time_span.count() < 60)
                printf("Currently on generation %u. Time elapsed: %.2lf seconds...\n",gen, time_span.count());
            else if(time_span.count() >= 60 && time_span.count() < 3600)
                printf("Currently on generation %u. Time elapsed: %.2lf minutes...\n",gen, time_span.count() / 60.0);
            else if(time_span.count() >= 3600)
                printf("Currently on generation %u. Time elapsed: %.2lf hours...\n",gen, time_span.count() / 3600.0);
        
            time_watcher += 60;

        }

        // Resets the fit of the population.
        for(i = 0; i < pop_count; i++) {
            population[i].victories = 0;
            population[i].draws = 0;
            population[i].defeats = 0;
            population[i].turns = 0;
        }

        // Plays the games of a generation.
        for(i = 0; i < pop_count; i++)
            for(j = i+1; j < pop_count; j++) { // Creates a separated thread for each match.
                // Plays a game with each one starting first.
                thread_manager.push_back(std::thread(play_game, &population, i, j));
                thread_manager.push_back(std::thread(play_game, &population, j, i));
            }

        // Wait for all games to finish.
        for(i = 0; i < thread_manager.size(); i++)
            thread_manager[i].join();
        // Clear the threads.
        thread_manager.clear();

        // Writes generation data to the output file.        
        if(record_fit_info == 'Y' || record_fit_info == 'y'){

            if(gen == 0) {
                fprintf(output_file, "\"Generation\",");
                fprintf(output_file, "\"Min Victories\",\"Max Victories\",");
                fprintf(output_file, "\"Min Defeats\",\"Max Defeats\",\"Average Victories/Defeats\",");
                fprintf(output_file, "\"Min Draws\",\"Max Draws\",\"Average Draws\",");
                fprintf(output_file, "\"Min Total Turns\",\"Max Total Turns\",\"Average Total Turns\"\n");
            }

            if(gen % record_frequency == 0) {

                min_vic = 0x3f3f3f3f;
                max_vic = 0;
                min_def = 0x3f3f3f3f;
                max_def = 0;
                average_vd = 0;

                min_drw = 0x3f3f3f3f;
                max_drw = 0;
                average_drw = 0;

                min_tur = 0x3f3f3f3f;
                max_tur = 0;
                average_tur = 0;

                for(i = 0; i < pop_count; i++) {

                    // Get info about victories.
                    if(population[i].victories < min_vic)
                        min_vic = population[i].victories;
                    if(population[i].victories > max_vic)
                        max_vic = population[i].victories;

                    // Get info about defeats.
                    if(population[i].defeats < min_def)
                        min_def = population[i].defeats;
                    if(population[i].defeats > max_def)
                        max_def = population[i].defeats;

                    average_vd += population[i].victories; // The average between victories and
                                                           // defeats is always the same.

                    // Get info about draws.
                    if(population[i].draws < min_drw)
                        min_drw = population[i].draws;
                    if(population[i].draws > max_drw)
                        max_drw = population[i].draws;
                    average_drw += population[i].draws;

                    // Get info about the total turns.
                    if(population[i].turns < min_tur)
                        min_tur = population[i].turns;
                    if(population[i].turns > max_tur)
                        max_tur = population[i].turns;
                    average_tur += population[i].turns;

                }

                // Calculate the averages.
                average_vd /= pop_count;
                average_drw /= pop_count;
                average_tur /= pop_count;

                // Prints info to file.
                fprintf(output_file, "%u, ", gen);
                fprintf(output_file, "%u, %u,", min_vic, max_vic);
                fprintf(output_file, "%u, %u, %.2f,", min_def, max_def, average_vd);
                fprintf(output_file, "%u, %u, %.2f,", min_drw, max_drw, average_drw);
                fprintf(output_file, "%u, %u, %.2f\n", min_tur, max_tur, average_tur);
            
            }
        }

        // Gets the fit of all pops.
        for(i = 0; i < pop_count; i++)
            population[i].get_fit();

        // Sorts the vector based on fit.
        std::sort(population.begin(), population.end(), &fit_sort);
        
        // Saves the weights of the best pop.
        if(gen % save_frequency == 0 || gen == number_of_gens - 1) {
            printf("%sSaving best AI of generation %u...\n", KBLU, gen + 1);
            population[0].save_data(gen, true);
            printf("Done saving AI...%s\n", KYEL);
        }
        if(gen % save_frequency == 0 || gen == number_of_gens - 1) {
            printf("%sSaving worst AI of generation %u...\n", KBLU, gen + 1);
            population[pop_count - 1].save_data(gen, false);
            printf("Done saving AI...%s\n", KYEL);
        }

        // Crossover the best pop with the best half of the population and assigns the new pops
        // the worst quarter.
        for(i = 0; i < pop_count / 2; i+=2) // Creates a new thread for each crossover.
            thread_manager.push_back(std::thread(&crossover, &population, i, i+1, (pop_count -1) - (i/2)));

        // Waits for the crossover to finish.
        for(i = 0; i < thread_manager.size(); i++)
            thread_manager[i].join();
        // Clears the threads.
        thread_manager.clear(); 
        
        // Mutates the rest of the population that is not from the crossover or the very best pop.
        for(i = 1; i < 3 * pop_count / 4; i++)
            thread_manager.push_back(std::thread(&mutate, &population, i, mutation_chance, max_mutation));

        // Waits for the mutation to finish.
        for(i = 0; i < thread_manager.size(); i++)
            thread_manager[i].join();
        // Clear the threads.
        thread_manager.clear();

    }

    // Closes the output file. 
    if(record_fit_info == 'Y' || record_fit_info == 'y')
        fclose(output_file);

    // Calculates the elapsed run time and prints it.
    t_now = std::chrono::steady_clock::now();
    time_span = std::chrono::duration_cast<std::chrono::duration<double>>(t_now - t_start);
    printf("%s", KGRN);
    if(time_span.count() < 60)
        printf("Completed after %.2lf seconds...\n", time_span.count());
    else if(time_span.count() >= 60 && time_span.count() < 3600)
        printf("Completed after %.2lf minutes...\n", time_span.count() / 60.0);
    else if(time_span.count() >= 3600)
        printf("Completed after %.2lf hours...\n", time_span.count() / 3600.0);
    printf("%s", KNRM);

    return 0;
}
