#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include <stdlib.h>
#include <time.h>
#include "include/raylib.h"
#include <math.h>

#define SIZE 4

GtkWidget *labels[SIZE][SIZE];
GtkWidget *drawing_area;
float shader_state = 0.5;
int shader_state_direaction = 1;
float shader_speed = 0.01; // Should be not to large...
int update_time = 50; //in ms 

int board[SIZE][SIZE] = {0};
int score = 0;

void update_cell(int row, int col, int value) {
    board[row][col] = value;

    char text[16];
    snprintf(text, sizeof(text), "%d", value);
    gtk_label_set_text(GTK_LABEL(labels[row][col]), text);
        GtkWidget *frame = gtk_widget_get_parent(labels[row][col]);
    gtk_widget_remove_css_class(frame, "two");
    gtk_widget_remove_css_class(frame, "four");
    gtk_widget_remove_css_class(frame, "eight");
    gtk_widget_remove_css_class(frame, "sixteen");
    gtk_widget_remove_css_class(frame, "thirtytwo");
    gtk_widget_remove_css_class(frame, "sixtyfour");
    gtk_widget_remove_css_class(frame, "hundredtwentyeitht");

    if (value == 2)
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
}

void spawn_number() {
    int rand_x_numb = rand() % SIZE;
    int rand_Y_numb = rand() % SIZE;

    while (board[rand_x_numb][rand_Y_numb] != 0) {
        rand_x_numb = rand() % SIZE;
        rand_Y_numb = rand() % SIZE;
    }
    update_cell(rand_x_numb, rand_Y_numb, 2);
}

void move_to_left() {
    int move = 0;
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
}

void moveright() {
    int move = 0;
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
}

void move_upwards() {
    int move = 0;

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
}

void move_down() {
    int move = 0;

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
                j++;
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
    
    // update_score(score);
    
}

// static void draw_function(GtkDrawingArea *area, cairo_t *cr, int width, int height, gpointer data)
// {
//     cairo_set_source_rgb(cr, 0, 0, 0);
//     cairo_paint(cr);

//     double mouse_y = 0.9;
//     if (data) mouse_y = *(double*)data;

//     double a = mouse_y * 3.1415;
//     double nx = sin(a);
//     double ny = cos(a);

//     for (int px = 0; px < width; px++) {
//         for (int py = 0; py < height; py++) {

//             double uvx = ((double)px / width) * 2.0 - 1.0;
//             double uvy = ((double)py / height) * 2.0 - 1.0;

//             uvx *= 1.1;
//             uvy *= 1.1;

//             double px2 = fabs(uvx);
//             double py2 = fabs(uvy);

//             double d = ( (px2 - 0.5) * nx + (py2 - 0.5) * ny );  // dot(p - .5, n)

//             d = fmin(d, px2);
//             d = fmax(d, -py2);
//             d = fabs(d);

//             double edge = 0.01;           
//             double intensity = (d < 0.005) ? 1.0 : 0.0;

//             double r = 0, g = 0, b = 0;

//             if (fmax(px2, py2) > 0.5)
//                 r += 0.8;

//             r += intensity;
//             g += intensity;
//             b += intensity;

//             cairo_set_source_rgb(cr, r, g, b);
//             cairo_rectangle(cr, px, py, 1, 1);
//             cairo_fill(cr);
//         }
//     }
// }

static void draw_function(GtkDrawingArea *area, cairo_t *cr, int width, int height, gpointer data)
{
    cairo_set_source_rgb(cr, 0, 0, 0);
    cairo_paint(cr);

    int canvas_size = 400;
    canvas_size = width;

    int point1x;
    int point2x;
    int point1y;
    int point2y;
    
    if (shader_state <= 0.5) {
        point1x = shader_state * canvas_size;
        point2x = canvas_size - shader_state * canvas_size;
        point1y = 0.5 * canvas_size;
        point2y = 0.5 * canvas_size;
    }
    else if (shader_state > 0.5) {
        point1x = 0.5 * canvas_size;
        point2x = 0.5 * canvas_size;
        point1y = canvas_size - shader_state * canvas_size;
        point2y = shader_state * canvas_size;
    }

    cairo_set_source_rgb (cr, 1, 1, 1);
    cairo_move_to (cr, point1x, point1y);
    cairo_line_to (cr, point2x, point2y);
    if (shader_state <= 0.5) {
        cairo_move_to (cr, 0, 0);
        cairo_line_to (cr, point1x, point1y);
        cairo_move_to (cr, 0, canvas_size);
        cairo_line_to (cr, point1x, point1y);
        cairo_move_to (cr, canvas_size, 0);
        cairo_line_to (cr, point2x, point2y);
        cairo_move_to (cr, canvas_size, canvas_size);
        cairo_line_to (cr, point2x, point2y);
    }
    else{
        cairo_move_to (cr, 0, 0);
        cairo_line_to (cr, point1x, point1y);
        cairo_move_to (cr, 0, canvas_size);
        cairo_line_to (cr, point2x, point2y);
        cairo_move_to (cr, canvas_size, 0);
        cairo_line_to (cr, point1x, point1y);
        cairo_move_to (cr, canvas_size, canvas_size);
        cairo_line_to (cr, point2x, point2y);
    }
    cairo_set_line_width (cr, 1);

    cairo_stroke(cr);
}

