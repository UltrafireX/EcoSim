#ifndef MAIN_H_INCLUDED
#define MAIN_H_INCLUDED

#include <gtk/gtk.h>
#include "world.h"

#define GLADE "extensionFiles/glade/main.glade"
#define CSS "extensionFiles/glade/css/main.css"
#define GRASS_PICTURE "extensionFiles/glade/images/grass.png"
#define FOX_PICTURE "extensionFiles/glade/images/fox.png"
#define RABBIT_PICTURE "extensionFiles/glade/images/rabbit.png"
#define CARROT_PICTURE "extensionFiles/glade/images/carrot.png"
#define TREE_PICTURE "extensionFiles/glade/images/tree.png"

typedef struct game_data {
    int current;
    int rounds;
    GtkWidget *textview;
    GtkWidget *map;
    GtkWidget *start;
    GtkWidget *pause;
    board_t *board;
    gint ID;
} game_data_t;

void add_css(void);
bool init_grid(game_data_t *data);
void update_grid(GtkWidget *grd_map, board_t *board,bool start);
void add_text(GtkWidget *textview,textlog_t textlog);
void start(void);
char *get_text(GtkTextView *text_view);

#endif