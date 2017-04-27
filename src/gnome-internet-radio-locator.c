/* $id$
 *
 * GNOME Internet Radio Locator
 *
 * Copyright (C) 2014, 2015, 2016, 2017  Ole Aamot Software
 *
 * Author: Ole Aamot <oka@oka.no>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include <gtk/gtk.h>
#include <champlain/champlain.h>
#include <champlain-gtk/champlain-gtk.h>
#include <clutter-gtk/clutter-gtk.h>
#include <glib/gstdio.h>
#include <string.h>

#include "gnome-internet-radio-locator.h"
#include "gnome-internet-radio-locator-gui.h"
#include "gnome-internet-radio-locator-player.h"

static ChamplainView *champlain_view;
GApplication *app;
GtkWidget *search_selector;
GtkWidget *window;
GNOMEInternetRadioLocatorData *gnome_internet_radio_locator;

GList *gnome_internet_radio_locator_archivers;
GList *gnome_internet_radio_locator_listeners;
GList *gnome_internet_radio_locator_programs;
GList *gnome_internet_radio_locator_stations;
GList *gnome_internet_radio_locator_streams;

GtkWidget *archivers_selector = NULL;
GtkWidget *listeners_selector = NULL;
GtkWidget *programs_selector = NULL;
GtkWidget *stations_selector = NULL;
GtkWidget *streams_selector = NULL;
GtkWidget *search_selector = NULL;

gchar *list_item_data_key ="list_item_data";

GtkWidget *gnome_internet_radio_locator_app;
GstPlayer *player;

extern struct GNOMEInternetRadioLocatorMedia *media;

static void
new_station(GSimpleAction *simple, GVariant *parameter, gpointer user_data) {
  g_print("New Internet Radio Station\n");
  return;
}

static void
search_station(GSimpleAction *simple, GVariant *parameter, gpointer user_data) {
  g_print("Search Internet Radio Station\n");
  return;
}

static void
listen_station(GSimpleAction *simple, GVariant *parameter, gpointer user_data) {
  player = gst_player_new (NULL, gst_player_g_main_context_signal_dispatcher_new
				      (NULL));
  gnome_internet_radio_locator_player_new(player, "http://fm939.wnyc.org/wnycfm");
  gst_player_play(player);
  return;
}

static void
stop_station(GSimpleAction *simple, GVariant *parameter, gpointer user_data) {
  gnome_internet_radio_locator_player_stop(player);
  return;
}

static void
pause_station(GSimpleAction *simple, GVariant *parameter, gpointer user_data) {
  /* FIXME: Removing Pause.  Can't quit after gst_player_pause is called. */
#if 0
  gnome_internet_radio_locator_player_pause(player);
#endif
  return;
}

static void
prev_station(GSimpleAction *simple, GVariant *parameter, gpointer user_data) {
  g_print("Previous Internet Radio Station\n");
  return;
}

static void
stations_all(GSimpleAction *simple, GVariant *parameter, gpointer user_data) {
  return;
}

static void
next_station(GSimpleAction *simple, GVariant *parameter, gpointer user_data) {
  return;
}

static void
about_station_cb(GSimpleAction *simple, GVariant *parameter, gpointer user_data) {
  return;
}

static void
about_program_cb(GSimpleAction *simple, GVariant *parameter, gpointer user_data) {
  return;
}

static void
quit_program(GSimpleAction *simple, GVariant *parameter, gpointer user_data) {
  gnome_internet_radio_locator_player_stop(player);
  g_application_quit(app);
  gst_deinit();
  return;
}

