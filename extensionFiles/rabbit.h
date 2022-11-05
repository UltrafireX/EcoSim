#ifndef RABBIT_H_INCLUDED
#define RABBIT_H_INCLUDED

creature_t *rabbit_find_priority(creature_t creature, board_t board);

void rabbit_act(creature_t *creature, board_t *board, bool *dead);

void rabbit_move(creature_t *creature, creature_t *target, board_t *board);


#endif