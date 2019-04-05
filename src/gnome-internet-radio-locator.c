/* $Id$
 *
 * GNOME Internet Radio Locator
 *
 * Copyright (C) 2014-2019  Aamot Software
 *
 * Author: Ole Aamot <ole@gnome.org>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <gtk/gtk.h>
#include <gst/player/player.h>
#include <champlain/champlain.h>
#include <champlain-gtk/champlain-gtk.h>
#include <clutter-gtk/clutter-gtk.h>
#include <geocode-glib/geocode-glib.h>
#include <glib/gstdio.h>
#include <glib/gi18n.h>
#include <string.h>

#include "gnome-internet-radio-locator.h"
#include "gnome-internet-radio-locator-gui.h"
#include "gnome-internet-radio-locator-markers.h"
#include "gnome-internet-radio-locator-player.h"

#define N_COLS 2
#define COL_ID 0
#define COL_NAME 1

static ChamplainPathLayer *path_layer;
static ChamplainPathLayer *path;
static gboolean destroying = FALSE;

static ChamplainView *champlain_view;
GApplication *app;
GtkWidget *search_selector;
GtkWidget *window;
GtkWidget *statusbar;
GNOMEInternetRadioLocatorData *gnome_internet_radio_locator;

GtkWidget *input;

GtkEntryCompletion *completion;

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
ChamplainMarkerLayer *layer;
ClutterActor *marker;

gchar *world_station_xml_filename, *local_station_xml_file;

extern GNOMEInternetRadioLocatorStationInfo *stationinfo, *localstation;

extern struct GNOMEInternetRadioLocatorMedia *media;

GStatBuf stats;

ChamplainView *view;

gchar *
str_channels (GNOMEInternetRadioLocatorChannels type) {
  gchar *channels;
  if (type == GNOME_INTERNET_RADIO_LOCATOR_CHANNELS_MONO) {
    channels = g_strdup("Mono");
  }
  if (type == GNOME_INTERNET_RADIO_LOCATOR_CHANNELS_STEREO) {
    channels = g_strdup("Stereo");
  }
  if (type == GNOME_INTERNET_RADIO_LOCATOR_CHANNELS_5_1) {
    channels = g_strdup("Surround");
  }
  if (type == GNOME_INTERNET_RADIO_LOCATOR_CHANNELS_NONE) {
    channels = g_strdup("None");
  }
  return channels;
}

/*
 * Terminate the main loop.
 */
static void
on_destroy (GtkWidget *widget, gpointer data)
{
	destroying = TRUE;
	gtk_main_quit ();
}


static void
toggle_layer (GtkToggleButton *widget,
	      ClutterActor *layer)
{
	if (gtk_toggle_button_get_active (widget))
	{
		champlain_path_layer_set_visible (path_layer, TRUE);
		champlain_path_layer_set_visible (path, TRUE);
		champlain_marker_layer_animate_in_all_markers (CHAMPLAIN_MARKER_LAYER (layer));
	}
	else
	{
		champlain_path_layer_set_visible (path_layer, FALSE);
		champlain_path_layer_set_visible (path, FALSE);
		champlain_marker_layer_animate_out_all_markers (CHAMPLAIN_MARKER_LAYER (layer));
	}
}

static gboolean
mouse_click_cb (ClutterActor *actor, ClutterButtonEvent *event, ChamplainView *view)
{
	GError **error;
	gdouble lat, lon;
	GeocodePlace *place_city, *place_country;
	GeocodeLocation *location_city;
	GeocodeLocation *location_country;
	GeocodeReverse *reverse_city, *reverse_country;
	ClutterColor city_color = { 0x9a, 0x9b, 0x9c, 0x9d };
	ClutterColor text_color = { 0xff, 0xff, 0xff, 0xff };

	const char *name, *name_city, *name_country;
	/* GeocodeForward *fwd; */
	/* GList *list; */
	/* GError **err; */
	lon = champlain_view_x_to_longitude (view, event->x);
	lat = champlain_view_y_to_latitude (view, event->y);
	/* champlain_view_center_on (CHAMPLAIN_VIEW (view), lat, lon); */
	location_city = geocode_location_new (lat, lon, GEOCODE_LOCATION_ACCURACY_CITY);
	location_country = geocode_location_new (lat, lon, GEOCODE_LOCATION_ACCURACY_COUNTRY);
	reverse_city = geocode_reverse_new_for_location (location_city);
	reverse_country = geocode_reverse_new_for_location (location_country);
	place_city = geocode_reverse_resolve (reverse_city, error);
	place_country = geocode_reverse_resolve (reverse_country, error);
	name_city = geocode_place_get_town (place_city);
	name_country = geocode_place_get_country (place_country);
	if (!g_strcmp0(name_country, "United States of America")) {
		name_country = geocode_place_get_state (place_country);
	}
	marker = champlain_label_new_from_file ("icons/emblem-generic.png", NULL);
	name = g_strconcat(name_city, ", ", name_country, NULL);
	champlain_label_set_text (CHAMPLAIN_LABEL (marker), (gchar *)name);
	champlain_label_set_color (CHAMPLAIN_LABEL (marker), &city_color);
	champlain_label_set_text_color (CHAMPLAIN_LABEL (marker), &text_color);
	champlain_location_set_location (CHAMPLAIN_LOCATION (marker), lat, lon);
	if (g_strcmp0(name, NULL)) {
		champlain_marker_layer_add_marker (layer, CHAMPLAIN_MARKER (marker));
		gtk_entry_set_text(GTK_ENTRY(input),(gchar *)name);
		g_signal_connect(CHAMPLAIN_LOCATION(marker), "button-press", G_CALLBACK(marker_function), NULL);
	}
	GNOME_INTERNET_RADIO_LOCATOR_DEBUG_MSG("Mouse click at: %f %f (%s)\n", lat, lon, name);
	return TRUE;
}


