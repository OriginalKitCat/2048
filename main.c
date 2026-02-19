// You and me know Phython + Gtk is 10x easier, but I don't care. The next challenge will be brainfuck.

#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include <stdlib.h>
#include <time.h>
#include "include/raylib.h"
#include <math.h>
#include <gio/gio.h>
#include <gst/gst.h>
#include <png.h>

#define SIZE 4

// Just for my self to remember
// Pop Sound https://pixabay.com/sound-effects/pop-402323/

// GstElement *playbin;
GError *error = NULL;

GtkWidget *playarea_vbox;
GtkWidget *winscreen_vbox;
GtkWidget *labels[SIZE][SIZE];
GtkWidget *drawing_area;
GtkWidget *scoreboard;
GtkWidget *highscoreboard;
GtkWidget *settings_popup;
GtkWidget *score_info;
GtkWidget *scorelabel;
GtkWidget *highscorelabel;
GtkWidget *movescountdisplay;
GtkWidget *timerlabel;
GtkWidget *welcomeScreen;
GtkWidget *restart_label;
GtkWidget *soundeffectsicon;
GtkWidget *soundeffectlabel;
GtkWidget *backgrounmusiclabel;
float shader_state = 0.5; // Don't use more than 2 decimals, 4 are theoretically possible but don't do this!
int shader_state_direaction = 1;
float shader_speed = 0.01; // Should be not to large...
int update_time = 50; //in ms 
int tiles_count = 6;
int tiles_count_h = 6;
int tiles_count_v = 7;
bool tile_rotate = TRUE;
float opacity = 0.21; //Don't works, just was a idea, but not important at the moment.
bool sound_activated = true;
int gamestatus = 3; // 0 playing, 1 lost, 2 won, 3 start screen
int number2spawn = 1024;
bool soundeffectes = TRUE; 

int board[SIZE][SIZE] = {0};
int score = 0;
int highscore = 2008;
int moves = 0;

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

gint64 start_time;

gchar *pop_sound_uri = "file:///home/adam/Projekte/gtk_apps/2048/sounds/pop.mp3";

void update_cell(int row, int col, int value) {
    board[row][col] = value;

    char text[16];
    snprintf(text, sizeof(text), "%d", value);
    gtk_label_set_text(GTK_LABEL(labels[row][col]), text);
    GtkWidget *frame = gtk_widget_get_parent(labels[row][col]);
    gtk_widget_remove_css_class(frame, "one");
    gtk_widget_remove_css_class(frame, "two");
    gtk_widget_remove_css_class(frame, "four");
    gtk_widget_remove_css_class(frame, "eight");
    gtk_widget_remove_css_class(frame, "sixteen");
    gtk_widget_remove_css_class(frame, "thirtytwo");
    gtk_widget_remove_css_class(frame, "sixtyfour");
    gtk_widget_remove_css_class(frame, "hundredtwentyeitht");
    gtk_widget_remove_css_class(frame, "twohundredfiftysix");
    gtk_widget_remove_css_class(frame, "fivehundredtwelve");
    gtk_widget_remove_css_class(frame, "thousendtwentyfour");
    gtk_widget_remove_css_class(frame, "normal");

    if (value == 1)
        gtk_widget_add_css_class(frame, "one");
    else if (value == 2)
        gtk_widget_add_css_class(frame, "two");
    else if (value == 4)
        gtk_widget_add_css_class(frame, "four");
    else if (value == 8)
        gtk_widget_add_css_class(frame, "eight");
    else if (value == 16)
        gtk_widget_add_css_class(frame, "sixteen");
    else if (value == 32)
        gtk_widget_add_css_class(frame, "thirtytwo");
    else if (value == 64)
        gtk_widget_add_css_class(frame, "sixtyfour");
    else if (value == 128)
        gtk_widget_add_css_class(frame, "hundredtwentyeitht");
    else if (value == 256)
        gtk_widget_add_css_class(frame, "twohundredfiftysix");
    else if (value == 512)
        gtk_widget_add_css_class(frame, "fivehundredtwelve");
    else if (value == 1024)
        gtk_widget_add_css_class(frame, "thousendtwentyfour");
    else
        gtk_widget_add_css_class(frame, "normal");
}

void spawn_number() {
    int rand_x_numb = rand() % SIZE;
    int rand_Y_numb = rand() % SIZE;

    while (board[rand_x_numb][rand_Y_numb] != 0) {
        rand_x_numb = rand() % SIZE;
        rand_Y_numb = rand() % SIZE;
    }
    update_cell(rand_x_numb, rand_Y_numb, number2spawn);
}

// void play_pop_sound() {
//     GstElement *pipeline;
//     pipeline = gst_parse_launch("playbin uri=NULL", NULL);
//     g_object_set(pipeline, "uri", pop_sound_uri, NULL);
//     gst_element_set_state(pipeline, GST_STATE_PLAYING);
//     GstBus *bus = gst_element_get_bus(pipeline);
//     GstMessage *msg = gst_bus_timed_pop_filtered(bus, GST_CLOCK_TIME_NONE, GST_MESSAGE_EOS | GST_MESSAGE_ERROR);
//     if (msg != NULL) {
//         if (GST_MESSAGE_TYPE(msg) == GST_MESSAGE_ERROR) {
//             GError *err;
//             gchar *debug;
//             gst_message_parse_error(msg, &err, &debug);
//             g_print("Error: %s\n", err->message);
//             g_error_free(err);
//             g_free(debug);
//         }
//         gst_message_unref(msg);
//     }
//     gst_object_unref(bus);
//     gst_element_set_state(pipeline, GST_STATE_NULL);
//     gst_object_unref(pipeline);
// }

void play_pop_sound() {
    GstElement *pipeline = gst_parse_launch("playbin uri=NULL", NULL);
    g_object_set(pipeline, "uri", pop_sound_uri, NULL);
    gst_element_set_state(pipeline, GST_STATE_PLAYING);
}

void play_background_music() {
    GstElement *backgroundpipe = gst_parse_launch("playbin uri=NULL", NULL);
    g_object_set(backgroundpipe, "uri", pop_sound_uri, NULL);
    gst_element_set_state(backgroundpipe, GST_STATE_PLAYING);
}

