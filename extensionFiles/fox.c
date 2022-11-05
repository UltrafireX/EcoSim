#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include "world.h"
#include "helperFunctions.h"
#include "creatureHelper.h"
#include "creatureOrder.h"
#include "fox.h"

/*
    Takes a creature struct and a board struct
    Finds the creature that is most important to the fox's survival in its range of sight
    Returns a pointer to that creature
*/
creature_t *fox_find_priority(creature_t creature, board_t board) {
    creature_t *seen_creature = NULL;
    type_t fst = FOX;
    type_t snd = RABBIT;

    if (creature.hunger <= creature.reprod_drive) {
        fst = RABBIT;
        snd = FOX;
    }
    
    seen_creature = find_seen_creature(*creature.current_pos, fst, board, creature.sight);
    if (!seen_creature) {
        seen_creature = find_seen_creature(*creature.current_pos, snd, board, creature.sight);
    }
    return seen_creature;
}

/*
    Takes pointers to 2 creatures and a board
    Moves the fox towards the target creature
    If there is no target creature, the fox moves randomly
*/
void fox_move(creature_t *creature, creature_t *target, board_t *board) {
    int curr_x = creature->current_pos->position.x;
    int curr_y = creature->current_pos->position.y;
    if (target) { // Target may be NULL if nothing is around
        int *curr_x_pointer = &curr_x;
        int *curr_y_pointer = &curr_y;
        switch (target->type) {
            case CARROT: ;
                random_move(curr_x_pointer, curr_y_pointer, *board);
                break;

            // Default accounts for both rabbits and potential mates, since they should behave identically
            default: ;
                if (curr_x > target->current_pos->position.x) { 
                    if (checker(curr_x - 1, curr_y, *board)) {
                        curr_x--;
                        break;
                    }
                } 
                if (curr_x < target->current_pos->position.x) {
                    if (checker(curr_x + 1, curr_y, *board)) {
                        curr_x++;
                        break;
                    }
                }
                if (curr_y > target->current_pos->position.y) {
                    if (checker(curr_x, curr_y - 1, *board)) {
                        curr_y--;
                        break;
                    }
                }
                if (curr_y < target->current_pos->position.y) {
                    if (checker(curr_x, curr_y + 1, *board)) {
                        curr_y++;
                        break;
                    }
                }  
                random_move(curr_x_pointer, curr_y_pointer, *board);
                break;
        } 
    } else {
        int *curr_x_pointer = &curr_x;
        int *curr_y_pointer = &curr_y;
        random_move(curr_x_pointer, curr_y_pointer, *board);
    }
    // one of these needs to go
    creature->current_pos->creature = NULL;
    board->squares[creature->current_pos->position.y][creature->current_pos->position.x].creature = NULL;

    // Adds the this action to the textlog
    char text[LOG_LINE_LENGTH];
    sprintf(
        text, 
        "Fox %d moves from (%d, %d) to (%d, %d)\n", 
        creature->id, 
        creature->current_pos->position.x, 
        creature->current_pos->position.y, 
        curr_y, 
        curr_x
    );
    add_to_textlog(&board->textlog, text);

    creature->current_pos = &board->squares[curr_y][curr_x];
    creature->current_pos->creature = creature;

}

/*
    Takes pointers to a creature, a board and a boolean
    Based on the creatures needs and relative position, calls the relevant function
    If the creature dies it sets the boolean to true
*/
void fox_act(creature_t *creature, board_t *board, bool *dead) {
    assert(creature);
    
    char text[LOG_LINE_LENGTH];
    for (int i = 0; i < creature->speed; i++) {
        creature->reprod = false;
        creature->eating = false;
        creature_t *priority_creature = fox_find_priority(*creature, *board);
        if (priority_creature) {
            switch (priority_creature->type) {
                case FOX:
                    if (is_adjacent(*creature->current_pos, *priority_creature->current_pos)) {
                        if (creature->reprod_drive <= look_up(FOX, max_age_values) - look_up(FOX, max_libido_values)) {
                            creature->reprod = true;
                        }
                    }
                    break;
                
                case RABBIT: 
                    if (is_adjacent(*creature->current_pos, *priority_creature->current_pos)) {
                            if (creature->hunger != look_up(FOX, max_hunger_values)) {
                                creature->eating = true;
                            }
                        }
                    break;

                default:
                    perror("ERROR fox_act: Priority Type not recognized\n");
                    break;
            }
        }
        
        if (creature->eating) {
            sprintf(text, "Fox %d eats rabbit %d\n", creature->id, priority_creature->id);
            add_to_textlog(&board->textlog, text);
            creature_eat(creature, priority_creature, board);
        } else if (creature->reprod) {
            if (priority_creature->reprod) {
                sprintf(text, "Fox %d mates with rabbit %d\n", creature->id, priority_creature->id);
                add_to_textlog(&board->textlog, text);
                creature_reprod(creature, priority_creature, board);
            }
        } else {
            fox_move(creature, priority_creature, board);
        }
        if (creature->hunger <= 0) {
            sprintf(text, "Fox %d starves\n", creature->id);
            add_to_textlog(&board->textlog, text);
            delete_node(board->order, creature);
            *dead = true;
            return;
        }
        if (creature->age > look_up(FOX, max_age_values)) {
            sprintf(text, "Fox %d dies of old age\n", creature->id);
            add_to_textlog(&board->textlog, text);
            delete_node(board->order, creature);
            *dead = true;
            return;
        }
        
    }
    if (creature->hunger > 0 && !creature->eating) {
        creature->hunger--;
    }
    if (creature->reprod_drive > 3 && !creature->reprod) {
        creature->reprod_drive--;
    }
    creature->age++;
}