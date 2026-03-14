#include <gtk/gtk.h>
#include <gst/gst.h>

//This is an copied file from Chatgpt to figure out how playing sounds work. In the main file everyting is human made.

// Function to play the MP3 sound using GStreamer
void play_sound(GtkWidget *widget, gpointer data) {
    const gchar *file_path = (const gchar *)data; // Get file path from the 'data' argument
    GstElement *pipeline;
    gchar *uri;

    // Convert the file path to a URI
    uri = g_strdup_printf("file://%s", file_path);

    // Initialize GStreamer pipeline to play the MP3
    pipeline = gst_parse_launch("playbin uri=NULL", NULL);
    g_object_set(pipeline, "uri", uri, NULL);

    // Start playing the sound
    gst_element_set_state(pipeline, GST_STATE_PLAYING);

    // Wait for the sound to finish (using bus)
    GstBus *bus = gst_element_get_bus(pipeline);
    GstMessage *msg = gst_bus_timed_pop_filtered(bus, GST_CLOCK_TIME_NONE, GST_MESSAGE_EOS | GST_MESSAGE_ERROR);

    // Handle errors or end-of-stream (EOS)
    if (msg != NULL) {
        if (GST_MESSAGE_TYPE(msg) == GST_MESSAGE_ERROR) {
            GError *err;
            gchar *debug;
            gst_message_parse_error(msg, &err, &debug);
            g_print("Error: %s\n", err->message);
            g_error_free(err);
            g_free(debug);
        }
        gst_message_unref(msg);
    }

    // Clean up the pipeline
    gst_object_unref(bus);
    gst_element_set_state(pipeline, GST_STATE_NULL);
    gst_object_unref(pipeline);
    g_free(uri);
}

// GTK Window callback function
static void on_activate(GtkApplication *app, gpointer user_data) {
    GtkWidget *window;
    GtkWidget *button;

    // Create a new window
    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "GTK MP3 Player");
    gtk_window_set_default_size(GTK_WINDOW(window), 200, 100);

    // Create a button to play the sound
    button = gtk_button_new_with_label("Play Sound");

    // Pass the file path to the play_sound function via user_data
    g_signal_connect(button, "clicked", G_CALLBACK(play_sound), "/home/adam/Projekte/gtk_apps/2048/sounds/pop.mp3");

    // Add the button to the window
    gtk_window_set_child(GTK_WINDOW(window), button);

    // Show the window and all its components
    gtk_window_present(GTK_WINDOW(window));
}

// Main function to run the GTK app
int main(int argc, char *argv[]) {
    GtkApplication *app;
    int status;

    // Initialize GTK and GStreamer
    gst_init(&argc, &argv);
    app = gtk_application_new("com.example.GtkMP3Player", G_APPLICATION_FLAGS_NONE);

    // Connect the "activate" signal to the on_activate function
    g_signal_connect(app, "activate", G_CALLBACK(on_activate), NULL);

    // Run the GTK application
    status = g_application_run(G_APPLICATION(app), argc, argv);

    // Clean up
    g_object_unref(app);
    return status;
}