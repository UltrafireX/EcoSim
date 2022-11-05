#ifndef HELPER_FUNCTIONS_H_INCLUDED
#define HELPER_FUNCTIONS_H_INCLUDED


bool is_available(square_t square);

bool is_adjacent(square_t current_square, square_t target_square);

void random_spawn(int number, type_t type, board_t *board);


bool checker(int x, int y, board_t board);

void random_move(int *curr_x, int *curr_y, board_t board);

square_t *find_available_square(square_t current_square, board_t *board, int range);

square_t *random_square(square_t current_square, board_t *board, int range);

creature_t *find_seen_creature(square_t current_square, type_t type, board_t board, int range);


#endif