void update_score() {
    char score_text[32];
    snprintf(score_text, sizeof(score_text), "Score: %d", score);
    gtk_label_set_text(GTK_LABEL(scoreboard), score_text);
    char high_label[32];
    snprintf(high_label, sizeof(high_label), "High: %d", highscore);
    gtk_label_set_text(GTK_LABEL(highscoreboard), high_label);

    if (score > highscore) {
        highscore = score;
    }
}

void update_timer() {
    gint64 now = g_get_monotonic_time();
    double elapsed = (now - start_time) / 1000000.0;
}

bool play_again() {
    gtk_widget_set_visible(playarea_vbox, FALSE);
    gtk_widget_set_visible(winscreen_vbox, FALSE);
    gtk_widget_set_visible(welcomeScreen, TRUE);
    score = 0;
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            board[i][j] = 0;
            update_cell(i, j, board[i][j]);
        }
    }
    char highscorestring[32];
    snprintf(highscorestring, sizeof(highscorestring), "Highscore: %d", highscore);
    gtk_label_set_text(GTK_LABEL(highscorelabel), highscorestring);
    gamestatus = 3;
    return FALSE;
}

void cairo_rounded_rectangle(cairo_t *cr, double x, double y, double width, double height, double radius) {
    double degrees = M_PI / 180.0;
    cairo_new_sub_path(cr);
    cairo_arc(cr, x + width - radius, y + radius, radius, -90 * degrees, 0 * degrees);
    cairo_arc(cr, x + width - radius, y + height - radius, radius, 0 * degrees, 90 * degrees);
    cairo_arc(cr, x + radius, y + height - radius, radius, 90 * degrees, 180 * degrees);
    cairo_arc(cr, x + radius, y + radius, radius, 180 * degrees, 270 * degrees);
    cairo_close_path(cr);
}

static void draw_board(cairo_t *cr, int width, int height)
{
    cairo_set_source_rgb(cr, 0, 0, 0);
    cairo_paint(cr);

    int canvas_size = 400;
    canvas_size = width;

    int onetilesize = canvas_size / tiles_count;

    for (int j = 0; j < tiles_count_v; j++) {
        for (int i = 0; i < tiles_count_h; i++) {
            float localshaderstate = shader_state;
            if (j % 2 == 1) {
                if (i % 2 == 1) {
                    localshaderstate = 1.0 - shader_state;
                }
            }
            else {
                if (i % 2 != 1) {
                    localshaderstate = 1.0 - shader_state;
                }
            }
            // if (tile_rotate) {
            //     localshaderstate = 1.0 - shader_state;
            // }
            int point1x, point2x, point1y, point2y;

            if (localshaderstate <= 0.5) {
                point1x = localshaderstate * onetilesize;
                point2x = onetilesize - localshaderstate * onetilesize;
                point1y = 0.5 * onetilesize;
                point2y = 0.5 * onetilesize;
            } else {
                point1x = 0.5 * onetilesize;
                point2x = 0.5 * onetilesize;
                point1y = onetilesize - localshaderstate * onetilesize;
                point2y = localshaderstate * onetilesize;
            }

            cairo_set_source_rgba(cr, 1.0, 1.0, 0.0, 0.3);
            if (localshaderstate <= 0.5) {
                cairo_move_to (cr, 0 + i * onetilesize, 0 + j * onetilesize);
                cairo_line_to (cr, point1x + i * onetilesize, point1y + j * onetilesize);
                cairo_move_to (cr, 0 + i * onetilesize, onetilesize + j * onetilesize);
                cairo_line_to (cr, point1x + i * onetilesize, point1y + j * onetilesize);
                cairo_move_to (cr, onetilesize + i * onetilesize, 0 + j * onetilesize);
                cairo_line_to (cr, point2x + i * onetilesize, point2y + j * onetilesize);
                cairo_move_to (cr, onetilesize + i * onetilesize, onetilesize + j * onetilesize);
                cairo_line_to (cr, point2x + i * onetilesize, point2y + j * onetilesize);
                cairo_move_to (cr, point1x + i * onetilesize, point1y + j * onetilesize);
                cairo_line_to (cr, point2x + i * onetilesize, point2y + j * onetilesize);
            }
            else{
                cairo_move_to (cr, 0 + i * onetilesize, 0 + j * onetilesize);
                cairo_line_to (cr, point1x + i * onetilesize, point1y + j * onetilesize);
                cairo_move_to (cr, 0 + i * onetilesize, onetilesize + j * onetilesize);
                cairo_line_to (cr, point2x + i * onetilesize, point2y + j * onetilesize);
                cairo_move_to (cr, onetilesize + i * onetilesize, 0 + j * onetilesize);
                cairo_line_to (cr, point1x + i * onetilesize, point1y + j * onetilesize);
                cairo_move_to (cr, onetilesize + i * onetilesize, onetilesize + j * onetilesize);
                cairo_line_to (cr, point2x + i * onetilesize, point2y + j * onetilesize);
                cairo_move_to (cr, point1x + i * onetilesize, point1y + j * onetilesize);
                cairo_line_to (cr, point2x + i * onetilesize, point2y + j * onetilesize);
            }
        }
    }

    cairo_set_line_width (cr, 1);

    cairo_stroke(cr);
}

static void draw_function(GtkDrawingArea *area, cairo_t *cr, int width, int height, gpointer data) {
    draw_board(cr, width, height);
}