static void
map_source_changed (GtkWidget *widget,
		    ChamplainView *view)
{
	gchar *id;
	ChamplainMapSource *source;
	GtkTreeIter iter;
	GtkTreeModel *model;

	if (!gtk_combo_box_get_active_iter (GTK_COMBO_BOX (widget), &iter))
		return;

	model = gtk_combo_box_get_model (GTK_COMBO_BOX (widget));

	gtk_tree_model_get (model, &iter, COL_ID, &id, -1);

	ChamplainMapSourceFactory *factory = champlain_map_source_factory_dup_default ();
	source = champlain_map_source_factory_create_cached_source (factory, id);
	g_object_set (G_OBJECT (view), "map-source", source, NULL);
	g_object_unref (factory);
}

static void
zoom_changed (GtkSpinButton *spinbutton,
	      ChamplainView *view)
{
	gint zoom = gtk_spin_button_get_value_as_int (spinbutton);

	g_object_set (G_OBJECT (view), "zoom-level", zoom, NULL);
}

static void
volume_changed (GtkSpinButton *spinbutton)
{
	gint volume = gtk_spin_button_get_value_as_int (spinbutton);

	g_object_set (G_OBJECT (player), "volume", volume, NULL);
}

static void
map_zoom_changed (ChamplainView *view,
		  GParamSpec *gobject,
		  GtkSpinButton *spinbutton)
{
	gint zoom;

	g_object_get (G_OBJECT (view), "zoom-level", &zoom, NULL);
	gtk_spin_button_set_value (spinbutton, zoom);
}


static void
view_state_changed (ChamplainView *view,
		    GParamSpec *gobject,
		    GtkImage *image)
{
	ChamplainState state;

	if (destroying)
		return;

	g_object_get (G_OBJECT (view), "state", &state, NULL);
	if (state == CHAMPLAIN_STATE_LOADING)
	{
		gtk_image_set_from_icon_name (image, "edit-find", GTK_ICON_SIZE_BUTTON);
	}
	else
	{
		gtk_image_clear (image);
	}
}


static void
zoom_in (GtkWidget *widget,
	 ChamplainView *view)
{
	champlain_view_zoom_in (view);
}


static void
zoom_out (GtkWidget *widget,
	  ChamplainView *view)
{
	champlain_view_zoom_out (view);
}


static void
toggle_wrap (GtkWidget *widget,
	     ChamplainView *view)
{
	gboolean wrap;

	wrap = champlain_view_get_horizontal_wrap (view);
	champlain_view_set_horizontal_wrap (view, !wrap);
}

static void
build_combo_box (GtkComboBox *box)
{
	ChamplainMapSourceFactory *factory;
	GSList *sources, *iter;
	GtkTreeStore *store;
	GtkTreeIter parent;
	GtkCellRenderer *cell;

	store = gtk_tree_store_new (N_COLS, G_TYPE_STRING, /* id */
				    G_TYPE_STRING, /* name */
				    -1);

	factory = champlain_map_source_factory_dup_default ();
	sources = champlain_map_source_factory_get_registered (factory);

	iter = sources;
	while (iter != NULL)
	{
		ChamplainMapSourceDesc *desc = CHAMPLAIN_MAP_SOURCE_DESC (iter->data);
		const gchar *id = champlain_map_source_desc_get_id (desc);
		const gchar *name = champlain_map_source_desc_get_name (desc);

		gtk_tree_store_append (store, &parent, NULL);
		gtk_tree_store_set (store, &parent, COL_ID, id,
				    COL_NAME, name, -1);

		iter = g_slist_next (iter);
	}

	g_slist_free (sources);
	g_object_unref (factory);

	gtk_combo_box_set_model (box, GTK_TREE_MODEL (store));

	cell = gtk_cell_renderer_text_new ();
	gtk_cell_layout_pack_start (GTK_CELL_LAYOUT (box), cell, FALSE);
	gtk_cell_layout_set_attributes (GTK_CELL_LAYOUT (box), cell,
					"text", COL_NAME, NULL);

}


static void
append_point (ChamplainPathLayer *layer, gdouble lon, gdouble lat)
{
	ChamplainCoordinate *coord;

	coord = champlain_coordinate_new_full (lon, lat);
	champlain_path_layer_add_node (layer, CHAMPLAIN_LOCATION (coord));
}

static void
add_clicked (GtkButton     *button,
	     ChamplainView *view)
{
	GtkWidget *window, *dialog, *vbox, *combo;
	GtkResponseType response;

	window = g_object_get_data (G_OBJECT (view), "window");
	dialog = gtk_dialog_new_with_buttons ("Add secondary map source",
					      GTK_WINDOW (window),
					      GTK_DIALOG_MODAL,
					      "Add",
					      GTK_RESPONSE_OK,
					      "Cancel",
					      GTK_RESPONSE_CANCEL,
					      NULL);

	combo = gtk_combo_box_new ();
	build_combo_box (GTK_COMBO_BOX (combo));
	gtk_combo_box_set_active (GTK_COMBO_BOX (combo), 0);

	vbox = gtk_dialog_get_content_area (GTK_DIALOG (dialog));
	gtk_container_add (GTK_CONTAINER (vbox), combo);

	gtk_widget_show_all (dialog);

	response = gtk_dialog_run (GTK_DIALOG (dialog));

	if (response == GTK_RESPONSE_OK)
	{
		GtkTreeModel *model;
		GtkTreeIter iter;
		ChamplainMapSource *source;
		ChamplainMapSourceFactory *factory;
		char *id;

		if (!gtk_combo_box_get_active_iter (GTK_COMBO_BOX (combo), &iter))
			return;

		model = gtk_combo_box_get_model (GTK_COMBO_BOX (combo));

		gtk_tree_model_get (model, &iter, COL_ID, &id, -1);

		factory = champlain_map_source_factory_dup_default ();
		source = champlain_map_source_factory_create_memcached_source (factory, id);

		champlain_view_add_overlay_source (view, source, 0.6 * 255);
		g_object_unref (factory);
		g_free (id);
	}

	gtk_widget_destroy (dialog);
}

