#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
#include "fileRead.h"
#include "world.h"




// Initialises all creature tables with the creature types as keys.
void init_tables() {
    for (int i = 0; i < 4; i++) {
        number_values[i].key = i;
        max_age_values[i].key = i;
        max_hunger_values[i].key = i;
        max_libido_values[i].key = i;
        fear_values[i].key = i;
        speed_values[i].key = i;
        sight_values[i].key = i; 
    }
}

// Reads the input file and assigns global variables and table values based on the file.
void reads_file(char **argv) {
    FILE *ptr = fopen(argv[1], "r");
    if (!ptr) {
        perror("Error opening the file");
        exit(EXIT_FAILURE);
    }

    char str[100];

    int index = -1;
    int number = 5;
    int max_age = 10;
    int max_hunger = 10;
    int max_libido = 5;
    int fear = 100;
    int speed = 1;
    int sight = 5;

    
    // Takes in each line and looks for certain lables, and reads the numbers that follow them.
    while(fgets(str, 100, ptr)) {
        if (index == 4) {
            if (!strncmp(str + 1, "Seed:", 5)) {
                    seed = atoi(str + 6);

            } else if (!strncmp(str + 1, "Size:", 5)) {
                    board_size = atoi(str + 6);

            } else if (!strncmp(str + 1, "Rounds:", 7)) {  
                    num_of_round = atoi(str + 8);

            }

        } else if (index != -1) {
            if (!strncmp(str + 1, "Number:", 7)) {
                    number = atoi(str + 8);

            } else if (!strncmp(str + 1, "Max Age:", 8)) {  
                    max_age = atoi(str + 9);
 
            } else if (!strncmp(str + 1, "Max Hunger:", 11)) {
                    max_hunger = atoi(str + 12);
                    
            } else if (!strncmp(str + 1, "Max Libido:", 11)) {
                    max_libido = atoi(str + 12);
                   
            } else if (!strncmp(str + 1, "Fear:", 5)) {
                    fear = atoi(str + 6);
                                    
            } else if (!strncmp(str + 1, "Speed:", 6)) {                
                    speed = atoi(str + 7);
                    
            } else if (!strncmp(str + 1, "Sight:", 6)) {               
                    sight = atoi(str + 7);
                   
            } 
        }
        
        if (!strcmp(str, "Board:\n")) {
            index = 4;
        } else if (!strcmp(str, "Fox:\n")) {
            index = 0;
        } else if (!strcmp(str, "Rabbit:\n")) {
            index = 1;
        } else if (!strcmp(str, "Carrot:\n")) {
            index = 2;
        } else if (!strcmp(str, "Tree:\n")) {
            index = 3;
        }

        // Places the values in the tables when a new line is found, based on the previous label.
        if (!strcmp(str, "\n") && index != 4 && index != -1) {
            number_values[index].val = number;
            max_age_values[index].val = max_age;
            max_hunger_values[index].val = max_hunger;
            max_libido_values[index].val = max_libido;
            fear_values[index].val = fear;
            speed_values[index].val = speed;
            sight_values[index].val = sight; 
            number = 5;
            max_age = 10;
            max_hunger = 10;
            max_libido = 5;
            fear = 100;
            speed = 1;
            sight = 5;
        }
        if (!strcmp(str, "\n")) {
            index = -1;
        }
    }
    fclose(ptr);
}