void on_new_station_changed(GtkWidget * a, gpointer user_data)
{
	GNOMEInternetRadioLocatorStationInfo *stationinfo = NULL;
	/* GList *l = g_list_first(gnome_internet_radio_locator_stations); */
	/* stationinfo = l->data; */

	/* if (gnome_internet_radio_locator->selected_station_uri != NULL) */
	/* 	g_free(gnome_internet_radio_locator->selected_station_uri); */

	gnome_internet_radio_locator->selected_station_uri =
	    g_strdup(g_object_get_data(G_OBJECT(a), "station_uri"));
	GNOME_INTERNET_RADIO_LOCATOR_DEBUG_MSG("on_new_station_changed: %s\n", gnome_internet_radio_locator->selected_station_uri);

	gnome_internet_radio_locator->selected_station_name =
	    g_strdup(g_object_get_data(G_OBJECT(a), "station_name"));
	GNOME_INTERNET_RADIO_LOCATOR_DEBUG_MSG("on_new_station_changed: %s\n",
	    gnome_internet_radio_locator->selected_station_name);

	gnome_internet_radio_locator->selected_station_location =
	    g_strdup(g_object_get_data(G_OBJECT(a), "station_location"));
	GNOME_INTERNET_RADIO_LOCATOR_DEBUG_MSG("on_new_station_changed: %s\n",
	    gnome_internet_radio_locator->selected_station_location);

	gnome_internet_radio_locator->selected_station_band =
	    g_strdup(g_object_get_data(G_OBJECT(a), "station_band"));
	GNOME_INTERNET_RADIO_LOCATOR_DEBUG_MSG("on_new_station_changed: %s\n",
	    gnome_internet_radio_locator->selected_station_band);

	gnome_internet_radio_locator->selected_station_description =
	    g_strdup(g_object_get_data(G_OBJECT(a), "station_description"));
	GNOME_INTERNET_RADIO_LOCATOR_DEBUG_MSG("on_new_station_changed: %s\n",
	    gnome_internet_radio_locator->selected_station_description);

	/* appbar_send_msg(_("Selected the radio station %s in %s: %s"), */
	/* 		gnome_internet_radio_locator->selected_station_name, */
	/* 		gnome_internet_radio_locator->selected_station_location, */
	/* 		gnome_internet_radio_locator->selected_station_uri, */
	/* 		gnome_internet_radio_locator->selected_station_band); */

	gnome_internet_radio_locator_station_update(stationinfo, gnome_internet_radio_locator->selected_station_band, gnome_internet_radio_locator->selected_station_description, gnome_internet_radio_locator->selected_station_name,
			  gnome_internet_radio_locator->selected_station_location,
			  gnome_internet_radio_locator->selected_station_uri, gnome_internet_radio_locator->selected_station_website);

	/* gnome_internet_radio_locator_helper_run(gnome_internet_radio_locator->selected_streams_uri, */
	/* 		gnome_internet_radio_locator->selected_streams_mime, */
	/* 		GNOME_INTERNET_RADIO_LOCATOR_STREAM_SHOUTCAST, */
	/* 	        GNOME_INTERNET_RADIO_LOCATOR_STREAM_PLAYER); */
}

void on_stations_selector_changed(GtkWidget * a, gpointer user_data)
{
	GNOMEInternetRadioLocatorStationInfo *station;

	if (gnome_internet_radio_locator->selected_station_uri != NULL)
		g_free(gnome_internet_radio_locator->selected_station_uri);

	gnome_internet_radio_locator->selected_station_uri = g_strdup(g_object_get_data(G_OBJECT(a), "station_uri"));

	GNOME_INTERNET_RADIO_LOCATOR_DEBUG_MSG("on_station_select_changed: %s\n", gnome_internet_radio_locator->selected_station_uri);

	gnome_internet_radio_locator->selected_station_name =
	    g_strdup(g_object_get_data(G_OBJECT(a), "station_name"));
	GNOME_INTERNET_RADIO_LOCATOR_DEBUG_MSG("on_station_select_changed: %s\n",
	    gnome_internet_radio_locator->selected_station_name);

	gnome_internet_radio_locator->selected_station_location =
	    g_strdup(g_object_get_data(G_OBJECT(a), "station_location"));
	GNOME_INTERNET_RADIO_LOCATOR_DEBUG_MSG("on_station_select_changed: %s\n",
	    gnome_internet_radio_locator->selected_station_location);

	gnome_internet_radio_locator->selected_station_band =
	    g_strdup(g_object_get_data(G_OBJECT(a), "station_band"));
	GNOME_INTERNET_RADIO_LOCATOR_DEBUG_MSG("on_station_select_changed: %s\n",
	    gnome_internet_radio_locator->selected_station_band);

	gnome_internet_radio_locator->selected_station_description =
	    g_strdup(g_object_get_data(G_OBJECT(a), "station_description"));
	GNOME_INTERNET_RADIO_LOCATOR_DEBUG_MSG("on_station_select_changed: %s\n",
	    gnome_internet_radio_locator->selected_station_description);

	/* appbar_send_msg(_("Selected the radio station %s in %s: %s"), */
	/* 		gnome_internet_radio_locator->selected_station_name, */
	/* 		gnome_internet_radio_locator->selected_station_location, */
	/* 		gnome_internet_radio_locator->selected_station_uri, */
	/* 		gnome_internet_radio_locator->selected_station_band); */

	station->name = g_strdup(g_object_get_data(G_OBJECT(a), "station_name"));
	/* gnome_internet_radio_locator_history = g_list_add(GLIST(gnome_internet_radio_locator_history), (GNOMEInternetRadioLocatorStationInfo *)station); */
	/* gnome_internet_radio_locator_helper_main(gnome_internet_radio_locator->selected_station_uri); */
}

