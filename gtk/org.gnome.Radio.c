#include <config.h>
#include <gst/player/player.h>
#include <gtk/gtk.h>

static void activate(GtkApplication * app, gpointer user_data)
{
  GtkWidget *window;
  GstPlayer *player;
  window = gtk_application_window_new(app);
  gtk_window_set_application (GTK_WINDOW(window), GTK_APPLICATION(app));
  gtk_window_set_title(GTK_WINDOW(window), g_strconcat(PACKAGE, " ", VERSION, NULL));
  gtk_window_set_default_size(GTK_WINDOW(window), 800, 600);
  gtk_widget_show(window);
  player = gst_player_new (NULL, gst_player_g_main_context_signal_dispatcher_new(NULL));
  gst_player_set_uri (GST_PLAYER (player), "http://stream.live.vc.bbcmedia.co.uk/bbc_world_service");
  gst_player_play (GST_PLAYER (player));
}

int main(int argc, char **argv)
{
  GtkApplication *app;
  int status;

  gst_init(&argc, &argv);
  gst_init(NULL, NULL);
	
  app = gtk_application_new("org.gnome.Radio", G_APPLICATION_FLAGS_NONE);
  g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
  status = g_application_run(G_APPLICATION(app), argc, argv);
  g_object_unref(app);

  return status;
}
