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


bool is_available(square_t square) {
    return square.creature == NULL;
}

bool is_adjacent(square_t current_square, square_t target_square) {
    return ((abs(current_square.position.x - target_square.position.x) <= 1) 
        && (abs(current_square.position.y - target_square.position.y) <= 1));
}

/*
    Takes 2 ints and a board
    If the ints (representing x, y coordinates) point to an available square within the board it returns true
    Otherwise returns false
*/
bool checker(int x, int y, board_t board) {
    if (x >= 0 && x < board_size && y >= 0 && y < board_size) {
        if (is_available(board.squares[y][x])) {
            return true;
        }
    }
    return false;
}

/*
    Takes an int a type and a pointer to a board
    Places a set number of creature of the set type randomly on the board
*/
void random_spawn(int number, type_t type, board_t *board) {
    for (int i = 0; i < number; i++) {
        int x = rand() % board_size;
        int y = rand() % board_size;
        while (!checker(x, y, *board)) {
            x = rand() % board_size;
            y = rand() % board_size;
        }
        add_to_rear(board->order, &board->squares[y][x], type);
    }
}

/*
    Takes pointers to 2 ints, along with a board
    Changes one of ints randomly by either decrementing, incrementing or doing nothing
*/
void random_move(int *curr_x, int *curr_y, board_t board) {
    int random_x = rand() % 3 - 1;
    int random_y = rand() % 3 - 1;
    int breaker = 0;
    while ((!checker((*curr_x + random_x), *curr_y, board)) && breaker < 5) {
        random_x = rand() % 3 - 1;
        breaker++;
    }
    breaker = 0;
    while ((!checker(*curr_x, (*curr_y + random_y), board)) && breaker < 5) {
        random_y = rand() % 3 - 1;
        breaker++;
    }

    if (!checker(*curr_x + random_x, *curr_y, board)) {
        random_x = 0;
    }

    if (!checker(*curr_x, *curr_y + random_y, board)) {
        random_y = 0;
    }

    int random_select = rand() % 2;
    if (random_select == 0) {
        *curr_x += random_x;
    } else {
        *curr_y += random_y;
    }
}

/*
    Takes a square, a pointer to a board and an int
    Finds the nearest available square to the given on in the range given
    Returns a pointer to the square found
    If no square is found, a NULL is returned
*/
square_t *find_available_square(square_t current_square, board_t *board, int range) {
    // Finds an available square within the given range from the current_square
    int current_x = current_square.position.x;
    int current_y = current_square.position.y;

    int x, y, dx, dy;
    x = 0;
    y = 0;
    dx = 0;
    dy = -1;
    int t = range * 2;
    int maxI = t*t;
    for(int i = 1; i < maxI; i++){
        if ((-range <= x) && (x <= range) && (-range <= y) && (y <= range)){
            if (current_x + x >= 0 && current_x + x < board_size && current_y + y >= 0 && current_y + y < board_size) {
                if (!board->squares[current_y + y][current_x + x].creature) {
                    if (!(x == 0 && y == 0)) {
                        return &board->squares[current_y + y][current_x + x];
                    }
                }
            }
        }
        if ((x == y) || ((x < 0) && (x == -y)) || ((x > 0) && (x == 1 - y))){
            t = dx;
            dx = -dy;
            dy = t;
        }
        x += dx;
        y += dy;
    }

    return NULL;
}

/*
    Takes a square, a pointer to a board and an int
    Finds a random available square within range of the given square
    Returns a pointer to that square
    If no square is found, a NULL is returned
*/
square_t *random_square(square_t current_square, board_t *board, int range) {
    int random_number = rand() % ((2 * range + 1) * (2 * range + 1)) ;
    square_t *back_up = NULL;
    int counter = 0;
    for (int i = current_square.position.x - range; i < current_square.position.x + range; i++) {
        for (int j = current_square.position.y - range; j < current_square.position.y + range; j++) {
            if (counter < random_number) {
                if (checker(i, j, *board)) {
                    back_up = &board->squares[j][i];
                }
            } else {
                if (checker(i, j, *board) ) {
                    return &board->squares[j][i];
                }
            }
            counter++;
        }
    }
    return back_up;
}

/*
    Takes a current square, a type, a board and an int
    Finds the nearest creature of the specified type within the given range of the given square
    Returns a pointer to that creature
    If no creature is found, a NULL is returned
*/
creature_t *find_seen_creature(square_t current_square, type_t type, board_t board, int range) {
    int current_x = current_square.position.x;
    int current_y = current_square.position.y;
    int x, y, dx, dy;
    x = 0;
    y = 0;
    dx = 0;
    dy = -1;
    int t = range * 2 + 1;
    int maxI = t*t;
    for(int i = 1; i < maxI; i++){
        if ((-range <= x) && (x <= range) && (-range <= y) && (y <= range)){
            if (current_x + x >= 0 && current_x + x < board_size && current_y + y >= 0 && current_y + y < board_size) {
                if (board.squares[current_y + y][current_x + x].creature) {
                    if (board.squares[current_y + y][current_x + x].creature->type == type) {
                        if (!(x == 0 && y == 0)) {
                            return board.squares[current_y + y][current_x + x].creature;
                        }
                    }
                }
            }
        }
        if ((x == y) || ((x < 0) && (x == -y)) || ((x > 0) && (x == 1 - y))){
            t = dx;
            dx = -dy;
            dy = t;
        }
        x += dx;
        y += dy;
    }

    return NULL;
}