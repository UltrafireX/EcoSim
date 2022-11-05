#ifndef CREATURE_HELPER_H_INCLUDED
#define CREATURE_HELPER_H_INCLUDED


int look_up(type_t key, t_symstruct *table);


creature_t *create_creature(square_t *square, type_t type);


void creature_reprod(creature_t *creature_papa, creature_t *creature_mama, board_t *board);

void creature_eat(creature_t *creature, creature_t *target, board_t *board);


void plant_act(creature_t *creature, board_t *board, bool *dead);


#endif