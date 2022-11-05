#include <stdlib.h>
#include <stdbool.h>
#include <gtk/gtk.h>
#include "main.h"
#include "helperFunctions.h"
#include "creatureHelper.h"
#include "fileRead.h"

game_data_t *data;
int seed;

// Adds CSS to GUI
void add_css(void) {
    GtkCssProvider *css_provider = gtk_css_provider_new ();
    gtk_css_provider_load_from_path(css_provider, CSS, NULL);
    gtk_style_context_add_provider_for_screen(gdk_screen_get_default(), GTK_STYLE_PROVIDER(css_provider), GTK_STYLE_PROVIDER_PRIORITY_USER);
}

// Initialises grid with starting parameters
bool init_grid(game_data_t *data) {
    board_t *game_board = data->board;
    init_id();

    int total_creatures = 
            look_up(FOX, number_values) + 
            look_up(RABBIT, number_values) + 
            look_up(CARROT, number_values) + 
            look_up(TREE, number_values);
            
    if (total_creatures >= board_size*board_size) {
        free(data);
        perror("Too many creatures to initialize");
        return false;
    }

    random_spawn(look_up(FOX, number_values), FOX, game_board);
    random_spawn(look_up(RABBIT, number_values), RABBIT, game_board);
    random_spawn(look_up(CARROT, number_values), CARROT, game_board);
    random_spawn(look_up(TREE, number_values), TREE, game_board);
    update_grid(data->map, game_board,true);
    return true;
}

// Updates grid with board values
void update_grid(GtkWidget *grd_map, board_t *board,bool start) {
    for (int i = 0; i < board_size; i++) {
        for (int j = 0; j < board_size; j++) {
            GtkWidget *pic;
            if (!start){
                gtk_widget_destroy(gtk_grid_get_child_at (GTK_GRID(grd_map), i, j));
            }
            if(board->squares[j][i].creature){
                switch (board->squares[j][i].creature->type)
                {
                case FOX: ;
                    pic = gtk_image_new_from_file(FOX_PICTURE);
                     gtk_grid_attach(GTK_GRID(grd_map), pic, i, j, 1, 1);
                    gtk_widget_show(pic);
                    break;
                case RABBIT: ;
                    pic = gtk_image_new_from_file(RABBIT_PICTURE);
                     gtk_grid_attach(GTK_GRID(grd_map), pic, i, j, 1, 1);
                    gtk_widget_show(pic);
                    break;
                case CARROT: ;
                    pic = gtk_image_new_from_file(CARROT_PICTURE);
                    gtk_grid_attach(GTK_GRID(grd_map), pic, i, j, 1, 1);
                    gtk_widget_show(pic);
                    break;
                case TREE: ;
                    pic = gtk_image_new_from_file(TREE_PICTURE);
                    gtk_grid_attach(GTK_GRID(grd_map), pic, i, j, 1, 1);
                    gtk_widget_show(pic);
                    break;
                default:
                    break;
                }

            } else {
                pic = gtk_image_new_from_file(GRASS_PICTURE);
                gtk_grid_attach(GTK_GRID(grd_map), pic, i, j, 1, 1);
                gtk_widget_show(pic);
            }
        }
    }   
}

// Adds text to the log in the GUI
void add_text(GtkWidget *textview,textlog_t textlog) {

    GtkTextBuffer *textbuffer = gtk_text_buffer_new(NULL);
    int size = textlog.current_size;
    
    for (int i = 0; i < size; i++) {
        
        gchar * message = textlog.texts[i];
        gtk_text_buffer_insert_at_cursor(textbuffer, message, -1);
    }
    gtk_text_view_set_buffer (GTK_TEXT_VIEW(textview), textbuffer);

    g_object_unref(textbuffer);
}

// Starts each round
void start(void) {
    board_t *game_board = data->board;
    
    world(game_board, data->current);
    update_grid(data->map, game_board,false);

    add_text(data->textview, game_board->textlog);

    data->current++;
    if(data->current == data->rounds){
        gtk_widget_set_sensitive (data->start, FALSE);
        gtk_widget_set_sensitive (data->pause, FALSE);
            free_all(game_board);
        g_source_remove(data->ID);
        free(data);
    }
}


int main(int argc, char *argv[]) {
    
    // Initalises the arguments for the simulation
    gtk_init(&argc, &argv);
    init_tables();
    reads_file(argv);
    srand(seed);

    GtkBuilder *builder = gtk_builder_new_from_file(GLADE);
    
    data = malloc(sizeof(game_data_t));
    data->current = 0;
    data->rounds = num_of_round;
    data->start = GTK_WIDGET(gtk_builder_get_object(builder, "Start"));
    data->pause = GTK_WIDGET(gtk_builder_get_object(builder, "Pause"));
    data->textview = GTK_WIDGET(gtk_builder_get_object(builder, "TextView"));
    data->map =  GTK_WIDGET(gtk_builder_get_object(builder, "Map"));
    data->board = create_board();

    GtkWidget *mainWindow = GTK_WIDGET(gtk_builder_get_object(builder, "MainWindow"));

    gtk_builder_connect_signals(builder, NULL);

    if (!init_grid(data)){
        return EXIT_FAILURE;
    }
    add_css();
    
    gtk_window_fullscreen(GTK_WINDOW(mainWindow));

    gtk_widget_show(mainWindow);

    g_object_unref(builder);

    gtk_main();
    
    return EXIT_SUCCESS;
}

// Callbacks

// Quit when window closes
void on_MainWindow_destroy(void) {
    gtk_main_quit();
}

// Save log to log file
void on_SaveButton_clicked(GtkWidget *save, gpointer phi) {
    char *text_to_save = get_text(phi);
    FILE *ptr = fopen("Events.log", "w");
    fprintf(ptr, "%s", text_to_save);
    fclose(ptr);
    g_free(text_to_save);
}

// Get text from textview
char *get_text(GtkTextView *text_view) {
    GtkTextIter start, end;
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(text_view);
    gchar *text;
    gtk_text_buffer_get_bounds(buffer, &start, &end);
    text = gtk_text_buffer_get_text(buffer, &start, &end, FALSE);
    return text;
}

// Start the simulation
void on_Start_clicked(void) {
    if(data->current != data->rounds){
        gint i = g_timeout_add(500, G_SOURCE_FUNC(start), NULL);
        data->ID = i;
    }

}

//Pause the Simulation
void on_Pause_clicked(void) {
    g_source_remove(data->ID);
}