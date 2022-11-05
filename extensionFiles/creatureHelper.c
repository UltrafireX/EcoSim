#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include <unistd.h>
#include "world.h"
#include "creatureOrder.h"
#include "helperFunctions.h"



/*
    Takes a type and an a pointer to a table
    Looks through table and returns the value associated with the type given
    If type not matched it returns -1
*/
int look_up(type_t key, t_symstruct *table) {
    for (int i = 0; i < TYPE_NUMBER; i++) {
        if (table[i].key == key) {
            return table[i].val;
        }
    }
    return -1;
}

/*
    Takes a pointer to a sqaure and a type
    Initialises a creature and places it in the given square
    Returns a pointer to the created creature
    Returned pointer must be freed
*/
creature_t *create_creature(square_t *square, type_t type) {
    creature_t *new_creature = malloc(sizeof(creature_t));

    assert(new_creature != NULL);

    new_creature->current_pos = square;
    new_creature->type = type;
    new_creature->age = 0;
    new_creature->eating = false;
    new_creature->reprod = false;
    new_creature->id = global_id++;
    
    new_creature->hunger = look_up(type, max_hunger_values);
    new_creature->speed = look_up(type, speed_values);
    new_creature->sight = look_up(type, sight_values);
    new_creature->fear = look_up(type, fear_values);
    new_creature->reprod_drive = look_up(type, max_libido_values);
    square->creature = new_creature;

    new_creature->next = NULL;

    return new_creature;
}

 /*
    Takes pointers to 2 creatures and a pointer to the board.
    It assumes all the conditions for mating are satisfied, then creates a creature in any available space around the parents
    If no spaces, no creature is produced.
    The parents have their reprod variable set back to false and their reprod_drive back up to max.
*/
void creature_reprod(creature_t *creature_papa, creature_t *creature_mama, board_t *board) {
    square_t *square = find_available_square(*creature_papa->current_pos, board, 1);
    if (!square) {
        square = find_available_square(*creature_mama->current_pos, board, 1);
    }
    if (square) {
        add_after_node(board->order, creature_mama, square, creature_mama->type);
        creature_papa->reprod_drive = look_up(creature_papa->type, max_libido_values);
        creature_mama->reprod_drive = look_up(creature_mama->type, max_libido_values);
        creature_papa->reprod = false;
        creature_mama->reprod = false;
    }
}

/*
    Takes pointers to 2 creatures and a board
    Resets the creature's hunger and deletes the target
*/
void creature_eat(creature_t *creature, creature_t *target, board_t *board) {
    creature->hunger = look_up(creature->type, max_hunger_values);
    delete_node(board->order, target);
}

/*
    Takes pointers to a creature, a board and a boolean
    Plants either do nothing or they produce offspring and die
*/
void plant_act(creature_t *creature, board_t *board, bool *dead) {
    assert(creature);
    
    int looked_up = look_up(creature->type, max_age_values);
    int random_number = rand() % looked_up / 2;

    if (creature->age + random_number < looked_up) {
        creature->age++;
        return;
    }

    char text[LOG_LINE_LENGTH];
    sprintf(text, "Plant %d died\n", creature->id);
    add_to_textlog(&board->textlog, text);

    square_t *new_square;
    for (int i = 0; i < creature->reprod_drive; i++) {
        new_square = random_square(*creature->current_pos, board, look_up(creature->type, sight_values));
        if (new_square) {
            add_after_node(board->order, creature, new_square, creature->type);
        }
    }
    
    delete_node(board->order, creature);
    *dead = true;
}