static void
new_station(GSimpleAction *simple, GVariant *parameter, gpointer user_data) {
	g_print(_("New Internet Radio Station\n"));
  return;
}

static void
search_station(GSimpleAction *simple, GVariant *parameter, gpointer user_data) {
	g_print(_("Search Internet Radio Station\n"));
  return;
}

static void
listen_station(GSimpleAction *simple, GVariant *parameter, gpointer user_data) {
	gnome_internet_radio_locator_player_stop(player);
	player = gst_player_new (NULL, gst_player_g_main_context_signal_dispatcher_new(NULL));
	/* g_object_set_data(G_OBJECT(widget), "station_uri", g_value_get_string(&value)); */
	if (!g_strcmp0(gnome_internet_radio_locator->selected_station_uri, NULL)) {
		gnome_internet_radio_locator_player_new(player, _("http://fm939.wnyc.org/wnycfm"));
	} else {
		gnome_internet_radio_locator_player_new(player, gnome_internet_radio_locator->selected_station_uri);
	}
	gst_player_play(player);
	return;
}

static void
stop_station(GSimpleAction *simple, GVariant *parameter, gpointer user_data) {
	guint context_id;
	gnome_internet_radio_locator_player_stop(player);
	context_id = gtk_statusbar_get_context_id (GTK_STATUSBAR (statusbar), "Station Name");
	gtk_statusbar_pop (GTK_STATUSBAR (statusbar), GPOINTER_TO_INT (context_id));
	gtk_statusbar_push (GTK_STATUSBAR (statusbar), GPOINTER_TO_INT (context_id), "Search by city or drag/click on the zoomable map to listen to a radio broadcast");
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
	g_print(_("Previous Internet Radio Station\n"));
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
  /* gnome_internet_radio_locator_player_stop(player); */
  /* g_application_quit(app); */
	gst_player_stop(player);
	g_object_unref (player);
	gst_deinit();
	gtk_main_quit ();
	return;
}

void on_new_station_changed(GtkWidget * a, gpointer user_data)
{
	GNOMEInternetRadioLocatorStationInfo *stationinfo = NULL;
	/* GList *l = g_list_first(gnome_internet_radio_locator_stations); */
	/* stationinfo = l->data; */

	gnome_internet_radio_locator->selected_station_band =
	    g_strdup(g_object_get_data(G_OBJECT(a), "station_band"));
	GNOME_INTERNET_RADIO_LOCATOR_DEBUG_MSG("on_new_station_changed: %s\n",
	    gnome_internet_radio_locator->selected_station_band);
	gnome_internet_radio_locator->selected_station_description =
	    g_strdup(g_object_get_data(G_OBJECT(a), "station_description"));
	GNOME_INTERNET_RADIO_LOCATOR_DEBUG_MSG("on_new_station_changed: %s\n",
	    gnome_internet_radio_locator->selected_station_description);
	gnome_internet_radio_locator->selected_station_location =
	    g_strdup(g_object_get_data(G_OBJECT(a), "station_location"));
	GNOME_INTERNET_RADIO_LOCATOR_DEBUG_MSG("on_new_station_changed: %s\n",
	    gnome_internet_radio_locator->selected_station_location);
	gnome_internet_radio_locator->selected_station_name =
	    g_strdup(g_object_get_data(G_OBJECT(a), "station_name"));
	GNOME_INTERNET_RADIO_LOCATOR_DEBUG_MSG("on_new_station_changed: %s\n",
	    gnome_internet_radio_locator->selected_station_name);
	gnome_internet_radio_locator->selected_station_uri =
	    g_strdup(g_object_get_data(G_OBJECT(a), "station_uri"));
	GNOME_INTERNET_RADIO_LOCATOR_DEBUG_MSG("on_new_station_changed: %s\n",
	    gnome_internet_radio_locator->selected_station_uri);
	gnome_internet_radio_locator->selected_station_website =
	    g_strdup(g_object_get_data(G_OBJECT(a), "station_website"));
	GNOME_INTERNET_RADIO_LOCATOR_DEBUG_MSG("on_new_station_changed: %s\n",
					       gnome_internet_radio_locator->selected_station_website);

	/* appbar_send_msg(_("Selected the radio station %s in %s: %s"), */
	/* 		gnome_internet_radio_locator->selected_station_name, */
	/* 		gnome_internet_radio_locator->selected_station_location, */
	/* 		selected_station_uri, */
	/* 		gnome_internet_radio_locator->selected_station_band); */
	gnome_internet_radio_locator_station_update(stationinfo,
						    gnome_internet_radio_locator->selected_station_band,
						    gnome_internet_radio_locator->selected_station_description,
						    gnome_internet_radio_locator->selected_station_name,
						    gnome_internet_radio_locator->selected_station_location,
						    gnome_internet_radio_locator->selected_station_uri,
						    gnome_internet_radio_locator->selected_station_website);
}

