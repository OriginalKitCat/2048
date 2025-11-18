#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include <stdlib.h>
#include <time.h>

#define SIZE 4

GtkWidget *labels[SIZE][SIZE];


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
    if (value == 2)
        gtk_widget_add_css_class(frame, "two");
    else if (value == 4)
        gtk_widget_add_css_class(frame, "four");
    else if (value == 8)
        gtk_widget_add_css_class(frame, "eight");
    else if (value == 16)
        gtk_widget_add_css_class(frame, "sixteen");
    gtk_widget_queue_draw(frame);
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
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 450);
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

    srand(time(NULL));

    app = gtk_application_new("apps.kitcat.game2048", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);

    status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);

    return status;
}