void export_score_png() {
    int width = 400;
    int height = 450;
    int onetilesize =  width / tiles_count;

    cairo_text_extents_t extents;
    double x_that_centers_that_damm_text;
    cairo_surface_t *surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, width, height);
    cairo_t *cr = cairo_create(surface);

    draw_board(cr, width, height);

    cairo_select_font_face(cr, "Sans", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
    cairo_set_font_size(cr, 26);
    cairo_set_source_rgb(cr, 0.58, 0.58, 0.58);

    cairo_text_extents(cr, "2048", &extents);
    x_that_centers_that_damm_text = (width  - extents.width)  / 2 - extents.x_bearing;
    cairo_move_to(cr, x_that_centers_that_damm_text, onetilesize - 24);
    cairo_show_text(cr, "2048");
    cairo_stroke(cr);

    cairo_text_extents(cr, "You won!", &extents);

    cairo_set_source_rgba(cr, 1, 1, 1, 0.21);
    cairo_rounded_rectangle(cr, 15, onetilesize - 9, 370, 60 + extents.height, 20);
    cairo_fill(cr);
    cairo_set_source_rgb(cr, 1, 1, 0);
    cairo_set_line_width(cr, 0.4);
    cairo_rounded_rectangle(cr, 15, onetilesize - 9, 370, 60 + extents.height, 20);
    cairo_stroke(cr);

    cairo_set_source_rgb(cr, 0.58, 0.58, 0.58);
    cairo_set_font_size(cr, 18);
    x_that_centers_that_damm_text = (width  - extents.width)  / 2 - extents.x_bearing;
    cairo_move_to(cr, x_that_centers_that_damm_text, onetilesize + extents.height + 15);
    cairo_show_text(cr, "You won!");

    cairo_surface_write_to_png(surface, "board.png");

    cairo_destroy(cr);
    cairo_surface_destroy(surface);


    //Just a placeholder at the moment
    g_print("Feature not aviveable at the moment. Please look out for updates...");
}

void check_if_won() {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (board[i][j] == 2048) {
                gamestatus = 2;
            }
        }
    }
    if (gamestatus == 2) {
        gtk_widget_set_visible(playarea_vbox, FALSE);
        gtk_widget_set_visible(winscreen_vbox, TRUE);

        char scorestring[32];
        snprintf(scorestring, sizeof(scorestring), "Score: %d", score);
        gtk_label_set_text(GTK_LABEL(scorelabel), scorestring);

        char highscorestring[32];
        snprintf(highscorestring, sizeof(highscorestring), "Highscore: %d", highscore);
        gtk_label_set_text(GTK_LABEL(highscorelabel), highscorestring);

        char movescountstring[32];
        snprintf(movescountstring, sizeof(movescountstring), "Moves Count: %d", moves);
        gtk_label_set_text(GTK_LABEL(movescountdisplay), movescountstring);

        gint64 now = g_get_monotonic_time();
        double elapsed = (now - start_time) / 1000000.0;

        snprintf(highscorestring, sizeof(highscorestring), "Time: %.2f sec", elapsed);
        gtk_label_set_text(GTK_LABEL(timerlabel), highscorestring);
    }
}

void move_to_left() {
    int move = 0;
    int times_changed = 0;

    for (int i = 0; i < SIZE; i++) {
        int temp[SIZE] = {0};
        int index = 0;

        for (int j = 0; j < SIZE; j++) {
            if (board[i][j] != 0) {
                temp[index++] = board[i][j];
                
            }
        }


        for (int j = 0; j < SIZE - 1; j++) {
            if (temp[j] != 0 && temp[j] == temp[j + 1]) {
                temp[j] *= 2;
                score += temp[j];
                temp[j + 1] = 0;
                times_changed++;
            }
        }

        int final_row[SIZE] = {0};
        index = 0;
        for (int j = 0; j < SIZE; j++) {
            if (temp[j] != 0) {
                final_row[index++] = temp[j];
            }
        }

        for (int j = 0; j < SIZE; j++) {
            if (board[i][j] != final_row[j]) {
                move = 1;
            }
            board[i][j] = final_row[j];
            update_cell(i, j, board[i][j]); 
        }
    }
    if (move)
        spawn_number();
    update_score();
    moves += 1;
    if (soundeffectes) {
        for (times_changed; times_changed > 0; times_changed--) {
            play_pop_sound();
        }
    }
    check_if_won();
}

void moveright() {
    int move = 0;
    int times_changed = 0;
    for (int i = 0; i < SIZE; i++) {
        int temp[SIZE] = {0};
        int index = SIZE - 1;

        for (int j = SIZE - 1; j >= 0; j--) {
            if (board[i][j] != 0) {
                temp[index--] = board[i][j];
            }
        }

        for (int j = SIZE - 1; j > 0; j--) {
            if (temp[j] != 0 && temp[j] == temp[j - 1]) {
                temp[j] *= 2;
                score += temp[j];
                temp[j - 1] = 0;
                times_changed++;
            }
        }

        int final_row[SIZE] = {0};
        index = SIZE - 1;
        for (int j = SIZE - 1; j >= 0; j--) {
            if (temp[j] != 0) {
                final_row[index--] = temp[j];
            }
        }

        for (int j = 0; j < SIZE; j++) {
            if (board[i][j] != final_row[j]) {
                move = 1;
            }
            board[i][j] = final_row[j];
            update_cell(i, j, board[i][j]);
        }
    }
    if (move) {
        spawn_number();
    }
    moves += 1;
    update_score(score);
    if (soundeffectes) {
        for (times_changed; times_changed > 0; times_changed--) {
            play_pop_sound();
        }
    }
    check_if_won();
}

void move_upwards() {
    int move = 0;

    int times_changed = 0;
    // is_game_over();

    for (int col = 0; col < SIZE; col++) {
        int temp[SIZE] = {0};
        int index = 0;

        for (int row = 0; row < SIZE; row++) {
            if (board[row][col] != 0) {
                temp[index++] = board[row][col];
            }
        }

        for (int j = 0; j < SIZE - 1; j++) {
            if (temp[j] != 0 && temp[j] == temp[j + 1]) {
                temp[j] *= 2;
                score += temp[j];
                temp[j + 1] = 0;
                times_changed++;
            }
        }

        int final_col[SIZE] = {0};
        index = 0;
        for (int j = 0; j < SIZE; j++) {
            if (temp[j] != 0) {
                final_col[index++] = temp[j];
            }
        }

        for (int row = 0; row < SIZE; row++) {
            if (board[row][col] != final_col[row]) {
                move = 1;
            }
            board[row][col] = final_col[row];
            update_cell(row, col, final_col[row]);
        }
    }

    if (move)
        spawn_number();
    moves += 1;
    update_score(score);
    if (soundeffectes) {
        for (times_changed; times_changed > 0; times_changed--) {
            play_pop_sound();
        }
    }
    check_if_won();
}