void on_stations_selector_changed(GtkWidget * a, gpointer user_data)
{
	GNOMEInternetRadioLocatorStationInfo *station = NULL;

	/* if (gnome_internet_radio_locator->selected_station_uri != NULL) */
	/* 	g_free(gnome_internet_radio_locator->selected_station_uri); */

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
	/* 		selected_station_uri, */
	/* 		gnome_internet_radio_locator->selected_station_band); */

	gnome_internet_radio_locator->selected_station_name = g_strdup(g_object_get_data(G_OBJECT(a), "station_name"));
	/* gnome_internet_radio_locator_history = g_list_add(GLIST(gnome_internet_radio_locator_history), (GNOMEInternetRadioLocatorStationInfo *)station); */
	/* gnome_internet_radio_locator_helper_main(selected_station_uri); */
}

static void
gnome_internet_radio_locator_window_cb (GtkApplication *app,
		gpointer user_data)
{
	GtkWidget *widget, *grid, *toolbar, *new, *search, *listen, *stop, *prev, *stations, *next, *station, *program, *quit;
	
	window = gtk_application_window_new (app);
	widget = gtk_champlain_embed_new();
	toolbar = gtk_toolbar_new();
	input = gtk_entry_new();

#if 0
	search = gtk_tool_button_new(gtk_image_new_from_icon_name(NULL, GTK_ICON_SIZE_BUTTON), _("Search"));
	gtk_tool_item_set_is_important(GTK_TOOL_ITEM(search), TRUE);
	gtk_toolbar_insert (GTK_TOOLBAR (toolbar), GTK_TOOL_ITEM(search), 1);
	gtk_widget_show (GTK_WIDGET(search));
	gtk_tool_item_set_tooltip_text (GTK_TOOL_ITEM(search), _("Search Internet Radio Station"));
	g_signal_connect(search, "clicked", G_CALLBACK (search_station), GTK_WINDOW (window));
#endif

#if 0
	prev = gtk_tool_button_new(gtk_image_new_from_icon_name(NULL, GTK_ICON_SIZE_BUTTON), _("Prev"));
	gtk_tool_item_set_is_important(GTK_TOOL_ITEM(prev), TRUE);
	gtk_toolbar_insert (GTK_TOOLBAR (toolbar), GTK_TOOL_ITEM(prev), 5);
	gtk_widget_show (GTK_WIDGET(prev));
	gtk_tool_item_set_tooltip_text (GTK_TOOL_ITEM(prev), _("Prev Internet Radio Station"));
	g_signal_connect(prev, "clicked", G_CALLBACK (prev_station), GTK_WINDOW (window));

	stations = gtk_tool_button_new(gtk_image_new_from_icon_name(NULL, GTK_ICON_SIZE_BUTTON), _("Stations"));
	gtk_tool_item_set_is_important(GTK_TOOL_ITEM(stations), TRUE);
	gtk_toolbar_insert (GTK_TOOLBAR (toolbar), GTK_TOOL_ITEM(stations), 6);
	gtk_widget_show (GTK_WIDGET(stations));
	gtk_tool_item_set_tooltip_text (GTK_TOOL_ITEM(stations), _("Stations"));
	g_signal_connect(stations, "clicked", G_CALLBACK (stations_all), GTK_WINDOW (window));

	next = gtk_tool_button_new(gtk_image_new_from_icon_name(NULL, GTK_ICON_SIZE_BUTTON), _("Next"));
	gtk_tool_item_set_is_important(GTK_TOOL_ITEM(next), TRUE);
	gtk_toolbar_insert (GTK_TOOLBAR (toolbar), GTK_TOOL_ITEM(next), 7);
	gtk_widget_show (GTK_WIDGET(next));
	gtk_tool_item_set_tooltip_text (GTK_TOOL_ITEM(next), _("Next Internet Radio Station"));
	g_signal_connect(next, "clicked", G_CALLBACK (next_station), GTK_WINDOW (window));
	
	station = gtk_tool_button_new(gtk_image_new_from_icon_name(NULL, GTK_ICON_SIZE_BUTTON), _("About Station"));
	gtk_tool_item_set_is_important(GTK_TOOL_ITEM(station), TRUE);
	gtk_toolbar_insert (GTK_TOOLBAR (toolbar), GTK_TOOL_ITEM(station), 8);
	gtk_widget_show (GTK_WIDGET(station));
	gtk_tool_item_set_tooltip_text (GTK_TOOL_ITEM(station), _("About Station"));
	g_signal_connect(station, "clicked", G_CALLBACK (about_station_cb), GTK_WINDOW (window));

	program = gtk_tool_button_new(gtk_image_new_from_icon_name(NULL, GTK_ICON_SIZE_BUTTON), _("About Program"));
	gtk_tool_item_set_is_important(GTK_TOOL_ITEM(program), TRUE);
	gtk_toolbar_insert (GTK_TOOLBAR (toolbar), GTK_TOOL_ITEM(program), 9);
	gtk_widget_show (GTK_WIDGET(program));
	gtk_tool_item_set_tooltip_text (GTK_TOOL_ITEM(program), _("About Program"));
	g_signal_connect(program, "clicked", G_CALLBACK (about_program_cb), GTK_WINDOW (window));
#endif
#if 0
	grid = gtk_grid_new();
	gtk_grid_attach (GTK_GRID(grid), GTK_WIDGET(toolbar), 0, 0, 1, 1);
	gtk_grid_attach (GTK_GRID(grid), GTK_WIDGET(widget), 0, 1, 1, 1);
        champlain_view = gtk_champlain_embed_get_view (GTK_CHAMPLAIN_EMBED(widget));
        gtk_widget_set_size_request(GTK_WIDGET(widget), 740, 580);
	gtk_container_add (GTK_CONTAINER(window), GTK_WIDGET(grid));
	g_signal_connect (window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
#endif
	gtk_window_set_title (GTK_WINDOW(window), _("GNOME Internet Radio Locator"));
	gtk_window_set_default_size (GTK_WINDOW(window), 740, 580);
	gtk_window_maximize (GTK_WINDOW (window));
	gnome_internet_radio_locator_app = create_gnome_internet_radio_locator_app();
	gtk_widget_show(gnome_internet_radio_locator_app);

	/* stations_selector = create_stations_selector(selected_station_uri, "gnome_internet_radio_locator.xml"); */

	/* g_object_add_weak_pointer(G_OBJECT(stations_selector), */
	/* 			  (void **) &(stations_selector)); */
	
	gtk_widget_show_all (window);
	
}

void on_new_station_clicked(GtkWidget *a,
			    gpointer user_data)
{
	GtkWidget *station;
	GNOMEInternetRadioLocatorStationInfo *stationinfo = NULL;
	/* GList *l = g_list_first(gnome_internet_radio_locator_stations); */
	gchar *selected_station_uri, *selected_station_band, *selected_station_description, *selected_station_name, *selected_station_location, *selected_station_website;
	/* stationinfo = l->data; */
	gint result;
	// appbar_send_msg(_("New radio station"));
	station = create_new_station_selector(user_data);
	result = gtk_dialog_run (GTK_DIALOG(station));
	switch (result)  {
	case GTK_RESPONSE_ACCEPT:
		GNOME_INTERNET_RADIO_LOCATOR_DEBUG_MSG("Squeak!\n\n");
		selected_station_band = g_strdup(g_object_get_data(G_OBJECT(station), "station_band"));
		GNOME_INTERNET_RADIO_LOCATOR_DEBUG_MSG("on_new_station_select_changed: %s\n", selected_station_band);
		selected_station_description = g_strdup(g_object_get_data(G_OBJECT(station), "station_description"));
		GNOME_INTERNET_RADIO_LOCATOR_DEBUG_MSG("on_new_station_select_changed: %s\n", selected_station_description);
		selected_station_location = g_strdup(g_object_get_data(G_OBJECT(station), "station_location"));
		GNOME_INTERNET_RADIO_LOCATOR_DEBUG_MSG("on_new_station_select_changed: %s\n", selected_station_location);
		selected_station_name = g_strdup(g_object_get_data(G_OBJECT(station), "station_name"));
		GNOME_INTERNET_RADIO_LOCATOR_DEBUG_MSG("on_new_station_select_changed: %s\n", selected_station_name);
		selected_station_uri = g_strdup(g_object_get_data(G_OBJECT(station), "station_uri"));
		GNOME_INTERNET_RADIO_LOCATOR_DEBUG_MSG("on_new_station_select_changed: %s\n", selected_station_uri);
		selected_station_website = g_strdup(g_object_get_data(G_OBJECT(station), "station_website"));
		GNOME_INTERNET_RADIO_LOCATOR_DEBUG_MSG("on_new_station_select_changed: %s\n", selected_station_website);
		gnome_internet_radio_locator_station_update (stationinfo, selected_station_band, selected_station_description, selected_station_name, selected_station_location, selected_station_uri, selected_station_website);
		break;
	default:
		g_print (_("Nothing\n\n"));
		break;
	}
	gtk_widget_destroy(station);
	/* gtk_widget_show(station); */
}

#if 0
static gboolean
on_location_matches(GtkEntryCompletion *widget,
		    GtkTreeModel *model,
		    GtkTreeIter *iter,
		    gpointer user_data)
{
	GValue value = {0, };
	gtk_tree_model_get_value(model, iter, STATION_LOCATION, &value);
	gnome_internet_radio_locator->selected_station_location = g_strdup(g_value_get_string(&value));
	g_value_unset(&value);

	/* appbar_send_msg(_("Found location %s"), */
	/* 		gnome_internet_radio_locator->selected_station_location); */
	/* gnome_internet_radio_locator_helper_run(selected_station_uri, */
	/* 		gnome_internet_radio_locator->selected_station_name, */
	/* 		GNOME_INTERNET_RADIO_LOCATOR_STREAM_SHOUTCAST, */
	/* 		GNOME_INTERNET_RADIO_LOCATOR_STREAM_PLAYER); */
	return FALSE;
}
#endif

gboolean
on_search_matches(GtkEntryCompletion *widget,
		  GtkTreeModel *model,
		  GtkTreeIter *iter,
		  gpointer user_data)
{
	/* GeocodeNominatim *geocode_nominatim; */
	GeocodePlace *place;
	GeocodeLocation *geocode_location;

	GeocodePlace *place_city, *place_country;
	GeocodeLocation *location_city;
	GeocodeLocation *location_country;
	GeocodeReverse *reverse_city, *reverse_country;

	glong lat, lon;
	GValue city = {0, };
	GValue value = {0, };
	GValue station_name = {0, };
	gchar *location;
	gchar *town;
	gchar *country;
	gchar *state;
	GError **err;
	guint context_id;

	gtk_tree_model_get_value(model, iter, STATION_LOCATION, &city);
	gtk_tree_model_get_value(model, iter, STATION_URI, &value);
	gtk_tree_model_get_value(model, iter, STATION_NAME, &station_name);
/* 	g_print ("on_search_matches: %s\n", (gchar *)g_value_get_string(&city)); */
/* 	location = (gchar *)g_value_get_string(&city); */
/* 	town = strtok(location, ", "); */
/* 	country = strtok(NULL, " "); */
/* /\* Handle U.S. states */
/* 	geocode_place_set_country (place, "United States of America"); */
/* 	country = geocode_place_get_state (country); */
/* *\/ */
/* 	place = geocode_place_new((gchar *)g_value_get_string(&station_name), GEOCODE_PLACE_TYPE_MISCELLANEOUS); */

/* 	g_print ("geocode_place_new:town: %s\n", geocode_place_get_town(place)); */
/* 	g_print ("geocode_place_new:country: %s\n", geocode_place_get_country(place)); */

/* 	geocode_place_set_town (place, town); */
/* 	geocode_place_set_country(place, country); */

/* 	geocode_nominatim = geocode_nominatim_new ("https://nominatim.gnome.org/", "ole@gnome.org"); */

/* 	g_print ("geocode_place_get_town: %s\n", geocode_place_get_town(place)); */
/* 	g_print ("geocode_place_get_country: %s\n", geocode_place_get_country(place)); */
/* 	/\* g_print ("%s\n", geocode_nominatim_get_city(geocode_nominatim)); *\/ */

/* 	reverse_city = geocode_reverse_new_for_location(geocode_location); */
/* 	place = geocode_reverse_resolve(reverse_city, err); */

/* 	geocode_location = geocode_place_get_location(place); */
/* 	g_print ("%f7.3\n", geocode_location_get_latitude(geocode_location)); */
/* 	lat = geocode_location_get_latitude(geocode_location); */
/* 	lon = geocode_location_get_longitude(geocode_location); */
/* 	g_print ("lat: %ld\n", lat); */
/* 	g_print ("lon: %ld\n", lon); */

/*
	location_city = geocode_location_new (lat, lon, GEOCODE_LOCATION_ACCURACY_CITY);
	location_country = geocode_location_new (lat, lon, GEOCODE_LOCATION_ACCURACY_COUNTRY);
	reverse_city = geocode_reverse_new_for_location (location_city);
	reverse_country = geocode_reverse_new_for_location (location_country);
	place_city = geocode_reverse_resolve (reverse_city, error);
	place_country = geocode_reverse_resolve (reverse_country, error);
	name_city = geocode_place_get_town (place_city);
	name_country = geocode_place_get_country (place_country);
*/
	/* champlain_view_center_on (CHAMPLAIN_VIEW (view),lat,lon); */
	gnome_internet_radio_locator_player_stop(player);
	player = gst_player_new (NULL, gst_player_g_main_context_signal_dispatcher_new(NULL));
	/* g_object_set_data(G_OBJECT(widget), "station_uri", g_value_get_string(&value)); */
	gnome_internet_radio_locator_player_new(player, g_value_get_string(&value));
	stationinfo = gnome_internet_radio_locator_station_load_from_file(localstation, world_station_xml_filename);
	while (stationinfo != NULL) {
	        if (strcasecmp(stationinfo->stream->uri, g_value_get_string(&value))==0) {
			gchar *statusmsg = g_strconcat(stationinfo->name, " (", stationinfo->uri, ") in ", stationinfo->location, " (", stationinfo->band, ", ", g_strdup_printf("%li", stationinfo->stream->samplerate), " Hz, ", g_strdup_printf("%li", stationinfo->stream->bitrate), " kbps)", NULL);
			context_id = gtk_statusbar_get_context_id (GTK_STATUSBAR (statusbar), "Station Name");
			gtk_statusbar_pop (GTK_STATUSBAR (statusbar), GPOINTER_TO_INT (context_id));
			gtk_statusbar_push (GTK_STATUSBAR (statusbar), GPOINTER_TO_INT (context_id), statusmsg);
		}
		stationinfo = stationinfo->next;
	}
	gst_player_play(player);
	return FALSE;
}

int
main (int argc,
      char **argv)
{
	GtkWidget *window;
	GtkWidget *widget, *vbox, *bbox, *button, *viewport, *image;
	ClutterActor *scale;
	ChamplainLicense *license_actor;
	GtkListStore *model;
	GtkTreeIter iter;
	GNOMEInternetRadioLocatorStationInfo *stationinfo, *localstation;
	guint context_id;
	bindtextdomain (GETTEXT_PACKAGE, GNOME_INTERNET_RADIO_LOCATOR_LOCALEDIR);
	bind_textdomain_codeset (GETTEXT_PACKAGE, "UTF-8");
	textdomain (GETTEXT_PACKAGE);
	if (gtk_clutter_init (&argc, &argv) != CLUTTER_INIT_SUCCESS)
		return 1;
	window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	/* give the window a 10px wide border */
	gtk_container_set_border_width (GTK_CONTAINER (window), 10);
	/* give it the title */
	gtk_window_set_title (GTK_WINDOW (window), _("GNOME Internet Radio Locator"));
	/* Connect the destroy event of the window with our on_destroy function
	 * When the window is about to be destroyed we get a notificaiton and
	 * stop the main GTK loop
	 */
	g_signal_connect (G_OBJECT (window), "destroy", G_CALLBACK (on_destroy),
			  NULL);

	vbox = gtk_box_new (GTK_ORIENTATION_VERTICAL, 10);

	widget = gtk_champlain_embed_new ();
	view = gtk_champlain_embed_get_view (GTK_CHAMPLAIN_EMBED (widget));
	clutter_actor_set_reactive (CLUTTER_ACTOR (view), TRUE);
	g_signal_connect (view, "button-release-event", G_CALLBACK (mouse_click_cb), view);

	g_object_set (G_OBJECT (view),
		      "kinetic-mode", TRUE,
		      "zoom-level", 3,
		      NULL);

	g_object_set_data (G_OBJECT (view), "window", window);

	scale = champlain_scale_new ();
	champlain_scale_connect_view (CHAMPLAIN_SCALE (scale), view);
	/* champlain_view_ensure_visible(G_OBJECT (view), NULL, TRUE); */
	/* champlain_view_set_keep_center_on_resize(G_OBJECT (view), TRUE); */
	/* align to the bottom left */
	clutter_actor_set_x_expand (scale, TRUE);
	clutter_actor_set_y_expand (scale, TRUE);
	clutter_actor_set_x_align (scale, CLUTTER_ACTOR_ALIGN_START);

	clutter_actor_set_y_align (scale, CLUTTER_ACTOR_ALIGN_END);
	clutter_actor_add_child (CLUTTER_ACTOR (view), scale);

	license_actor = champlain_view_get_license_actor (view);
	champlain_license_set_extra_text (license_actor, "Free Internet Radio");
	champlain_view_center_on (CHAMPLAIN_VIEW (view), 42.3617430, -71.0839082);
	layer = create_marker_layer (view, &path);
	champlain_view_add_layer (view, CHAMPLAIN_LAYER (path));
	champlain_view_add_layer (view, CHAMPLAIN_LAYER (layer));

	path_layer = champlain_path_layer_new ();
	/* Cheap approx of Highway 10 */
	append_point (path_layer, 45.4095, -73.3197);
	append_point (path_layer, 45.4104, -73.2846);
	append_point (path_layer, 45.4178, -73.2239);
	append_point (path_layer, 45.4176, -73.2181);
	append_point (path_layer, 45.4151, -73.2126);
	append_point (path_layer, 45.4016, -73.1926);
	append_point (path_layer, 45.3994, -73.1877);
	append_point (path_layer, 45.4000, -73.1815);
	append_point (path_layer, 45.4151, -73.1218);
	champlain_view_add_layer (view, CHAMPLAIN_LAYER (path_layer));

	gtk_widget_set_size_request (widget, 640, 520);

	bbox = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 10);

	button = gtk_button_new();
	image = gtk_image_new_from_icon_name("media-playback-start", GTK_ICON_SIZE_BUTTON);
	gtk_button_set_image (GTK_BUTTON (button), image);
	gtk_button_set_label (GTK_BUTTON (button), "New");
	g_signal_connect(button, "clicked", G_CALLBACK (on_new_station_clicked), view);
	gtk_container_add (GTK_CONTAINER (bbox), button);

	memset(&stats, 0, sizeof(stats));

	input = gtk_entry_new();

	completion = gtk_entry_completion_new();
	gtk_entry_completion_set_text_column(completion, STATION_NAME);
	gtk_entry_completion_set_text_column(completion, STATION_LOCATION);
	gtk_entry_set_completion(GTK_ENTRY(input), completion);
	g_signal_connect(G_OBJECT(completion), "match-selected",
			 G_CALLBACK(on_search_matches), NULL);
	model = gtk_list_store_new(11, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING);
	world_station_xml_filename = g_strconcat(GNOME_INTERNET_RADIO_LOCATOR_DATADIR, "/gnome-internet-radio-locator.xml", NULL);
	GNOME_INTERNET_RADIO_LOCATOR_DEBUG_MSG("world_station_xml_filename = %s\n",
	    world_station_xml_filename);

	if (world_station_xml_filename == NULL) {
		g_warning(("Failed to open %s.  Please install it.\n"),
			  world_station_xml_filename);
	}

	local_station_xml_file =
	    g_strconcat(g_get_home_dir(), "/.gnome-internet-radio-locator/gnome-internet-radio-locator.xml", NULL);

	if (!g_stat(local_station_xml_file, &stats)) {
		localstation = gnome_internet_radio_locator_station_load_from_file(NULL, local_station_xml_file);
	} else {
		localstation = NULL;
	}

	if (localstation == NULL) {
		printf(_("Failed to open %s\n"), local_station_xml_file);
	}

/*   g_free (local_station_xml_file); */

	stationinfo = gnome_internet_radio_locator_station_load_from_file(localstation, world_station_xml_filename);

	gnome_internet_radio_locator_stations = NULL;

	while (stationinfo != NULL) {

		gtk_list_store_append(model, &iter);
		gtk_list_store_set(model,
				   &iter,
				   STATION_NAME,
				   stationinfo->name,
				   STATION_LOCATION,
				   stationinfo->location,
				   STATION_URI,
				   stationinfo->stream->uri,
				   STATION_DESCRIPTION,
				   stationinfo->description,
				   STATION_FREQUENCY,
				   stationinfo->frequency,
				   STATION_BAND,
				   stationinfo->band,
				   STATION_TYPE,
				   stationinfo->type,
				   STATION_RANK,
				   stationinfo->rank,
				   STATION_BITRATE,
				   stationinfo->bitrate,
				   STATION_SAMPLERATE,
				   stationinfo->samplerate,
				   STATION_ID,
				   stationinfo->id,
				   -1);

		stationinfo = stationinfo->next;
	}

	gtk_entry_completion_set_model(completion, GTK_TREE_MODEL(model));

	gtk_widget_show(input);

	gtk_container_add (GTK_CONTAINER (bbox), input);

#if 0
	button = gtk_button_new();
	image = gtk_image_new_from_icon_name("media-playback-start", GTK_ICON_SIZE_BUTTON);
	gtk_button_set_image (GTK_BUTTON (button), image);
	gtk_button_set_label (GTK_BUTTON (button), "Listen");
	g_signal_connect(button, "clicked", G_CALLBACK (listen_station), view);
	gtk_container_add (GTK_CONTAINER (bbox), button);
#endif

	button = gtk_button_new();
	image = gtk_image_new_from_icon_name("media-playback-stop", GTK_ICON_SIZE_BUTTON);
	gtk_button_set_image (GTK_BUTTON (button), image);
	gtk_button_set_label (GTK_BUTTON (button), _("Stop"));
	g_signal_connect(button, "clicked", G_CALLBACK (stop_station), view);
	gtk_container_add (GTK_CONTAINER (bbox), button);
	button = gtk_button_new ();
	image = gtk_image_new_from_icon_name ("zoom-in", GTK_ICON_SIZE_BUTTON);
	gtk_button_set_image (GTK_BUTTON (button), image);
	gtk_button_set_label (GTK_BUTTON (button), _("Zoom In"));
	g_signal_connect (button, "clicked", G_CALLBACK (zoom_in), view);
	gtk_container_add (GTK_CONTAINER (bbox), button);
	button = gtk_spin_button_new_with_range (0, 20, 1);
	gtk_spin_button_set_value (GTK_SPIN_BUTTON (button),
				   champlain_view_get_zoom_level (view));
	g_signal_connect (button, "changed", G_CALLBACK (zoom_changed), view);
	g_signal_connect (view, "notify::zoom-level", G_CALLBACK (map_zoom_changed),
			  button);
	gtk_container_add (GTK_CONTAINER (bbox), button);

	button = gtk_button_new ();
	image = gtk_image_new_from_icon_name ("zoom-out", GTK_ICON_SIZE_BUTTON);
	gtk_button_set_image (GTK_BUTTON (button), image);
	gtk_button_set_label (GTK_BUTTON (button), _("Zoom Out"));
	g_signal_connect (button, "clicked", G_CALLBACK (zoom_out), view);
	gtk_container_add (GTK_CONTAINER (bbox), button);

#if 0
	button = gtk_toggle_button_new_with_label (_("Markers"));
	gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (button), TRUE);
	g_signal_connect (button, "toggled", G_CALLBACK (toggle_layer), layer);
	gtk_container_add (GTK_CONTAINER (bbox), button);
