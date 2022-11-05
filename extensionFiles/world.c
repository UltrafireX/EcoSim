#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
#include "world.h"
#include "creatureHelper.h"
#include "creatureOrder.h"
#include "rabbit.h"
#include "fox.h"


int global_id;

int seed;
int board_size;
int num_of_round;

t_symstruct number_values[4];
t_symstruct max_age_values[4];
t_symstruct max_hunger_values[4];
t_symstruct max_libido_values[4];
t_symstruct fear_values[4];
t_symstruct speed_values[4];
t_symstruct sight_values[4];

void init_id(void) {
    global_id = 0;
}

/*
    Initialises board
    Returns a pointer to the malloc'ed board
    Returned pointer must be freed
*/
board_t *create_board(void) {
    board_t *new_board = malloc(sizeof(board_t));
    new_board->squares = malloc(board_size * sizeof(square_t *));
    
    for (int i = 0; i < board_size; i++) {
        new_board->squares[i] = malloc(board_size * sizeof(square_t));
        for (int j = 0; j < board_size; j++) {
            square_t new_square;
            new_square.position.x = j;
            new_square.position.y = i;
            new_board->squares[i][j] = new_square;
        }
    }

    textlog_t new_textlog;
    new_textlog.current_size = 0;
    new_textlog.texts = calloc(new_textlog.current_size, sizeof(char *));
    new_board->textlog = new_textlog;

    creature_order_t *new_order = malloc(sizeof(creature_order_t));
    initialise_order(new_order);
    new_board->order = new_order;
    return new_board;
}

/*
    Takes a pointer to a board
    Frees all of board
*/
void free_all(board_t *board) {
    for (int i = 0; i < board_size; i++) {
        for (int j = 0; j < board_size; j++) {
            if (board->squares[i][j].creature) {
                delete_node(board->order, board->squares[i][j].creature);
            }
        }
        free(board->squares[i]);
    }
    free(board->squares);
    for (int i = 0; i < board->textlog.current_size; i++) {
        free(board->textlog.texts[i]);
    }
    free(board->textlog.texts);
    free(board->order);
    free(board);
}

/*
    Takes a pointer to a textlog and a string
    Realloc's the textlog's contents and adds the string to them
*/
void add_to_textlog(textlog_t *textlog, char *text) {
    textlog->current_size++;
    textlog->texts = realloc(textlog->texts, textlog->current_size * sizeof(char *));
    textlog->texts[textlog->current_size - 1] = malloc(strlen(text) + 1);
    strcpy(textlog->texts[textlog->current_size - 1], text);
}

void print_textlog(textlog_t *textlog) {
    for (int i = 0; i < textlog->current_size; i++) {
        printf("%s\n", textlog->texts[i]); 
    } 
}

void print_board(board_t *board, int round_number) {
    printf("Round: %d\n\n", round_number);
    for (int i = 0; i < board_size; i++) {
        for (int j = 0; j < board_size; j++) {
            if (board->squares[i][j].creature) {
                switch (board->squares[i][j].creature->type) {
                    case FOX:
                        printf("f%d\t", board->squares[i][j].creature->id);
                        break;

                    case RABBIT:
                        printf("r%d\t", board->squares[i][j].creature->id);
                        break;

                    case CARROT:
                        printf("c%d\t", board->squares[i][j].creature->id);
                        break;

                    case TREE:
                        printf("t%d\t", board->squares[i][j].creature->id);
                        break;

                    default:
                        break;
                }

            } else {
                printf("..\t");
            }
        }
        printf("\n");
    }
    printf("\n\n");
}

void world(board_t *board, int rounds) {
    char text[LOG_LINE_LENGTH];
    creature_t *prev_node = NULL;
    creature_t *current_node = board->order->front;
    bool dead;
    
    sprintf(text, "Round %d\n", rounds+1);
    add_to_textlog(&board->textlog, text);

    while (current_node) {
        dead = false;
        switch (current_node->type) {
            case FOX:
                fox_act(current_node, board, &dead);
                break;

            case RABBIT:
                rabbit_act(current_node, board, &dead);
                break;

            default:
                plant_act(current_node, board, &dead);
                break;
        }
        if (!board->order->front) {
            current_node = NULL;
        } else if (dead && !prev_node) {
            current_node = board->order->front;
        } else if (dead) {
            current_node = prev_node->next;
        } else {
            prev_node = current_node;
            current_node = current_node->next;
        }
    }
    //print_textlog(&board->textlog);
    // sleep(1);
}