void move_down() {
    int move = 0;
    int times_changed = 0;

    for (int col = 0; col < SIZE; col++) {
        int temp[SIZE] = {0};
        int index = 0;

        for (int row = SIZE - 1; row >= 0; row--) {
            if (board[row][col] != 0) {
                temp[index++] = board[row][col];
            }
        }

        for (int j = 0; j < SIZE - 1; j++) {
            if (temp[j] != 0 && temp[j] == temp[j + 1]) {
                temp[j] *= 2;
                score += temp[j];
                temp[j + 1] = 0;
                times_changed++;
            }
        }

        int final_col[SIZE] = {0};
        index = SIZE - 1;
        for (int j = 0; j < SIZE; j++) {
            if (temp[j] != 0) {
                final_col[index--] = temp[j];
            }
        }

        for (int row = SIZE - 1; row >= 0; row--) {
            if (board[row][col] != final_col[row]) {
                move = 1;
            }
            board[row][col] = final_col[row];
            update_cell(row, col, final_col[row]);
        }
    }

    if (move)
        spawn_number();
    moves += 1;
    update_score(score);
    if (soundeffectes) {
        for (times_changed; times_changed > 0; times_changed--) {
            play_pop_sound();
        }
    }
    check_if_won();
}

void startgame() {
    spawn_number();
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            int value = board[i][j];
            update_cell(i, j, board[i][j]);
        }
    }
    gtk_widget_set_visible(welcomeScreen, FALSE);
    gtk_widget_set_visible(playarea_vbox, TRUE);
    start_time = g_get_monotonic_time();
    gamestatus = 0;
}

void dificulty_hard() {
    number2spawn = 1;
    startgame();
}

void dificulty_normal() {
    number2spawn = 2;
    startgame();
}

void dificulty_easy() {
    number2spawn = 16;
    startgame();
}

void dificulty_very_easy() {
    number2spawn = 256;
    startgame();
}

char *get_config_file_path(void)
{
    char *config_dir = g_build_filename(
        g_get_user_config_dir(),
        "apps.kitcat.game2048",
        NULL
    );

    g_mkdir_with_parents(config_dir, 0700);

    char *config_file = g_build_filename(
        config_dir,
        "config.ini",
        NULL
    );

    g_free(config_dir);
    return config_file;
}

void save_config(int highscore)
{
    GKeyFile *keyfile = g_key_file_new();
    char *file_path = get_config_file_path();

    g_key_file_set_integer(keyfile, "highscore", "high", highscore);

    gsize length;
    char *data = g_key_file_to_data(keyfile, &length, NULL);

    g_file_set_contents(file_path, data, length, NULL);

    g_free(data);
    g_free(file_path);
    g_key_file_free(keyfile);
}

void load_config(int *width, int *height, char **username)
{
    GKeyFile *keyfile = g_key_file_new();
    char *file_path = get_config_file_path();

    if (g_key_file_load_from_file(keyfile, file_path, G_KEY_FILE_NONE, NULL)) {

        *width  = g_key_file_get_integer(keyfile, "window", "width", NULL);
        *height = g_key_file_get_integer(keyfile, "window", "height", NULL);
        *username = g_key_file_get_string(keyfile, "user", "name", NULL);
    }

    g_free(file_path);
    g_key_file_free(keyfile);
}

void stop_start_music() {
    if (soundeffectes == TRUE) {
        soundeffectes = FALSE;
        gtk_label_set_label(GTK_LABEL(backgrounmusiclabel), "Backgroundmusic off");
    } else if (soundeffectes == FALSE){
        soundeffectes = TRUE;
        gtk_label_set_label(GTK_LABEL(backgrounmusiclabel), "Backgroundmusic on ");
    }
}

void stop_start_sound() {
    if (soundeffectes == TRUE) {
        soundeffectes = FALSE;
        gtk_image_set_from_file(GTK_IMAGE(soundeffectsicon), "lucide-icons/volume-x.svg");
        gtk_label_set_label(GTK_LABEL(soundeffectlabel), "Sound Effects off");
    } else if (soundeffectes == FALSE){
        soundeffectes = TRUE;
        gtk_image_set_from_file(GTK_IMAGE(soundeffectsicon), "lucide-icons/volume-2.svg");
        gtk_label_set_label(GTK_LABEL(soundeffectlabel), "Sound Effects on");
    }
}

gboolean update_shaderstate(gpointer data) {
    shader_state += shader_speed * shader_state_direaction;
    if (shader_state <= 0.0) {
        shader_state_direaction = -shader_state_direaction;
        // shader_state = 1 - shader_state;
        tile_rotate = !tile_rotate;
    } 
    else if (shader_state >= 1.0) {
        shader_state_direaction = -shader_state_direaction;
        // shader_state = 1 - shader_state;
        tile_rotate = !tile_rotate;
    }
    gtk_widget_queue_draw(drawing_area);
    return TRUE;
}


void controlpadding(GtkWidget *window, GtkAllocation *allocation, gpointer user_data) {
    GtkWidget *grid = GTK_WIDGET(user_data);
    int width = allocation->width;
    int height = allocation->height;

    int h_margin = 0;
    int v_margin = 0;

    if (width > height)
        h_margin = (width - height) / 2;
    else if (height > width)
        v_margin = (height - width) / 2;

    gtk_widget_set_margin_bottom(grid, v_margin);
    gtk_widget_set_margin_top(grid, v_margin);
    gtk_widget_set_margin_start(grid, h_margin);
    gtk_widget_set_margin_end(grid, h_margin);
}