gboolean update_shaderstate(gpointer data) {
    shader_state += shader_speed * shader_state_direaction;
    if (shader_state <= 0) {
        shader_state_direaction = -shader_state_direaction;
    } 
    else if (shader_state >= 1) {
        shader_state_direaction = -shader_state_direaction;
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

int is_game_over() {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (board[i][j] == 0) {
                return 0;
            }
            if (i < SIZE - 1 && board[i][j] == board[i + 1][j]) {
                return 0;
            }
            if (j < SIZE - 1 && board[i][j] == board[i][j + 1]) {
                return 0;
            }
        }
    }
    return 1;  // Game over
}

gboolean on_key_pressed(GtkEventControllerKey *controller, guint keyval, guint keycode, GdkModifierType state, gpointer user_data) {
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

void activate(GtkApplication *app, gpointer data) {
    GtkWidget *window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "2048");
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 750);
    gtk_widget_set_focusable(GTK_WIDGET(window), TRUE);
    gtk_widget_grab_focus(GTK_WIDGET(window));

    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 6);
    gtk_window_set_child(GTK_WINDOW(window), vbox);


    // GtkWidget *overlay = gtk_overlay_new();
    // gtk_widget_set_hexpand(overlay, TRUE);
    // gtk_widget_set_vexpand(overlay, TRUE);

    GtkWidget *scoreboard = gtk_label_new("Score: 0");
    gtk_widget_add_css_class(scoreboard, "score-label");
    gtk_widget_set_halign(scoreboard, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(scoreboard, GTK_ALIGN_CENTER);
    gtk_widget_set_margin_top(scoreboard, 10);
    gtk_widget_set_margin_end(scoreboard, 10);
    gtk_widget_set_hexpand(scoreboard, FALSE);
    gtk_widget_set_vexpand(scoreboard, FALSE);
    gtk_widget_add_css_class(scoreboard, "infobox");
    gtk_box_append(GTK_BOX(vbox), scoreboard);

    drawing_area = gtk_drawing_area_new ();
    gtk_drawing_area_set_content_width (GTK_DRAWING_AREA (drawing_area), 400);
    gtk_drawing_area_set_content_height (GTK_DRAWING_AREA (drawing_area), 400);
    gtk_drawing_area_set_draw_func (GTK_DRAWING_AREA (drawing_area), draw_function, NULL, NULL);
    gtk_widget_set_hexpand(drawing_area, FALSE);
    gtk_widget_set_vexpand(drawing_area, FALSE);
    gtk_box_append(GTK_BOX(vbox), drawing_area);

    g_timeout_add(update_time, update_shaderstate, NULL);

    GtkWidget *container = gtk_aspect_frame_new(0.5, 0.5, 1.0, TRUE);
    gtk_box_append(GTK_BOX(vbox), container);


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
    ".two { background-color: #ffee00ff;  }"
    ".four { background-color: #ffd000ff; }"
    ".eight { background-color: #ffbb00ff; }"
    ".sixteen { background-color: #ffae00ff; }"
    ".thirtytwo { background-color: #ffa600ff; }"
    ".sixtyfour { background-color: #e49400ff }"
    ".hundredtwentyeitht { background-color: #c98200ff }"
    ".infobox { color: black; font-size 28px; font-weight: normal; border: 1px solid #000000ff; border-radius: 12px; padding: 15px; margin-right: 30px; margin-left: 30px; }"
    ".label { color: black; font-size: 28px; font-weight: bold; }");
    gtk_style_context_add_provider_for_display(
        gdk_display_get_default(),
        GTK_STYLE_PROVIDER(css_provider),
        GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    update_cell(0, 2, 0);
    spawn_number();
    // controlpadding(window, grid);

    gtk_window_present(GTK_WINDOW(window));
}

int main(int argc, char **argv) {
    GtkApplication *app;
    int status;

    app = gtk_application_new("apps.kitcat.game2048", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);

    status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);

    return status;
}
