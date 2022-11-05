#ifndef FOX_H_INCLUDED
#define FOX_H_INCLUDED

creature_t *fox_find_priority(creature_t creature, board_t board);

void fox_act(creature_t *creature, board_t *board, bool *dead);

void fox_move(creature_t *creature, creature_t *target, board_t *board);

#endif