gboolean on_key_pressed(GtkEventControllerKey *controller, guint keyval, guint keycode, GdkModifierType state, gpointer user_data) {
    if (gamestatus == 0) {
        if (keyval == GDK_KEY_space) {
            // for (int i = 0; i < SIZE; i++) {
            //     for (int j = 0; j < SIZE; j++) {
            //         update_cell(i, j, 0);
            //     }
            // }
            spawn_number();
            return TRUE;
        }
        if (keyval == GDK_KEY_Left || keyval == GDK_KEY_A || keyval == GDK_KEY_a) {
            move_to_left();
        }
        if (keyval == GDK_KEY_Right || keyval == GDK_KEY_D || keyval == GDK_KEY_d){
            moveright();
        }
        if (keyval == GDK_KEY_Up || keyval == GDK_KEY_W || keyval == GDK_KEY_w){
            move_upwards();
        }
        if (keyval == GDK_KEY_Down || keyval == GDK_KEY_S || keyval == GDK_KEY_s)
            move_down();
        return FALSE;
    }
}

void menu_button_pressed(GtkButton *button, gpointer user_data)
{
    GtkPopover *popover = GTK_POPOVER(user_data);
    GtkWidget  *widget  = GTK_WIDGET(button);

    GdkRectangle rect = { 0, 0, 1, 1 };
    gtk_popover_set_pointing_to(popover, &rect);
    gtk_popover_set_position(popover, GTK_POS_BOTTOM);

    gtk_popover_popup(popover);
}


