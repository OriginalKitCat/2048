#include <gtk/gtk.h>

#define SIZE 4
GtkWidget *labels[SIZE][SIZE];

void update_board(int board[SIZE][SIZE]) {
    char text[16];
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (board[i][j] == 0)
                gtk_label_set_text(GTK_LABEL(labels[i][j]), "");
            else {
                snprintf(text, sizeof(text), "%d", board[i][j]);
                gtk_label_set_text(GTK_LABEL(labels[i][j]), text);
            }
        }
    }
}

void activate(GtkApplication *app, gpointer data) {
    GtkWidget *window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "2048");
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 400);

    GtkWidget *container = gtk_aspect_frame_new(0.5, 0.5, 1.0, TRUE);
    gtk_widget_set_hexpand(container, TRUE);
    gtk_widget_set_vexpand(container, TRUE);
    gtk_widget_set_margin_start(container, 12);
    gtk_widget_set_margin_end(container, 12);
    gtk_widget_set_margin_top(container, 12);
    gtk_widget_set_margin_bottom(container, 12);
    gtk_window_set_child(GTK_WINDOW(window), container);

    GtkWidget *grid = gtk_grid_new();
    gtk_grid_set_row_homogeneous(GTK_GRID(grid), TRUE);
    gtk_grid_set_column_homogeneous(GTK_GRID(grid), TRUE);
    gtk_widget_set_hexpand(grid, TRUE);
    gtk_widget_set_vexpand(grid, TRUE);
    gtk_aspect_frame_set_child(GTK_ASPECT_FRAME(container), grid);

    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            labels[i][j] = gtk_label_new("");
            gtk_widget_set_hexpand(labels[i][j], TRUE);
            gtk_widget_set_vexpand(labels[i][j], TRUE);
            gtk_label_set_xalign(GTK_LABEL(labels[i][j]), 0.5);
            gtk_label_set_yalign(GTK_LABEL(labels[i][j]), 0.5);

            GtkWidget *frame = gtk_frame_new(NULL);
            gtk_widget_set_hexpand(frame, TRUE);
            gtk_widget_set_vexpand(frame, TRUE);
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