#endif
	/* button = gtk_combo_box_new (); */
	/* build_combo_box (GTK_COMBO_BOX (button)); */
	/* gtk_combo_box_set_active (GTK_COMBO_BOX (button), 0); */
	/* g_signal_connect (button, "changed", G_CALLBACK (map_source_changed), view); */
	/* gtk_container_add (GTK_CONTAINER (bbox), button); */

#if 0
	button = gtk_button_new ();
	image = gtk_image_new_from_icon_name ("list-add", GTK_ICON_SIZE_BUTTON);
	gtk_button_set_image (GTK_BUTTON (button), image);

	g_signal_connect (button, "clicked", G_CALLBACK (add_clicked), view);
	gtk_container_add (GTK_CONTAINER (bbox), button);
#endif
	button = gtk_button_new();
	image = gtk_image_new_from_icon_name("stop", GTK_ICON_SIZE_BUTTON);
	gtk_button_set_image (GTK_BUTTON (button), image);
	gtk_button_set_label (GTK_BUTTON (button), _("Exit"));
	g_signal_connect(button, "clicked", G_CALLBACK(quit_program), view);
	gtk_container_add (GTK_CONTAINER (bbox), button);

	button = gtk_image_new ();
	gtk_widget_set_size_request (button, 22, -1);
	g_signal_connect (view, "notify::state", G_CALLBACK (view_state_changed),
			  button);
	gtk_box_pack_end (GTK_BOX (bbox), button, FALSE, FALSE, 0);
	viewport = gtk_frame_new (NULL);
	gtk_container_add (GTK_CONTAINER (viewport), widget);
	gtk_box_pack_start (GTK_BOX (vbox), bbox, FALSE, FALSE, 0);
	statusbar = gtk_statusbar_new ();
	context_id = gtk_statusbar_get_context_id (GTK_STATUSBAR (statusbar), "Station Name");
	gtk_statusbar_pop (GTK_STATUSBAR (statusbar), GPOINTER_TO_INT (context_id));
	gtk_statusbar_push (GTK_STATUSBAR (statusbar), GPOINTER_TO_INT (context_id), "Search by city or drag/click on the zoomable map to listen to a radio broadcast");
	gtk_box_pack_end (GTK_BOX (vbox), statusbar, FALSE, FALSE, 0);
	gtk_widget_show (statusbar);
	gtk_container_add (GTK_CONTAINER (vbox), viewport);

	/* and insert it into the main window  */
	gtk_container_add (GTK_CONTAINER (window), vbox);

	/* make sure that everything, window and label, are visible */
	gtk_widget_show_all (window);

	gst_init(&argc, &argv);

	/* start the main loop */
	gtk_main ();

	return 0;
#if 0
	
	app = gtk_application_new ("org.gnome.gnome-internet-radio-locator", G_APPLICATION_FLAGS_NONE);

	g_signal_connect (app, "activate", G_CALLBACK (gnome_internet_radio_locator_window_cb), NULL);
	
	status = g_application_run (G_APPLICATION (app), argc, argv);

	g_object_unref (app);

	return status;
#endif
}