void activate(GtkApplication *app, gpointer data) {
    gst_init(NULL, NULL);

    GtkWidget *window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "2048");
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 450);
    gtk_widget_set_focusable(GTK_WIDGET(window), TRUE);
    gtk_widget_grab_focus(GTK_WIDGET(window));

    GtkWidget *overeachother = gtk_overlay_new();
    gtk_window_set_child(GTK_WINDOW(window), overeachother);

    playarea_vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 6);
    gtk_overlay_add_overlay(GTK_OVERLAY(overeachother), playarea_vbox);
    gtk_widget_set_visible(playarea_vbox, FALSE);
    // gtk_widget_set_visible(playarea_vbox, FALSE); // I have to comment this out when win screen is finished.

    // GtkWidget *overlay = gtk_overlay_new();
    // gtk_widget_set_hexpand(overlay, TRUE);
    // gtk_widget_set_vexpand(overlay, TRUE);

    GtkWidget *info_area = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_widget_set_hexpand(info_area, FALSE);
    gtk_widget_set_halign(info_area, GTK_ALIGN_CENTER);
    gtk_box_append(GTK_BOX(playarea_vbox), info_area);

    scoreboard = gtk_label_new("Score: 0");
    gtk_widget_add_css_class(scoreboard, "score-label");
    // gtk_widget_set_halign(scoreboard, GTK_ALIGN_CENTER);
    // gtk_widget_set_valign(scoreboard, GTK_ALIGN_CENTER);
    gtk_widget_set_margin_top(scoreboard, 10);
    gtk_widget_set_margin_bottom(scoreboard, 5);
    gtk_widget_set_hexpand(scoreboard, FALSE);
    gtk_widget_set_vexpand(scoreboard, FALSE);
    gtk_widget_add_css_class(scoreboard, "infobox");
    gtk_box_append(GTK_BOX(info_area), scoreboard);

    highscoreboard = gtk_label_new("Failed to get high");
    gtk_widget_add_css_class(highscoreboard, "score-label");
    // gtk_widget_set_halign(highscore, GTK_ALIGN_CENTER);
    // gtk_widget_set_valign(highscore, GTK_ALIGN_CENTER);
    char highscorestring[32];
    snprintf(highscorestring, sizeof(highscorestring), "High: %d", highscore);
    gtk_label_set_text(GTK_LABEL(highscoreboard), highscorestring);
    gtk_widget_set_margin_top(highscoreboard, 10);
    gtk_widget_set_margin_bottom(highscoreboard, 5);
    gtk_widget_set_hexpand(highscoreboard, FALSE);
    gtk_widget_set_vexpand(highscoreboard, FALSE);
    gtk_widget_add_css_class(highscoreboard, "infobox");
    gtk_box_append(GTK_BOX(info_area), highscoreboard);

    GtkWidget *menu_icon = gtk_image_new_from_file("lucide-icons/menu.svg");
    GtkWidget *menubutton = gtk_button_new();
    gtk_widget_set_margin_top(menubutton, 10);
    gtk_widget_set_margin_bottom(menubutton, 5);
    gtk_widget_set_hexpand(menubutton, FALSE);
    gtk_widget_set_vexpand(menubutton, FALSE);
    gtk_button_set_child(GTK_BUTTON(menubutton), menu_icon);
    gtk_box_append(GTK_BOX(info_area), menubutton);
    gtk_widget_add_css_class(menubutton, "infobutton");

    //Hopefully I'll work on that later on - and it will work too...
    settings_popup = gtk_popover_new();
    // gtk_popover_set_relative_to(GTK_POPOVER(settings_popup), menubutton); // This was the gtk3 way ig
    gtk_widget_set_parent(settings_popup, menubutton);
    gtk_popover_set_autohide(GTK_POPOVER(settings_popup), TRUE);
    gtk_popover_set_has_arrow(GTK_POPOVER(settings_popup), TRUE);
    // gtk_popover_set_pointing_to(GTK_POPOVER(settings_popup), menubutton);
    gtk_widget_add_css_class(settings_popup, "popover");

    GtkWidget *menu_vStack = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_widget_set_margin_top(menu_vStack, 15);
    gtk_widget_set_margin_bottom(menu_vStack, 15);
    gtk_widget_set_margin_start(menu_vStack, 15);
    gtk_widget_set_margin_end(menu_vStack, 15);
    gtk_popover_set_child(GTK_POPOVER(settings_popup), menu_vStack);

    GtkWidget *bgm_button = gtk_button_new();
    gtk_box_append(GTK_BOX(menu_vStack), bgm_button);
    gtk_widget_add_css_class(bgm_button, "sm_infobutton");
    GtkWidget *bgm_h_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_button_set_child(GTK_BUTTON(bgm_button), bgm_h_box);
    GtkWidget *bgm_icon = gtk_image_new_from_file("lucide-icons/piano.svg");
    gtk_widget_set_margin_end(bgm_icon, 6);
    gtk_box_append(GTK_BOX(bgm_h_box), bgm_icon);
    backgrounmusiclabel = gtk_label_new("Backgroundmusic on ");
    gtk_box_append(GTK_BOX(bgm_h_box), backgrounmusiclabel);
    gtk_label_set_xalign(GTK_LABEL(bgm_h_box), 0.5f);
    gtk_widget_set_hexpand(bgm_h_box, TRUE);

    GtkWidget *effects_mute = gtk_button_new();
    gtk_box_append(GTK_BOX(menu_vStack), effects_mute);
    gtk_widget_add_css_class(effects_mute, "sm_infobutton");
    GtkWidget *eff_mute_h_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_button_set_child(GTK_BUTTON(effects_mute), eff_mute_h_box);
    soundeffectsicon = gtk_image_new_from_file("lucide-icons/volume-2.svg");
    gtk_box_append(GTK_BOX(eff_mute_h_box), soundeffectsicon);
    soundeffectlabel = gtk_label_new("Sound Effects on");
    gtk_box_append(GTK_BOX(eff_mute_h_box), soundeffectlabel);
    gtk_label_set_xalign(GTK_LABEL(soundeffectlabel), 0.5f);
    gtk_widget_set_hexpand(soundeffectlabel, TRUE);

    GtkWidget *restart_game = gtk_button_new();
    gtk_box_append(GTK_BOX(menu_vStack), restart_game);
    gtk_widget_add_css_class(restart_game, "sm_infobutton");
    GtkWidget *restart_h_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_button_set_child(GTK_BUTTON(restart_game), restart_h_box);
    GtkWidget *restart_icon = gtk_image_new_from_file("lucide-icons/rotate-ccw.svg");
    gtk_box_append(GTK_BOX(restart_h_box), restart_icon);
    restart_label = gtk_label_new("Restart game");
    gtk_box_append(GTK_BOX(restart_h_box), restart_label);
    gtk_label_set_xalign(GTK_LABEL(restart_label), 0.5f);
    gtk_widget_set_hexpand(restart_label, TRUE);

    g_signal_connect(menubutton, "clicked", G_CALLBACK(menu_button_pressed), settings_popup);
    g_signal_connect(restart_game, "clicked", G_CALLBACK(play_again), NULL);
    g_signal_connect(effects_mute, "clicked", G_CALLBACK(stop_start_sound), NULL);
    g_signal_connect(bgm_button, "clicked", G_CALLBACK(stop_start_music), NULL);


    //Win screen
    winscreen_vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_widget_set_margin_top(winscreen_vbox, 15);
    gtk_widget_set_margin_bottom(winscreen_vbox, 15);
    gtk_widget_set_margin_start(winscreen_vbox, 15);
    gtk_widget_set_margin_end(winscreen_vbox, 15);
    gtk_overlay_add_overlay(GTK_OVERLAY(overeachother), winscreen_vbox);
    gtk_widget_set_visible(winscreen_vbox, FALSE);

    GtkWidget *twozeroheading = gtk_label_new("2048");
    gtk_widget_set_margin_bottom(twozeroheading, 5);
    gtk_widget_add_css_class(twozeroheading, "twozeroheadingcss");
    gtk_box_append(GTK_BOX(winscreen_vbox), twozeroheading);

    GtkWidget *won_heading = gtk_label_new("You won!");
    gtk_widget_set_margin_bottom(won_heading, 5);
    gtk_widget_add_css_class(won_heading, "infoboxheading");
    gtk_widget_set_hexpand(won_heading, TRUE);
    gtk_box_append(GTK_BOX(winscreen_vbox), won_heading);

    score_info = gtk_box_new(GTK_ORIENTATION_VERTICAL, 6);
    gtk_widget_add_css_class(score_info, "infoboxhuge");
    gtk_box_append(GTK_BOX(winscreen_vbox), score_info);
    gtk_widget_set_margin_bottom(score_info, 5);

    scorelabel = gtk_label_new("Error getting score :(");
    gtk_box_append(GTK_BOX(score_info), scorelabel);
    gtk_widget_set_halign(scorelabel, GTK_ALIGN_START);
    char scorestring[32];
    snprintf(scorestring, sizeof(scorestring), "Score: %d", score);
    gtk_label_set_text(GTK_LABEL(scorelabel), scorestring);

    highscorelabel = gtk_label_new("Error getting highscore :(");
    gtk_box_append(GTK_BOX(score_info), highscorelabel);
    gtk_widget_set_halign(highscorelabel, GTK_ALIGN_START);
    highscorestring[32];
    snprintf(highscorestring, sizeof(highscorestring), "Highscore: %d", highscore);
    gtk_label_set_text(GTK_LABEL(highscorelabel), highscorestring);

    movescountdisplay = gtk_label_new("Error getting moves count.");
    gtk_box_append(GTK_BOX(score_info), movescountdisplay);
    gtk_widget_set_halign(movescountdisplay, GTK_ALIGN_START);

    timerlabel = gtk_label_new("Error getting time");
    gtk_box_append(GTK_BOX(score_info), timerlabel);
    gtk_widget_set_halign(timerlabel, GTK_ALIGN_START);

    GtkWidget *shareButton = gtk_button_new();
    gtk_box_append(GTK_BOX(winscreen_vbox), shareButton);
    gtk_widget_add_css_class(shareButton, "infobutton");
    gtk_widget_set_margin_bottom(shareButton, 5);
    GtkWidget *shareLable = gtk_label_new("Get scoresheet");
    gtk_button_set_child(GTK_BUTTON(shareButton), shareLable);
    
    GtkWidget *playAgain = gtk_button_new();
    gtk_box_append(GTK_BOX(winscreen_vbox), playAgain);
    gtk_widget_add_css_class(playAgain, "infobutton");
    GtkWidget *buttonlable = gtk_label_new("Play Again!");
    gtk_button_set_child(GTK_BUTTON(playAgain), buttonlable);

    GtkWidget *copyrightnote = gtk_label_new("© 2026 KitCat / GNU GPLv3");
    gtk_box_append(GTK_BOX(winscreen_vbox), copyrightnote);
    gtk_widget_add_css_class(copyrightnote, "copyright");
    gtk_widget_set_margin_bottom(copyrightnote, 5);

    g_signal_connect(playAgain, "clicked", G_CALLBACK(play_again), NULL);
    g_signal_connect(shareButton, "clicked", G_CALLBACK(export_score_png), NULL);

    // Welcome Screen
    welcomeScreen = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_widget_set_margin_top(welcomeScreen, 15);
    gtk_widget_set_margin_bottom(welcomeScreen, 15);
    gtk_widget_set_margin_start(welcomeScreen, 15);
    gtk_widget_set_margin_end(welcomeScreen, 15);

    gtk_overlay_add_overlay(GTK_OVERLAY(overeachother), welcomeScreen);
    gtk_widget_set_visible(welcomeScreen, TRUE);

    GtkWidget *twozeroheading_two = gtk_label_new("2048");
    gtk_widget_set_margin_bottom(twozeroheading_two, 2);
    gtk_widget_add_css_class(twozeroheading_two, "twozeroheadingcss");
    gtk_box_append(GTK_BOX(welcomeScreen), twozeroheading_two);

    GtkWidget *explain_label_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 6);
    gtk_widget_add_css_class(explain_label_box, "infotext_unboxed");
    gtk_widget_set_margin_bottom(explain_label_box, 2);
    //gtk_widget_add_css_class(explain_label_box, "score-label");
    gtk_box_append(GTK_BOX(welcomeScreen), explain_label_box);

    GtkWidget *explain_label_one = gtk_label_new("2048 is a game where you merge numbers");
    gtk_box_append(GTK_BOX(explain_label_box), explain_label_one);

    GtkWidget *explain_label_two = gtk_label_new("till you reach 2048. Use WASD / Arrow keys.");
    gtk_box_append(GTK_BOX(explain_label_box), explain_label_two);

    GtkWidget *select_header = gtk_label_new("Select Gamemode");
    gtk_widget_set_margin_bottom(select_header, 7);
    gtk_widget_add_css_class(select_header, "selectgamemodecss");
    gtk_box_append(GTK_BOX(welcomeScreen), select_header);

    GtkWidget *buttonhard = gtk_button_new();
    gtk_box_append(GTK_BOX(welcomeScreen), buttonhard);
    gtk_widget_add_css_class(buttonhard, "infobutton");
    GtkWidget *button_hard_label = gtk_label_new("Hard");
    gtk_button_set_child(GTK_BUTTON(buttonhard), button_hard_label);
    g_signal_connect(buttonhard, "clicked", G_CALLBACK(dificulty_hard), NULL);

    GtkWidget *button_normal = gtk_button_new();
    gtk_box_append(GTK_BOX(welcomeScreen), button_normal);
    gtk_widget_add_css_class(button_normal, "infobutton");
    GtkWidget *button_normal_label = gtk_label_new("Normal");
    gtk_button_set_child(GTK_BUTTON(button_normal), button_normal_label);
    g_signal_connect(button_normal, "clicked", G_CALLBACK(dificulty_normal), NULL);

    GtkWidget *button_easy = gtk_button_new();
    gtk_box_append(GTK_BOX(welcomeScreen), button_easy);
    gtk_widget_add_css_class(button_easy, "infobutton");
    GtkWidget *button_easy_label = gtk_label_new("Easy");
    gtk_button_set_child(GTK_BUTTON(button_easy), button_easy_label);
    g_signal_connect(button_easy, "clicked", G_CALLBACK(dificulty_easy), NULL);

    GtkWidget *button_very_easy = gtk_button_new();
    gtk_box_append(GTK_BOX(welcomeScreen), button_very_easy);
    gtk_widget_add_css_class(button_very_easy, "infobutton");
    GtkWidget *button_very_easy_label = gtk_label_new("Too Easy");
    gtk_button_set_child(GTK_BUTTON(button_very_easy), button_very_easy_label);
    g_signal_connect(button_very_easy, "clicked", G_CALLBACK(dificulty_very_easy), NULL);

    GtkWidget *copyrightnote_two = gtk_label_new("© 2026 KitCat / GNU GPLv3");
    gtk_box_append(GTK_BOX(welcomeScreen), copyrightnote_two);
    gtk_widget_add_css_class(copyrightnote_two, "copyright");
    gtk_widget_set_margin_bottom(copyrightnote_two, 5);

    //Background
    drawing_area = gtk_drawing_area_new();
    gtk_drawing_area_set_content_width (GTK_DRAWING_AREA (drawing_area), 400);
    gtk_drawing_area_set_content_height (GTK_DRAWING_AREA (drawing_area), 400);
    gtk_drawing_area_set_draw_func (GTK_DRAWING_AREA (drawing_area), draw_function, NULL, NULL);
    gtk_widget_set_hexpand(drawing_area, FALSE);
    gtk_widget_set_vexpand(drawing_area, FALSE);
    // gtk_box_append(GTK_BOX(playarea_vbox), drawing_area);
    gtk_overlay_set_child(GTK_OVERLAY(overeachother), drawing_area);

    g_timeout_add(update_time, update_shaderstate, NULL);

    GtkWidget *container = gtk_aspect_frame_new(0.5, 0.5, 1.0, TRUE);
    gtk_box_append(GTK_BOX(playarea_vbox), container);
    // gtk_overlay_add_overlay(GTK_OVERLAY(overeachother), container);

    GtkWidget *grid = gtk_grid_new();
    gtk_grid_set_row_homogeneous(GTK_GRID(grid), TRUE);
    gtk_grid_set_column_homogeneous(GTK_GRID(grid), TRUE);
    gtk_widget_set_hexpand(grid, FALSE);
    gtk_widget_set_vexpand(grid, FALSE);
    gtk_aspect_frame_set_child(GTK_ASPECT_FRAME(container), grid);
    g_signal_connect(window, "size-allocate", G_CALLBACK(controlpadding), grid);



    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            char text[16];
            snprintf(text, sizeof(text), "%d", board[i][j]);
            labels[i][j] = gtk_label_new(text);
            gtk_widget_add_css_class(labels[i][j], "label");
            gtk_label_set_max_width_chars(GTK_LABEL(labels[i][j]), TRUE);
            gtk_widget_set_hexpand(labels[i][j], FALSE);
            gtk_widget_set_vexpand(labels[i][j], FALSE);
            gtk_label_set_xalign(GTK_LABEL(labels[i][j]), 0.5);
            gtk_label_set_yalign(GTK_LABEL(labels[i][j]), 0.5);
            gtk_widget_set_size_request(labels[i][j], 80, 80);

            GtkWidget *frame = gtk_frame_new(NULL);
            gtk_widget_set_hexpand(frame, FALSE);
            gtk_widget_set_vexpand(frame, FALSE);
            gtk_widget_set_margin_top(frame, 5);
            gtk_widget_set_margin_bottom(frame, 5);
            gtk_widget_set_margin_start(frame, 5);
            gtk_widget_set_margin_end(frame, 5);

            // GtkWidget *container = gtk_aspect_frame_new(0.5, 0.5, 1.0, TRUE);

            gtk_frame_set_child(GTK_FRAME(frame), labels[i][j]);
            // gtk_aspect_frame_set_child(GTK_ASPECT_FRAME(container), frame);
            gtk_grid_attach(GTK_GRID(grid), frame, j, i, 1, 1);
        }
    }

    GtkEventController *key_controller = gtk_event_controller_key_new();
    g_signal_connect(key_controller, "key-pressed", G_CALLBACK(on_key_pressed), NULL);
    gtk_widget_add_controller(window, key_controller);

    GtkCssProvider *css_provider = gtk_css_provider_new();
    gtk_css_provider_load_from_string(css_provider,
    ".normal { background-color: #d4d4d438; border: 1px solid rgba(255, 255, 0, 0.21); }"
    ".one { background-color: #bbff003d; border: 1px solid rgba(255, 255, 0, 0.21); }"
    ".two { background-color: #ffee003d; border: 1px solid rgba(255, 255, 0, 0.21); }"
    ".four { background-color: #ffd0002c; border: 1px solid rgba(255, 255, 0, 0.21); }"
    ".eight { background-color: #ffbb0025; border: 1px solid rgba(255, 255, 0, 0.21); }"
    ".sixteen { background-color: #ffae003d; border: 1px solid rgba(255, 255, 0, 0.21); }"
    ".thirtytwo { background-color: #ffa60038; border: 1px solid rgba(255, 255, 0, 0.21); }"
    ".sixtyfour { background-color: #ffa60038; border: 1px solid rgba(255, 255, 0, 0.21); }"
    ".hundredtwentyeitht { background-color: #ff880038; border: 1px solid rgba(255, 255, 0, 0.21); }"
    ".twohundredfiftysix { background-color: #ff730038; border: 1px solid rgba(255, 255, 0, 0.21); }"
    ".fivehundredtwelve { background-color: #ff5e0038; border: 1px solid rgba(255, 255, 0, 0.21); }"
    ".thousendtwentyfour { background-color: #ff480038; border: 1px solid rgba(255, 255, 0, 0.21); }"
    ".infobox { background-color: #d4d4d438; color: #d6d6d69a; font-size: 14px; font-weight: normal; border: 1px solid rgba(255, 255, 0, 0.21); border-radius: 12px; padding: 15px; }"
    ".sm_infobutton { background-color: #d4d4d438; color: #d6d6d69a; font-size: 14px; font-weight: normal; border: 1px solid rgba(255, 255, 0, 0.21); border-radius: 12px; padding: 15px; background-image: none; }"
    ".sm_infobutton:hover { background-color: #d4d4d460; background-image: none; }"
    ".infobutton { background-color: #d4d4d438; color: #d6d6d69a; font-size: 18px; font-weight: normal; border: 1px solid rgba(255, 255, 0, 0.21); border-radius: 12px; padding: 15px; background-image: none; }"
    ".infobutton:hover { background-color: #d4d4d460; background-image: none; }"
    ".infoboxheading { background-color: #d4d4d438; color: #d6d6d69a; font-size: 18px; font-weight: bold; border: 1px solid rgba(255, 255, 0, 0.21); border-radius: 12px; padding: 15px; }"
    ".infoboxhuge { background-color: #d4d4d438; color: #d6d6d69a; font-size: 18px; font-weight: normal; border: 1px solid rgba(255, 255, 0, 0.21); border-radius: 12px; padding: 15px; }"
    ".infoboxsmall { background-color: #d4d4d438; color: #d6d6d69a; font-size: 14px; font-weight: normal; border: 1px solid rgba(255, 255, 0, 0.21); border-radius: 12px; padding: 15px; }"
    ".twozeroheadingcss { color: #949494ff; font-size: 26px; font-weight: bold; }"
    ".selectgamemodecss { color: #949494; font-size: 20px; font-weight: bold; }"
    ".infotext_unboxed { color: #949494; font-size: 16px; font-weight: normal; }"
    ".copyright { color: #949494; font-size: 12px; font-weight: normal; } "
    ".popover.background > contents { background-color: #000000ff; border: 1px solid rgba(255, 255, 0, 0.42); }"
    ".popover.background > arrow{ background-color: #000000ff; border: 1px solid rgba(255, 255, 0, 0.42); }"
    // ".popover > contents > background { background-color: #000000ff; border-radius: 12px; color: #000000ff; }"
    ".label { color: #d6d6d69a; font-size: 28px; font-weight: bold; font-weight: bold;}");
    gtk_style_context_add_provider_for_display(
        gdk_display_get_default(),
        GTK_STYLE_PROVIDER(css_provider),
        GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    // update_cell(0, 2, 0);
    // controlpadding(window, grid);
    start_time = g_get_monotonic_time();
    gtk_window_present(GTK_WINDOW(window));
}

int main(int argc, char **argv) {
    GtkApplication *app;
    int status;
    start_time = clock();

    gst_init(&argc, &argv);

    srand((unsigned int)time(NULL));

    app = gtk_application_new("apps.kitcat.game2048", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);

    status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);

    return status;
}