static void
gnome_internet_radio_locator_window_cb (GtkApplication *app,
		gpointer user_data)
{
  GtkWidget *widget, *grid, *toolbar, *new, *search, *input, *listen, *stop, *prev, *stations, *next, *station, *program, *quit;
	
	window = gtk_application_window_new (app);
	widget = gtk_champlain_embed_new();
	toolbar = gtk_toolbar_new();
#if 0
	new = gtk_tool_button_new(gtk_image_new_from_icon_name(NULL, GTK_ICON_SIZE_BUTTON), "New");
	gtk_tool_item_set_is_important(GTK_TOOL_ITEM(new), TRUE);
	gtk_toolbar_insert (GTK_TOOLBAR (toolbar), GTK_TOOL_ITEM(new), 0);
	gtk_widget_show (GTK_WIDGET(new));
	gtk_tool_item_set_tooltip_text (GTK_TOOL_ITEM(new), "New Station");
	g_signal_connect(new, "clicked", G_CALLBACK (on_new_station_clicked), GTK_WINDOW (window));

	search = gtk_tool_button_new(gtk_image_new_from_icon_name(NULL, GTK_ICON_SIZE_BUTTON), "Search");
	gtk_tool_item_set_is_important(GTK_TOOL_ITEM(search), TRUE);
	gtk_toolbar_insert (GTK_TOOLBAR (toolbar), GTK_TOOL_ITEM(search), 1);
	gtk_widget_show (GTK_WIDGET(search));
	gtk_tool_item_set_tooltip_text (GTK_TOOL_ITEM(search), "Search Internet Radio Station");
	g_signal_connect(search, "clicked", G_CALLBACK (search_station), GTK_WINDOW (window));
#endif
	input = gtk_entry_new();
	gtk_toolbar_insert (GTK_TOOLBAR (toolbar), GTK_WIDGET(input), 2);
	gtk_widget_show (GTK_WIDGET(input));
	gtk_tool_item_set_tooltip_text (GTK_WIDGET(input), "WNYC");
	g_signal_connect(input, "clicked", G_CALLBACK (listen_station), GTK_WINDOW (window));
		
	listen = gtk_tool_button_new(gtk_image_new_from_icon_name(NULL, GTK_ICON_SIZE_BUTTON), "Listen");
	gtk_tool_item_set_is_important(GTK_TOOL_ITEM(listen), TRUE);
	gtk_toolbar_insert (GTK_TOOLBAR (toolbar), GTK_TOOL_ITEM(listen), 3);
	gtk_widget_show (GTK_WIDGET(listen));
	gtk_tool_item_set_tooltip_text (GTK_TOOL_ITEM(listen), "Listen Internet Radio Station");
	g_signal_connect(listen, "clicked", G_CALLBACK (listen_station), GTK_WINDOW (window));

	stop = gtk_tool_button_new(gtk_image_new_from_icon_name(NULL, GTK_ICON_SIZE_BUTTON), "Stop");
	gtk_tool_item_set_is_important(GTK_TOOL_ITEM(stop), TRUE);
	gtk_toolbar_insert (GTK_TOOLBAR (toolbar), GTK_TOOL_ITEM(stop), 4);
	gtk_widget_show (GTK_WIDGET(stop));
	gtk_tool_item_set_tooltip_text (GTK_TOOL_ITEM(stop), "Stop Internet Radio Station");
	g_signal_connect(stop, "clicked", G_CALLBACK (stop_station), GTK_WINDOW (window));

#if 0
	prev = gtk_tool_button_new(gtk_image_new_from_icon_name(NULL, GTK_ICON_SIZE_BUTTON), "Prev");
	gtk_tool_item_set_is_important(GTK_TOOL_ITEM(prev), TRUE);
	gtk_toolbar_insert (GTK_TOOLBAR (toolbar), GTK_TOOL_ITEM(prev), 5);
	gtk_widget_show (GTK_WIDGET(prev));
	gtk_tool_item_set_tooltip_text (GTK_TOOL_ITEM(prev), "Prev Internet Radio Station");
	g_signal_connect(prev, "clicked", G_CALLBACK (prev_station), GTK_WINDOW (window));

	stations = gtk_tool_button_new(gtk_image_new_from_icon_name(NULL, GTK_ICON_SIZE_BUTTON), "Stations");
	gtk_tool_item_set_is_important(GTK_TOOL_ITEM(stations), TRUE);
	gtk_toolbar_insert (GTK_TOOLBAR (toolbar), GTK_TOOL_ITEM(stations), 6);
	gtk_widget_show (GTK_WIDGET(stations));
	gtk_tool_item_set_tooltip_text (GTK_TOOL_ITEM(stations), "Stations");
	g_signal_connect(stations, "clicked", G_CALLBACK (stations_all), GTK_WINDOW (window));

	next = gtk_tool_button_new(gtk_image_new_from_icon_name(NULL, GTK_ICON_SIZE_BUTTON), "Next");
	gtk_tool_item_set_is_important(GTK_TOOL_ITEM(next), TRUE);
	gtk_toolbar_insert (GTK_TOOLBAR (toolbar), GTK_TOOL_ITEM(next), 7);
	gtk_widget_show (GTK_WIDGET(next));
	gtk_tool_item_set_tooltip_text (GTK_TOOL_ITEM(next), "Next Internet Radio Station");
	g_signal_connect(next, "clicked", G_CALLBACK (next_station), GTK_WINDOW (window));
	
	station = gtk_tool_button_new(gtk_image_new_from_icon_name(NULL, GTK_ICON_SIZE_BUTTON), "About Station");
	gtk_tool_item_set_is_important(GTK_TOOL_ITEM(station), TRUE);
	gtk_toolbar_insert (GTK_TOOLBAR (toolbar), GTK_TOOL_ITEM(station), 8);
	gtk_widget_show (GTK_WIDGET(station));
	gtk_tool_item_set_tooltip_text (GTK_TOOL_ITEM(station), "About Station");
	g_signal_connect(station, "clicked", G_CALLBACK (about_station_cb), GTK_WINDOW (window));

	program = gtk_tool_button_new(gtk_image_new_from_icon_name(NULL, GTK_ICON_SIZE_BUTTON), "About Program");
	gtk_tool_item_set_is_important(GTK_TOOL_ITEM(program), TRUE);
	gtk_toolbar_insert (GTK_TOOLBAR (toolbar), GTK_TOOL_ITEM(program), 9);
	gtk_widget_show (GTK_WIDGET(program));
	gtk_tool_item_set_tooltip_text (GTK_TOOL_ITEM(program), "About Program");
	g_signal_connect(program, "clicked", G_CALLBACK (about_program_cb), GTK_WINDOW (window));
#endif
	quit = gtk_tool_button_new(gtk_image_new_from_icon_name(NULL, GTK_ICON_SIZE_BUTTON), "Exit");
	gtk_tool_item_set_is_important(GTK_TOOL_ITEM(quit), TRUE);
	gtk_toolbar_insert (GTK_TOOLBAR (toolbar), GTK_TOOL_ITEM(quit), 10);
	gtk_widget_show (GTK_WIDGET(quit));
	gtk_tool_item_set_tooltip_text (GTK_TOOL_ITEM(quit), "Exit Program");
	g_signal_connect(quit, "clicked", G_CALLBACK (quit_program), GTK_WINDOW (window));

	grid = gtk_grid_new();
	gtk_grid_attach (GTK_GRID(grid), GTK_WIDGET(toolbar), 0, 0, 1, 1);
	gtk_grid_attach (GTK_GRID(grid), GTK_WIDGET(widget), 0, 1, 1, 1);
        champlain_view = gtk_champlain_embed_get_view (GTK_CHAMPLAIN_EMBED(widget));
        gtk_widget_set_size_request(GTK_WIDGET(widget), 1440, 720);
	gtk_container_add (GTK_CONTAINER(window), GTK_WIDGET(grid));
	g_signal_connect (window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
	gtk_window_set_title (GTK_WINDOW(window), "GNOME Internet Radio Locator");
	gtk_window_set_default_size (GTK_WINDOW(window), 1440, 720);

	gnome_internet_radio_locator_app = create_gnome_internet_radio_locator_app();
	gtk_widget_show(gnome_internet_radio_locator_app);

	/* stations_selector = create_stations_selector(gnome_internet_radio_locator->selected_station_uri, "gnome_internet_radio_locator.xml"); */

	/* g_object_add_weak_pointer(G_OBJECT(stations_selector), */
	/* 			  (void **) &(stations_selector)); */
	
	gtk_widget_show_all (window);
	
}

void on_new_station_clicked(GtkWidget *a,
			    gpointer user_data)
{
	GtkWidget *station;
	GNOMEInternetRadioLocatorStationInfo *stationinfo;
	/* GList *l = g_list_first(gnome_internet_radio_locator_stations); */

	/* stationinfo = l->data; */
	gint result;
	// appbar_send_msg(_("New radio station"));
	/* station = create_new_station_selector(); */
	result = gtk_dialog_run (GTK_DIALOG(station));
	switch (result)  {
	case GTK_RESPONSE_ACCEPT:

		GNOME_INTERNET_RADIO_LOCATOR_DEBUG_MSG("Squeak!\n\n");
		gnome_internet_radio_locator->selected_station_uri = g_strdup(g_object_get_data(G_OBJECT(station), "station_uri"));
		GNOME_INTERNET_RADIO_LOCATOR_DEBUG_MSG("on_new_station_select_changed: %s\n", gnome_internet_radio_locator->selected_station_uri);
		gnome_internet_radio_locator->selected_station_description = g_strdup(g_object_get_data(G_OBJECT(station), "station_description"));
		GNOME_INTERNET_RADIO_LOCATOR_DEBUG_MSG("on_new_station_select_changed: %s\n", gnome_internet_radio_locator->selected_station_description);
		gnome_internet_radio_locator->selected_station_band = g_strdup(g_object_get_data(G_OBJECT(station), "station_band"));
		GNOME_INTERNET_RADIO_LOCATOR_DEBUG_MSG("on_new_station_select_changed: %s\n", gnome_internet_radio_locator->selected_station_band);
		gnome_internet_radio_locator->selected_station_website = g_strdup(g_object_get_data(G_OBJECT(station), "station_website"));
		GNOME_INTERNET_RADIO_LOCATOR_DEBUG_MSG("on_new_station_select_changed: %s\n", gnome_internet_radio_locator->selected_station_website);
		gnome_internet_radio_locator->selected_station_name = g_strdup(g_object_get_data(G_OBJECT(station), "station_name"));
		GNOME_INTERNET_RADIO_LOCATOR_DEBUG_MSG("on_new_station_select_changed: %s\n",
			       gnome_internet_radio_locator->selected_station_name);
		gnome_internet_radio_locator->selected_station_location = g_strdup(g_object_get_data(G_OBJECT(station), "station_location"));
		GNOME_INTERNET_RADIO_LOCATOR_DEBUG_MSG("on_new_station_select_changed: %s\n",
			       gnome_internet_radio_locator->selected_station_location);
		gnome_internet_radio_locator_station_update (stationinfo, gnome_internet_radio_locator->selected_station_band, gnome_internet_radio_locator->selected_station_description, gnome_internet_radio_locator->selected_station_name, gnome_internet_radio_locator->selected_station_location, gnome_internet_radio_locator->selected_station_uri, gnome_internet_radio_locator->selected_station_website);
		break;
	default:
		g_print ("Nothing\n\n");
		break;
	}
	gtk_widget_destroy(station);
	/* gtk_widget_show(station); */

}

int
main (int argc,
      char **argv)
{
	int status;

	if (gtk_clutter_init (&argc, &argv) != CLUTTER_INIT_SUCCESS)
		return 1;

	gst_init(&argc, &argv);
	
	app = gtk_application_new ("org.gnome.gnome-internet-radio-locator", G_APPLICATION_FLAGS_NONE);

	g_signal_connect (app, "activate", G_CALLBACK (gnome_internet_radio_locator_window_cb), NULL);
	
	status = g_application_run (G_APPLICATION (app), argc, argv);

	g_object_unref (app);

	return status;
}

