#ifndef WORLD_H_INCLUDED
#define WORLD_H_INCLUDED

#define LOG_LINE_LENGTH 100
#define TYPE_NUMBER 4


typedef enum type{FOX, RABBIT, CARROT, TREE} type_t;

// Stores the x and y of a certain point on the board.
typedef struct position {
  int x;
  int y;
} position_t;


/* 
	Stores a pointer to a creature and its position on the board.
	If no creature is on the board the pointer will be null
*/
typedef struct square {
  position_t position;
  struct creature *creature;
} square_t;


/* 
	Stores the properties of a creature, as well as a pointer to the square it is currently on.
	Also acts as a node for the order, storing a pointer to the next creature in the order.
*/
typedef struct creature {

  //Creature dependatnt
  type_t type;
  int speed;

  int hunger;
  int reprod_drive;
  int age;
  
  bool eating;
  bool reprod;

  //Constants:
  int id;         
  int fear;
  int sight;  

  //Position:
  square_t *current_pos;

  struct creature *next;
} creature_t; 

// Stores pointers to the front and rear of the order.
typedef struct creature_order {
  creature_t *front;
  creature_t *rear;
} creature_order_t;

// Stores the actions creatures take each turn.
typedef struct textlog {
  char ** texts;
  int current_size;
} textlog_t;

/* 
	Stores a 2d array of squares, as well as the order and the textlog. 
	From this, most variables of the program can be accessed.
*/
typedef struct board {
  square_t **squares;
  creature_order_t *order;
  textlog_t textlog;
} board_t;


extern int global_id;

extern int seed;
extern int board_size;
extern int num_of_round;


void init_id(void);
board_t *create_board(void);
void free_all(board_t *board);
void print_board(board_t *board, int round_number);
void add_to_textlog(textlog_t *textlog, char *text);
void print_textlog(textlog_t *textlog);
void world(board_t *board, int rounds);

typedef struct { type_t key; int val; } t_symstruct;

extern t_symstruct number_values[];
extern t_symstruct max_age_values[];
extern t_symstruct max_hunger_values[];
extern t_symstruct max_libido_values[];
extern t_symstruct fear_values[];
extern t_symstruct speed_values[];
extern t_symstruct sight_values[];

#endif