/* $id$
 *
 * GNOME Internet Radio Locator
 *
 * Copyright (C) 2019  Aamot Software
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

/*
 * Copyright (C) 2008 Pierre-Luc Beaudoin <pierre-luc@pierlux.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include <string.h>
#include <gtk/gtk.h>
#include <gst/player/player.h>
#include <champlain/champlain.h>
#include "gnome-internet-radio-locator.h"
#include "gnome-internet-radio-locator-markers.h"
#include "gnome-internet-radio-locator-player.h"

extern GtkWidget *input;
extern GtkEntryCompletion *completion;
extern GNOMEInternetRadioLocatorStationInfo *stationinfo, *localstation;
extern gchar *world_station_xml_filename;
extern GstPlayer *player;
extern ChamplainMarkerLayer *layer;
extern GtkWidget *window;
extern ChamplainView *view;

typedef struct
{
	ChamplainView *view;
	ChamplainMarker *marker;
} LocationCallbackData;

static gboolean
location_callback (LocationCallbackData *data)
{
	/* champlain_view_center_on (data->view, lat, lon); */
	/* champlain_location_set_location (CHAMPLAIN_LOCATION (data->marker), lat, lon); */
	g_print("%s\n", __FUNCTION__);
	return TRUE;
}

void
marker_function (ChamplainMarker *self,
		 gdouble          dx,
		 gdouble          dy,
		 ClutterEvent    *event,
		 gpointer         user_data)
{
        gchar *station, *station_link;
	gchar *markup;
	station = (gchar *)champlain_label_get_text (CHAMPLAIN_LABEL (self));
	station_link = strtok(station, "\n");
	gtk_entry_set_text(GTK_ENTRY(input),(gchar *)station_link);
	gst_player_stop(player);
	player = gst_player_new (NULL, gst_player_g_main_context_signal_dispatcher_new(NULL));
	stationinfo = gnome_internet_radio_locator_station_load_from_file(localstation, world_station_xml_filename);
	while (stationinfo != NULL) {
		if (strcasecmp(stationinfo->location, station_link)==0) {
			gst_player_stop(player);
			gnome_internet_radio_locator_player_new(GST_PLAYER(player), stationinfo->stream->uri);
			gst_player_play(player);
		}
		stationinfo = stationinfo->next;
	}
	if (user_data != NULL) {
		champlain_label_set_text (CHAMPLAIN_LABEL (self), user_data);
	}
	/* g_signal_connect(CHAMPLAIN_LOCATION(self), "button-press", G_CALLBACK (on_new_station_clicked), user_data); */
	return;
}

ChamplainMarkerLayer *
create_marker_layer (G_GNUC_UNUSED ChamplainView *view, ChamplainPathLayer **path)
{
  ClutterActor *marker;
  ClutterActor *layer_actor;
  ClutterColor city_color = { 0xf3, 0x94, 0x07, 0xbb };
  ClutterColor text_color = { 0xff, 0xff, 0xff, 0xff };
  LocationCallbackData callback_data;
  gchar *station = NULL;
  *path = champlain_path_layer_new ();
  layer = champlain_marker_layer_new_full (CHAMPLAIN_SELECTION_SINGLE);
  layer_actor = CLUTTER_ACTOR (layer);
  /* Create callback that updates the map periodically */
  /* callback_data.view = CHAMPLAIN_VIEW (view); */
  /* callback_data.marker = CHAMPLAIN_MARKER (layer); */
  /* g_timeout_add (1000, (GSourceFunc) location_callback, &callback_data); */

  /* marker = champlain_point_new (); */
  /* champlain_location_set_location (CHAMPLAIN_LOCATION (marker),  37.873093, -122.303769); */
  /* champlain_location_set_location (CHAMPLAIN_LOCATION (marker),  37.873093, -122.303769); */
  /* champlain_marker_layer_add_marker (layer, CHAMPLAIN_MARKER (marker)); */
  /* champlain_path_layer_add_node (*path, CHAMPLAIN_LOCATION (marker)); */
#if 0
  marker = champlain_label_new_with_text ("Norway\n<span size=\"small\">Oslo</span>", "Helvetica 14", NULL, NULL);
  champlain_label_set_use_markup (CHAMPLAIN_LABEL (marker), TRUE);
  champlain_label_set_alignment (CHAMPLAIN_LABEL (marker), PANGO_ALIGN_RIGHT);
  champlain_label_set_color (CHAMPLAIN_LABEL (marker), &city_color);
  champlain_label_set_text_color (CHAMPLAIN_LABEL (marker), &text_color);
  champlain_location_set_location (CHAMPLAIN_LOCATION (marker),  37.873093, -122.303769);
  champlain_marker_layer_add_marker (layer, CHAMPLAIN_MARKER (marker));
  champlain_path_layer_add_node (*path, CHAMPLAIN_LOCATION (marker));
#endif
  marker = champlain_label_new_from_file ("icons/emblem-generic.png", NULL);
  station = g_strdup("Ayr, Scotland\n<span size=\"small\">UWS Radio</span>");
  champlain_label_set_text (CHAMPLAIN_LABEL (marker), station);
  champlain_label_set_use_markup (CHAMPLAIN_LABEL (marker), TRUE);
  champlain_label_set_color (CHAMPLAIN_LABEL (marker), &city_color);
  champlain_label_set_text_color (CHAMPLAIN_LABEL (marker), &text_color);
  champlain_location_set_location (CHAMPLAIN_LOCATION (marker), 55.4594119, -4.6326702);
  champlain_marker_layer_add_marker (layer, CHAMPLAIN_MARKER (marker));
  champlain_marker_animate_in(CHAMPLAIN_MARKER (marker));
  /* champlain_path_layer_add_node (*path, CHAMPLAIN_LOCATION (marker)); */
  g_signal_connect(CHAMPLAIN_LOCATION(marker), "button-press", G_CALLBACK(marker_function), station);
  marker = champlain_label_new_from_file ("icons/emblem-generic.png", NULL);
  station = g_strdup("Bergen, Norway\n<span size=\"small\">SRIB</span>");
  champlain_label_set_text (CHAMPLAIN_LABEL (marker), station);
  champlain_label_set_use_markup (CHAMPLAIN_LABEL (marker), TRUE);
  champlain_label_set_color (CHAMPLAIN_LABEL (marker), &city_color);
  champlain_label_set_text_color (CHAMPLAIN_LABEL (marker), &text_color);
  champlain_location_set_location (CHAMPLAIN_LOCATION (marker), 60.3943034, 5.3258117);
  champlain_marker_layer_add_marker (layer, CHAMPLAIN_MARKER (marker));
  champlain_marker_animate_in(CHAMPLAIN_MARKER (marker));
  /* champlain_path_layer_add_node (*path, CHAMPLAIN_LOCATION (marker)); */
  g_signal_connect(CHAMPLAIN_LOCATION(marker), "button-press", G_CALLBACK(marker_function), station);
  marker = champlain_label_new_from_file ("icons/emblem-generic.png", NULL);
  station = g_strdup("Berkeley, California\n<span size=\"small\">KALX</span>");
  champlain_label_set_text (CHAMPLAIN_LABEL (marker), station);
  champlain_label_set_use_markup (CHAMPLAIN_LABEL (marker), TRUE);
  champlain_label_set_color (CHAMPLAIN_LABEL (marker), &city_color);
  champlain_label_set_text_color (CHAMPLAIN_LABEL (marker), &text_color);
  champlain_location_set_location (CHAMPLAIN_LOCATION (marker), 37.873093, -122.303769);
  champlain_marker_layer_add_marker (layer, CHAMPLAIN_MARKER (marker));
  champlain_marker_animate_in(CHAMPLAIN_MARKER (marker));
  /* champlain_path_layer_add_node (*path, CHAMPLAIN_LOCATION (marker)); */
  g_signal_connect(CHAMPLAIN_LOCATION(marker), "button-press", G_CALLBACK(marker_function), station);
  marker = champlain_label_new_from_file ("icons/emblem-generic.png", NULL);
  station = g_strdup("Boston, Massachusetts\n<span size=\"small\">WMBR</span>");
  champlain_label_set_text (CHAMPLAIN_LABEL (marker), station);
  champlain_label_set_use_markup (CHAMPLAIN_LABEL (marker), TRUE);
  champlain_label_set_color (CHAMPLAIN_LABEL (marker), &city_color);
  champlain_label_set_text_color (CHAMPLAIN_LABEL (marker), &text_color);
  champlain_location_set_location (CHAMPLAIN_LOCATION (marker), 42.3604823, -71.0595678);
  champlain_marker_layer_add_marker (layer, CHAMPLAIN_MARKER (marker));
  champlain_marker_animate_in(CHAMPLAIN_MARKER (marker));
  /* champlain_path_layer_add_node (*path, CHAMPLAIN_LOCATION (marker)); */
  g_signal_connect(CHAMPLAIN_LOCATION(marker), "button-press", G_CALLBACK(marker_function), station);
  marker = champlain_label_new_from_file ("icons/emblem-generic.png", NULL);
  station = g_strdup("Bruxelles, Belgium\n<span size=\"small\">Radio Campus</span>");
  champlain_label_set_text (CHAMPLAIN_LABEL (marker), station);
  champlain_label_set_use_markup (CHAMPLAIN_LABEL (marker), TRUE);
  champlain_label_set_color (CHAMPLAIN_LABEL (marker), &city_color);
  champlain_label_set_text_color (CHAMPLAIN_LABEL (marker), &text_color);
  champlain_location_set_location (CHAMPLAIN_LOCATION (marker), 50.84404145, 4.36720169448285);
  champlain_marker_layer_add_marker (layer, CHAMPLAIN_MARKER (marker));
  champlain_marker_animate_in(CHAMPLAIN_MARKER (marker));
  /* champlain_path_layer_add_node (*path, CHAMPLAIN_LOCATION (marker)); */
  g_signal_connect(CHAMPLAIN_LOCATION(marker), "button-press", G_CALLBACK(marker_function), station);
  marker = champlain_label_new_from_file ("icons/emblem-generic.png", NULL);
  station = g_strdup("Calgary, Canada\n<span size=\"small\">CJSW</span>");
  champlain_label_set_text (CHAMPLAIN_LABEL (marker), station);
  champlain_label_set_use_markup (CHAMPLAIN_LABEL (marker), TRUE);
  champlain_label_set_color (CHAMPLAIN_LABEL (marker), &city_color);
  champlain_label_set_text_color (CHAMPLAIN_LABEL (marker), &text_color);
  champlain_location_set_location (CHAMPLAIN_LOCATION (marker), 51.0534234, -114.0625892);
  champlain_marker_layer_add_marker (layer, CHAMPLAIN_MARKER (marker));
  champlain_marker_animate_in(CHAMPLAIN_MARKER (marker));
  /* champlain_path_layer_add_node (*path, CHAMPLAIN_LOCATION (marker)); */
  g_signal_connect(CHAMPLAIN_LOCATION(marker), "button-press", G_CALLBACK(marker_function), station);
  marker = champlain_label_new_from_file ("icons/emblem-generic.png", NULL);
  station = g_strdup("Cambridge, United Kingdom\n<span size=\"small\">Cam FM</span>");
  champlain_label_set_text (CHAMPLAIN_LABEL (marker), station);
  champlain_label_set_use_markup (CHAMPLAIN_LABEL (marker), TRUE);
  champlain_label_set_color (CHAMPLAIN_LABEL (marker), &city_color);
  champlain_label_set_text_color (CHAMPLAIN_LABEL (marker), &text_color);
  champlain_location_set_location (CHAMPLAIN_LOCATION (marker), 52.2033051, 0.124862);
  champlain_marker_layer_add_marker (layer, CHAMPLAIN_MARKER (marker));
  champlain_marker_animate_in(CHAMPLAIN_MARKER (marker));
  /* champlain_path_layer_add_node (*path, CHAMPLAIN_LOCATION (marker)); */
  g_signal_connect(CHAMPLAIN_LOCATION(marker), "button-press", G_CALLBACK(marker_function), station);
  marker = champlain_label_new_from_file ("icons/emblem-generic.png", NULL);
  station = g_strdup("Cape Town, South Africa\n<span size=\"small\">UCT Radio</span>");
  champlain_label_set_text (CHAMPLAIN_LABEL (marker), station);
  champlain_label_set_use_markup (CHAMPLAIN_LABEL (marker), TRUE);
  champlain_label_set_color (CHAMPLAIN_LABEL (marker), &city_color);
  champlain_label_set_text_color (CHAMPLAIN_LABEL (marker), &text_color);
  champlain_location_set_location (CHAMPLAIN_LOCATION (marker), -33.928992, 18.417396);
  champlain_marker_layer_add_marker (layer, CHAMPLAIN_MARKER (marker));
  champlain_marker_animate_in(CHAMPLAIN_MARKER (marker));
  /* champlain_path_layer_add_node (*path, CHAMPLAIN_LOCATION (marker)); */
  g_signal_connect(CHAMPLAIN_LOCATION(marker), "button-press", G_CALLBACK(marker_function), station);
  marker = champlain_label_new_from_file ("icons/emblem-generic.png", NULL);
  station = g_strdup("Copenhagen, Denmark\n<span size=\"small\">Danmarks Radio Nyheder</span>");
  champlain_label_set_text (CHAMPLAIN_LABEL (marker), station);
  champlain_label_set_use_markup (CHAMPLAIN_LABEL (marker), TRUE);
  champlain_label_set_color (CHAMPLAIN_LABEL (marker), &city_color);
  champlain_label_set_text_color (CHAMPLAIN_LABEL (marker), &text_color);
  champlain_location_set_location (CHAMPLAIN_LOCATION (marker), 55.6867243, 12.5700724);
  champlain_marker_layer_add_marker (layer, CHAMPLAIN_MARKER (marker));
  champlain_marker_animate_in(CHAMPLAIN_MARKER (marker));
  /* champlain_path_layer_add_node (*path, CHAMPLAIN_LOCATION (marker)); */
  g_signal_connect(CHAMPLAIN_LOCATION(marker), "button-press", G_CALLBACK(marker_function), station);
  marker = champlain_label_new_from_file ("icons/emblem-generic.png", NULL);
  station = g_strdup("Chicago, Illinois\n<span size=\"small\">WHPK</span>");
  champlain_label_set_text (CHAMPLAIN_LABEL (marker), station);
  champlain_label_set_use_markup (CHAMPLAIN_LABEL (marker), TRUE);
  champlain_label_set_color (CHAMPLAIN_LABEL (marker), &city_color);
  champlain_label_set_text_color (CHAMPLAIN_LABEL (marker), &text_color);
  champlain_location_set_location (CHAMPLAIN_LOCATION (marker), 41.8755546, -87.6244212);
  champlain_marker_layer_add_marker (layer, CHAMPLAIN_MARKER (marker));
  champlain_marker_animate_in(CHAMPLAIN_MARKER (marker));
  /* champlain_path_layer_add_node (*path, CHAMPLAIN_LOCATION (marker)); */
  g_signal_connect(CHAMPLAIN_LOCATION(marker), "button-press", G_CALLBACK(marker_function), station);
#if 0 /* Non-streamble as of 2018/03/26 */
  marker = champlain_label_new_from_file ("icons/emblem-generic.png", NULL);
  station = g_strdup("Dublin, Ireland\n<span size=\"small\">UWS Radio</span>");
  champlain_label_set_text (CHAMPLAIN_LABEL (marker), station);
  champlain_label_set_use_markup (CHAMPLAIN_LABEL (marker), TRUE);
  champlain_label_set_color (CHAMPLAIN_LABEL (marker), &city_color);
  champlain_label_set_text_color (CHAMPLAIN_LABEL (marker), &text_color);
  champlain_location_set_location (CHAMPLAIN_LOCATION (marker), 53.3497645, -6.2602732);
  champlain_marker_layer_add_marker (layer, CHAMPLAIN_MARKER (marker));
  /* champlain_path_layer_add_node (*path, CHAMPLAIN_LOCATION (marker)); */
  g_signal_connect(CHAMPLAIN_LOCATION(marker), "button-press", G_CALLBACK(marker_function), station);
#endif
  marker = champlain_label_new_from_file ("icons/emblem-generic.png", NULL);
  station = g_strdup("Guatemala City, Guatemala\n<span size=\"small\">Radio Universidad</span>");
  champlain_label_set_text (CHAMPLAIN_LABEL (marker), station);
  champlain_label_set_use_markup (CHAMPLAIN_LABEL (marker), TRUE);
  champlain_label_set_color (CHAMPLAIN_LABEL (marker), &city_color);
  champlain_label_set_text_color (CHAMPLAIN_LABEL (marker), &text_color);
  champlain_location_set_location (CHAMPLAIN_LOCATION (marker), 14.6417889, -90.5132239);
  champlain_marker_layer_add_marker (layer, CHAMPLAIN_MARKER (marker));
  champlain_marker_animate_in(CHAMPLAIN_MARKER (marker));
  /* champlain_path_layer_add_node (*path, CHAMPLAIN_LOCATION (marker)); */
  g_signal_connect(CHAMPLAIN_LOCATION(marker), "button-press", G_CALLBACK(marker_function), station);
  marker = champlain_label_new_from_file ("icons/emblem-generic.png", NULL);
  station = g_strdup("Honolulu, Hawaii\n<span size=\"small\">Hawaii Public Radio</span>");
  champlain_label_set_text (CHAMPLAIN_LABEL (marker), station);
  champlain_label_set_use_markup (CHAMPLAIN_LABEL (marker), TRUE);
  champlain_label_set_color (CHAMPLAIN_LABEL (marker), &city_color);
  champlain_label_set_text_color (CHAMPLAIN_LABEL (marker), &text_color);
  champlain_location_set_location (CHAMPLAIN_LOCATION (marker), 21.304547, -157.8556764);
  champlain_marker_layer_add_marker (layer, CHAMPLAIN_MARKER (marker));
  champlain_marker_animate_in(CHAMPLAIN_MARKER (marker));
  /* champlain_path_layer_add_node (*path, CHAMPLAIN_LOCATION (marker)); */
  g_signal_connect(CHAMPLAIN_LOCATION(marker), "button-press", G_CALLBACK(marker_function), station);
  marker = champlain_label_new_from_file ("icons/emblem-generic.png", NULL);
  station = g_strdup("Kingston, Canada\n<span size=\"small\">CFRC</span>");
  champlain_label_set_text (CHAMPLAIN_LABEL (marker), station);
  champlain_label_set_use_markup (CHAMPLAIN_LABEL (marker), TRUE);
  champlain_label_set_color (CHAMPLAIN_LABEL (marker), &city_color);
  champlain_label_set_text_color (CHAMPLAIN_LABEL (marker), &text_color);
  champlain_location_set_location (CHAMPLAIN_LOCATION (marker), 44.230687, -76.481323);
  champlain_marker_layer_add_marker (layer, CHAMPLAIN_MARKER (marker));
  champlain_marker_animate_in(CHAMPLAIN_MARKER (marker));
  /* champlain_path_layer_add_node (*path, CHAMPLAIN_LOCATION (marker)); */
  g_signal_connect(CHAMPLAIN_LOCATION(marker), "button-press", G_CALLBACK(marker_function), station);

  marker = champlain_label_new_from_file ("icons/emblem-generic.png", NULL);
  station = g_strdup("Kristiansand, Norway\n<span size=\"small\">NRK Sørlandet</span>");
  champlain_label_set_text (CHAMPLAIN_LABEL (marker), station);
  champlain_label_set_use_markup (CHAMPLAIN_LABEL (marker), TRUE);
  champlain_label_set_color (CHAMPLAIN_LABEL (marker), &city_color);
  champlain_label_set_text_color (CHAMPLAIN_LABEL (marker), &text_color);
  champlain_location_set_location (CHAMPLAIN_LOCATION (marker), 58.14615, 7.9957333);
  champlain_marker_layer_add_marker (layer, CHAMPLAIN_MARKER (marker));
  champlain_marker_animate_in(CHAMPLAIN_MARKER (marker));
  /* champlain_path_layer_add_node (*path, CHAMPLAIN_LOCATION (marker)); */
  g_signal_connect(CHAMPLAIN_LOCATION(marker), "button-press", G_CALLBACK(marker_function), station);
  marker = champlain_label_new_from_file ("icons/emblem-generic.png", NULL);
  station = g_strdup("Houston, Texas\n<span size=\"small\">Coog Radio, University of Houston</span>");
  champlain_label_set_text (CHAMPLAIN_LABEL (marker), station);
  champlain_label_set_use_markup (CHAMPLAIN_LABEL (marker), TRUE);
  champlain_label_set_color (CHAMPLAIN_LABEL (marker), &city_color);
  champlain_label_set_text_color (CHAMPLAIN_LABEL (marker), &text_color);
  champlain_location_set_location (CHAMPLAIN_LOCATION (marker), 29.7589382, -95.3676974);
  champlain_marker_layer_add_marker (layer, CHAMPLAIN_MARKER (marker));
  champlain_marker_animate_in(CHAMPLAIN_MARKER (marker));
  /* champlain_path_layer_add_node (*path, CHAMPLAIN_LOCATION (marker)); */
  g_signal_connect(CHAMPLAIN_LOCATION(marker), "button-press", G_CALLBACK(marker_function), station);
  marker = champlain_label_new_from_file ("icons/emblem-generic.png", NULL);
  station = g_strdup("London, United Kingdom\n<span size=\"small\">Imperial College Radio</span>");
  champlain_label_set_text (CHAMPLAIN_LABEL (marker), station);
  champlain_label_set_use_markup (CHAMPLAIN_LABEL (marker), TRUE);
  champlain_label_set_color (CHAMPLAIN_LABEL (marker), &city_color);
  champlain_label_set_text_color (CHAMPLAIN_LABEL (marker), &text_color);
  champlain_location_set_location (CHAMPLAIN_LOCATION (marker), 51.5073219, -0.1276474);
  champlain_marker_layer_add_marker (layer, CHAMPLAIN_MARKER (marker));
  champlain_marker_animate_in(CHAMPLAIN_MARKER (marker));
  /* champlain_path_layer_add_node (*path, CHAMPLAIN_LOCATION (marker)); */
  g_signal_connect(CHAMPLAIN_LOCATION(marker), "button-press", G_CALLBACK(marker_function), station);
  marker = champlain_label_new_from_file ("icons/emblem-generic.png", NULL);
  station = g_strdup("Los Angeles, California\n<span size=\"small\">KXSC</span>");
  champlain_label_set_text (CHAMPLAIN_LABEL (marker), station);
  champlain_label_set_use_markup (CHAMPLAIN_LABEL (marker), TRUE);
  champlain_label_set_color (CHAMPLAIN_LABEL (marker), &city_color);
  champlain_label_set_text_color (CHAMPLAIN_LABEL (marker), &text_color);
  champlain_location_set_location (CHAMPLAIN_LOCATION (marker), 34.1430079, -118.14176172581);
  champlain_marker_layer_add_marker (layer, CHAMPLAIN_MARKER (marker));
  champlain_marker_animate_in(CHAMPLAIN_MARKER (marker));
  /* champlain_path_layer_add_node (*path, CHAMPLAIN_LOCATION (marker)); */
  g_signal_connect(CHAMPLAIN_LOCATION(marker), "button-press", G_CALLBACK(marker_function), station);
  marker = champlain_label_new_from_file ("icons/emblem-generic.png", NULL);
  station = g_strdup("México City, México\n<span size=\"small\">Radio UNAM</span>");
  champlain_label_set_text (CHAMPLAIN_LABEL (marker), station);
  champlain_label_set_use_markup (CHAMPLAIN_LABEL (marker), TRUE);
  champlain_label_set_color (CHAMPLAIN_LABEL (marker), &city_color);
  champlain_label_set_text_color (CHAMPLAIN_LABEL (marker), &text_color);
  champlain_location_set_location (CHAMPLAIN_LOCATION (marker), 19.647012, -101.22900565);
  champlain_marker_layer_add_marker (layer, CHAMPLAIN_MARKER (marker));
  champlain_marker_animate_in(CHAMPLAIN_MARKER (marker));
  /* champlain_path_layer_add_node (*path, CHAMPLAIN_LOCATION (marker)); */
  g_signal_connect(CHAMPLAIN_LOCATION(marker), "button-press", G_CALLBACK(marker_function), station);
  marker = champlain_label_new_from_file ("icons/emblem-generic.png", NULL);
  station = g_strdup("Moscow, Russia\n<span size=\"small\">Echo of Moscow</span>");
  champlain_label_set_text (CHAMPLAIN_LABEL (marker), station);
  champlain_label_set_use_markup (CHAMPLAIN_LABEL (marker), TRUE);
  champlain_label_set_color (CHAMPLAIN_LABEL (marker), &city_color);
  champlain_label_set_text_color (CHAMPLAIN_LABEL (marker), &text_color);
  champlain_location_set_location (CHAMPLAIN_LOCATION (marker), 55.4792046, 37.3273304);
  champlain_marker_layer_add_marker (layer, CHAMPLAIN_MARKER (marker));
  champlain_marker_animate_in(CHAMPLAIN_MARKER (marker));
  /* champlain_path_layer_add_node (*path, CHAMPLAIN_LOCATION (marker)); */
  g_signal_connect(CHAMPLAIN_LOCATION(marker), "button-press", G_CALLBACK(marker_function), station);
  marker = champlain_label_new_from_file ("icons/emblem-generic.png", NULL);
  station = g_strdup("Newcastle, Australia\n<span size=\"small\">2NURFM</span>");
  champlain_label_set_text (CHAMPLAIN_LABEL (marker), station);
  champlain_label_set_use_markup (CHAMPLAIN_LABEL (marker), TRUE);
  champlain_label_set_color (CHAMPLAIN_LABEL (marker), &city_color);
  champlain_label_set_text_color (CHAMPLAIN_LABEL (marker), &text_color);
  champlain_location_set_location (CHAMPLAIN_LOCATION (marker), -32.9272881, 151.7812534);
  champlain_marker_layer_add_marker (layer, CHAMPLAIN_MARKER (marker));
  champlain_marker_animate_in(CHAMPLAIN_MARKER (marker));
  /* champlain_path_layer_add_node (*path, CHAMPLAIN_LOCATION (marker)); */
  g_signal_connect(CHAMPLAIN_LOCATION(marker), "button-press", G_CALLBACK(marker_function), station);
  marker = champlain_label_new_from_file ("icons/emblem-generic.png", NULL);
  station = g_strdup("New York City, New York\n<span size=\"small\">WKCR</span>");
  champlain_label_set_text (CHAMPLAIN_LABEL (marker), station);
  champlain_label_set_use_markup (CHAMPLAIN_LABEL (marker), TRUE);
  champlain_label_set_color (CHAMPLAIN_LABEL (marker), &city_color);
  champlain_label_set_text_color (CHAMPLAIN_LABEL (marker), &text_color);
  champlain_location_set_location (CHAMPLAIN_LOCATION (marker), 40.7306458, -73.9866136);
  champlain_marker_layer_add_marker (layer, CHAMPLAIN_MARKER (marker));
  champlain_marker_animate_in(CHAMPLAIN_MARKER (marker));
  /* champlain_path_layer_add_node (*path, CHAMPLAIN_LOCATION (marker)); */
  g_signal_connect(CHAMPLAIN_LOCATION(marker), "button-press", G_CALLBACK(marker_function), station);
  marker = champlain_label_new_from_file ("icons/emblem-generic.png", NULL);
  station = g_strdup("Oslo, Norway\n<span size=\"small\">NRK Sport</span>");
  champlain_label_set_text (CHAMPLAIN_LABEL (marker), station);
  champlain_label_set_use_markup (CHAMPLAIN_LABEL (marker), TRUE);
  champlain_label_set_color (CHAMPLAIN_LABEL (marker), &city_color);
  champlain_label_set_text_color (CHAMPLAIN_LABEL (marker), &text_color);
  champlain_location_set_location (CHAMPLAIN_LOCATION (marker), 59.9132694, 10.7391112);
  champlain_marker_layer_add_marker (layer, CHAMPLAIN_MARKER (marker));
  champlain_marker_animate_in(CHAMPLAIN_MARKER (marker));
  /* champlain_path_layer_add_node (*path, CHAMPLAIN_LOCATION (marker)); */
  g_signal_connect(CHAMPLAIN_LOCATION(marker), "button-press", G_CALLBACK(marker_function), station);
  marker = champlain_label_new_from_file ("icons/emblem-generic.png", NULL);
  station = g_strdup("Oxford, United Kingdom\n<span size=\"small\">Oxide Radio</span>");
  champlain_label_set_text (CHAMPLAIN_LABEL (marker), station);
  champlain_label_set_use_markup (CHAMPLAIN_LABEL (marker), TRUE);
  champlain_label_set_color (CHAMPLAIN_LABEL (marker), &city_color);
  champlain_label_set_text_color (CHAMPLAIN_LABEL (marker), &text_color);
  champlain_location_set_location (CHAMPLAIN_LOCATION (marker), 51.7520131, -1.2578499);
  champlain_marker_layer_add_marker (layer, CHAMPLAIN_MARKER (marker));
  champlain_marker_animate_in(CHAMPLAIN_MARKER (marker));
  /* champlain_path_layer_add_node (*path, CHAMPLAIN_LOCATION (marker)); */
  g_signal_connect(CHAMPLAIN_LOCATION(marker), "button-press", G_CALLBACK(marker_function), station);
  marker = champlain_label_new_from_file ("icons/emblem-generic.png", NULL);
  station = g_strdup("Ottawa, Canada\n<span size=\"small\">CHUO</span>");
  champlain_label_set_text (CHAMPLAIN_LABEL (marker), station);
  champlain_label_set_use_markup (CHAMPLAIN_LABEL (marker), TRUE);
  champlain_label_set_color (CHAMPLAIN_LABEL (marker), &city_color);
  champlain_label_set_text_color (CHAMPLAIN_LABEL (marker), &text_color);
  champlain_location_set_location (CHAMPLAIN_LOCATION (marker), 45.421106, -75.690308);
  champlain_marker_layer_add_marker (layer, CHAMPLAIN_MARKER (marker));
  champlain_marker_animate_in(CHAMPLAIN_MARKER (marker));
  /* champlain_path_layer_add_node (*path, CHAMPLAIN_LOCATION (marker)); */
  g_signal_connect(CHAMPLAIN_LOCATION(marker), "button-press", G_CALLBACK(marker_function), station);
  marker = champlain_label_new_from_file ("icons/emblem-generic.png", NULL);
  station = g_strdup("Paris, France\n<span size=\"small\">Radio Campus Paris</span>");
  champlain_label_set_text (CHAMPLAIN_LABEL (marker), station);
  champlain_label_set_use_markup (CHAMPLAIN_LABEL (marker), TRUE);
  champlain_label_set_color (CHAMPLAIN_LABEL (marker), &city_color);
  champlain_label_set_text_color (CHAMPLAIN_LABEL (marker), &text_color);
  champlain_location_set_location (CHAMPLAIN_LOCATION (marker), 48.8566101, 2.3514992);
  champlain_marker_layer_add_marker (layer, CHAMPLAIN_MARKER (marker));
  champlain_marker_animate_in(CHAMPLAIN_MARKER (marker));
  /* champlain_path_layer_add_node (*path, CHAMPLAIN_LOCATION (marker)); */
  g_signal_connect(CHAMPLAIN_LOCATION(marker), "button-press", G_CALLBACK(marker_function), station);
  marker = champlain_label_new_from_file ("icons/emblem-generic.png", NULL);
  station = g_strdup("Phoenix, Arizona\n<span size=\"small\">KASC</span>");
  champlain_label_set_text (CHAMPLAIN_LABEL (marker), station);
  champlain_label_set_use_markup (CHAMPLAIN_LABEL (marker), TRUE);
  champlain_label_set_color (CHAMPLAIN_LABEL (marker), &city_color);
  champlain_label_set_text_color (CHAMPLAIN_LABEL (marker), &text_color);
  champlain_location_set_location (CHAMPLAIN_LOCATION (marker), 33.4485866, -112.0773456);
  champlain_marker_layer_add_marker (layer, CHAMPLAIN_MARKER (marker));
  champlain_marker_animate_in(CHAMPLAIN_MARKER (marker));
  /* champlain_path_layer_add_node (*path, CHAMPLAIN_LOCATION (marker)); */
  g_signal_connect(CHAMPLAIN_LOCATION(marker), "button-press", G_CALLBACK(marker_function), station);
  marker = champlain_label_new_from_file ("icons/emblem-generic.png", NULL);
  station = g_strdup("San Francisco, California\n<span size=\"small\">SomaFM</span>");
  champlain_label_set_text (CHAMPLAIN_LABEL (marker), station);
  champlain_label_set_use_markup (CHAMPLAIN_LABEL (marker), TRUE);
  champlain_label_set_color (CHAMPLAIN_LABEL (marker), &city_color);
  champlain_label_set_text_color (CHAMPLAIN_LABEL (marker), &text_color);
  champlain_location_set_location (CHAMPLAIN_LOCATION (marker), 37.7792808, -122.4192363);
  champlain_marker_layer_add_marker (layer, CHAMPLAIN_MARKER (marker));
  champlain_marker_animate_in(CHAMPLAIN_MARKER (marker));
  /* champlain_path_layer_add_node (*path, CHAMPLAIN_LOCATION (marker)); */
  g_signal_connect(CHAMPLAIN_LOCATION(marker), "button-press", G_CALLBACK(marker_function), station);
  marker = champlain_label_new_from_file ("icons/emblem-generic.png", NULL);
  station = g_strdup("Seattle, Washington\n<span size=\"small\">KSUB</span>");
  champlain_label_set_text (CHAMPLAIN_LABEL (marker), station);
  champlain_label_set_use_markup (CHAMPLAIN_LABEL (marker), TRUE);
  champlain_label_set_color (CHAMPLAIN_LABEL (marker), &city_color);
  champlain_label_set_text_color (CHAMPLAIN_LABEL (marker), &text_color);
  champlain_location_set_location (CHAMPLAIN_LOCATION (marker), 47.6038321, -122.3300624);
  champlain_marker_layer_add_marker (layer, CHAMPLAIN_MARKER (marker));
  champlain_marker_animate_in(CHAMPLAIN_MARKER (marker));
  /* champlain_path_layer_add_node (*path, CHAMPLAIN_LOCATION (marker)); */
  g_signal_connect(CHAMPLAIN_LOCATION(marker), "button-press", G_CALLBACK(marker_function), station);
  marker = champlain_label_new_from_file ("icons/emblem-generic.png", NULL);
  station = g_strdup("Stanford, California\n<span size=\"small\">KZSU</span>");
  champlain_label_set_text (CHAMPLAIN_LABEL (marker), station);
  champlain_label_set_use_markup (CHAMPLAIN_LABEL (marker), TRUE);
  champlain_label_set_color (CHAMPLAIN_LABEL (marker), &city_color);
  champlain_label_set_text_color (CHAMPLAIN_LABEL (marker), &text_color);
  champlain_location_set_location (CHAMPLAIN_LOCATION (marker), 37.4248398, -122.1677058);
  champlain_marker_layer_add_marker (layer, CHAMPLAIN_MARKER (marker));
  champlain_marker_animate_in(CHAMPLAIN_MARKER (marker));
  /* champlain_path_layer_add_node (*path, CHAMPLAIN_LOCATION (marker)); */
  g_signal_connect(CHAMPLAIN_LOCATION(marker), "button-press", G_CALLBACK(marker_function), station);
  marker = champlain_label_new_from_file ("icons/emblem-generic.png", NULL);
  station = g_strdup("Stockholm, Sweden\n<span size=\"small\">Sveriges Radio P1</span>");
  champlain_label_set_text (CHAMPLAIN_LABEL (marker), station);
  champlain_label_set_use_markup (CHAMPLAIN_LABEL (marker), TRUE);
  champlain_label_set_color (CHAMPLAIN_LABEL (marker), &city_color);
  champlain_label_set_text_color (CHAMPLAIN_LABEL (marker), &text_color);
  champlain_location_set_location (CHAMPLAIN_LOCATION (marker), 59.3251172, 18.0710935);
  champlain_marker_layer_add_marker (layer, CHAMPLAIN_MARKER (marker));
  champlain_marker_animate_in(CHAMPLAIN_MARKER (marker));
  /* champlain_path_layer_add_node (*path, CHAMPLAIN_LOCATION (marker)); */
  g_signal_connect(CHAMPLAIN_LOCATION(marker), "button-press", G_CALLBACK(marker_function), station);
  marker = champlain_label_new_from_file ("icons/emblem-generic.png", NULL);
  station = g_strdup("Sydney, Canada\n<span size=\"small\">Caper Radio</span>");
  champlain_label_set_text (CHAMPLAIN_LABEL (marker), station);
  champlain_label_set_use_markup (CHAMPLAIN_LABEL (marker), TRUE);
  champlain_label_set_color (CHAMPLAIN_LABEL (marker), &city_color);
  champlain_label_set_text_color (CHAMPLAIN_LABEL (marker), &text_color);
  champlain_location_set_location (CHAMPLAIN_LOCATION (marker), 46.1654766, -60.1735637935906);
  champlain_marker_layer_add_marker (layer, CHAMPLAIN_MARKER (marker));
  champlain_marker_animate_in(CHAMPLAIN_MARKER (marker));
  /* champlain_path_layer_add_node (*path, CHAMPLAIN_LOCATION (marker)); */
  g_signal_connect(CHAMPLAIN_LOCATION(marker), "button-press", G_CALLBACK(marker_function), station);
  marker = champlain_label_new_from_file ("icons/emblem-generic.png", NULL);
  station = g_strdup("Tampere, Finland\n<span size=\"small\">Radio Moreeni</span>");
  champlain_label_set_text (CHAMPLAIN_LABEL (marker), station);
  champlain_label_set_use_markup (CHAMPLAIN_LABEL (marker), TRUE);
  champlain_label_set_color (CHAMPLAIN_LABEL (marker), &city_color);
  champlain_label_set_text_color (CHAMPLAIN_LABEL (marker), &text_color);
  champlain_location_set_location (CHAMPLAIN_LOCATION (marker), 61.4980214, 23.7603118);
  champlain_marker_layer_add_marker (layer, CHAMPLAIN_MARKER (marker));
  champlain_marker_animate_in(CHAMPLAIN_MARKER (marker));
  /* champlain_path_layer_add_node (*path, CHAMPLAIN_LOCATION (marker)); */
  g_signal_connect(CHAMPLAIN_LOCATION(marker), "button-press", G_CALLBACK(marker_function), station);
  marker = champlain_label_new_from_file ("icons/emblem-generic.png", NULL);
  station = g_strdup("Toronto, Canada\n<span size=\"small\">CIUT</span>");
  champlain_label_set_text (CHAMPLAIN_LABEL (marker), station);
  champlain_label_set_use_markup (CHAMPLAIN_LABEL (marker), TRUE);
  champlain_label_set_color (CHAMPLAIN_LABEL (marker), &city_color);
  champlain_label_set_text_color (CHAMPLAIN_LABEL (marker), &text_color);
  champlain_location_set_location (CHAMPLAIN_LOCATION (marker), 43.653963, -79.387207);
  champlain_marker_layer_add_marker (layer, CHAMPLAIN_MARKER (marker));
  champlain_marker_animate_in(CHAMPLAIN_MARKER (marker));
  /* champlain_path_layer_add_node (*path, CHAMPLAIN_LOCATION (marker)); */
  g_signal_connect(CHAMPLAIN_LOCATION(marker), "button-press", G_CALLBACK(marker_function), station);
  marker = champlain_label_new_from_file ("icons/emblem-generic.png", NULL);
  station = g_strdup("Trondheim, Norway\n<span size=\"small\">Radio Revolt</span>");
  champlain_label_set_text (CHAMPLAIN_LABEL (marker), station);
  champlain_label_set_use_markup (CHAMPLAIN_LABEL (marker), TRUE);
  champlain_label_set_color (CHAMPLAIN_LABEL (marker), &city_color);
  champlain_label_set_text_color (CHAMPLAIN_LABEL (marker), &text_color);
  champlain_location_set_location (CHAMPLAIN_LOCATION (marker), 63.4305658, 10.3951929);
  champlain_marker_layer_add_marker (layer, CHAMPLAIN_MARKER (marker));
  champlain_marker_animate_in(CHAMPLAIN_MARKER (marker));
  /* champlain_path_layer_add_node (*path, CHAMPLAIN_LOCATION (marker)); */
  g_signal_connect(CHAMPLAIN_LOCATION(marker), "button-press", G_CALLBACK(marker_function), station);
  marker = champlain_label_new_from_file ("icons/emblem-generic.png", NULL);
  station = g_strdup("Warsaw, Poland\n<span size=\"small\">Radio Aktywne</span>");
  champlain_label_set_text (CHAMPLAIN_LABEL (marker), station);
  champlain_label_set_use_markup (CHAMPLAIN_LABEL (marker), TRUE);
  champlain_label_set_color (CHAMPLAIN_LABEL (marker), &city_color);
  champlain_label_set_text_color (CHAMPLAIN_LABEL (marker), &text_color);
  champlain_location_set_location (CHAMPLAIN_LOCATION (marker), 52.2319237, 21.0067265);
  champlain_marker_layer_add_marker (layer, CHAMPLAIN_MARKER (marker));
  champlain_marker_animate_in(CHAMPLAIN_MARKER (marker));
  /* champlain_path_layer_add_node (*path, CHAMPLAIN_LOCATION (marker)); */
  g_signal_connect(CHAMPLAIN_LOCATION(marker), "button-press", G_CALLBACK(marker_function), station);
  marker = champlain_label_new_from_file ("icons/emblem-generic.png", NULL);
  station = g_strdup("Washington, District of Columbia\n<span size=\"small\">WAMU</span>");
  champlain_label_set_text (CHAMPLAIN_LABEL (marker), station);
  champlain_label_set_use_markup (CHAMPLAIN_LABEL (marker), TRUE);
  champlain_label_set_color (CHAMPLAIN_LABEL (marker), &city_color);
  champlain_label_set_text_color (CHAMPLAIN_LABEL (marker), &text_color);
  champlain_location_set_location (CHAMPLAIN_LOCATION (marker), 38.8949549, -77.0366456);
  champlain_marker_layer_add_marker (layer, CHAMPLAIN_MARKER (marker));
  champlain_marker_animate_in(CHAMPLAIN_MARKER (marker));
  /* champlain_path_layer_add_node (*path, CHAMPLAIN_LOCATION (marker)); */
  g_signal_connect(CHAMPLAIN_LOCATION(marker), "button-press", G_CALLBACK(marker_function), station);
  marker = champlain_label_new_from_file ("icons/emblem-generic.png", NULL);
  station = g_strdup("Waterloo, Canada\n<span size=\"small\">SoundFM</span>");
  champlain_label_set_text (CHAMPLAIN_LABEL (marker), station);
  champlain_label_set_use_markup (CHAMPLAIN_LABEL (marker), TRUE);
  champlain_label_set_color (CHAMPLAIN_LABEL (marker), &city_color);
  champlain_label_set_text_color (CHAMPLAIN_LABEL (marker), &text_color);
  champlain_location_set_location (CHAMPLAIN_LOCATION (marker), 43.466874, -80.524635);
  champlain_marker_layer_add_marker (layer, CHAMPLAIN_MARKER (marker));
  champlain_marker_animate_in(CHAMPLAIN_MARKER (marker));
  /* champlain_path_layer_add_node (*path, CHAMPLAIN_LOCATION (marker)); */
  g_signal_connect(CHAMPLAIN_LOCATION(marker), "button-press", G_CALLBACK(marker_function), station);
#if 0
  marker = champlain_label_new_from_file ("icons/emblem-important.png", NULL);
  champlain_label_set_color (CHAMPLAIN_LABEL (marker), &city_color);
  champlain_label_set_text_color (CHAMPLAIN_LABEL (marker), &text_color);
  champlain_location_set_location (CHAMPLAIN_LOCATION (marker), 37.873093, -122.303769);
  champlain_marker_layer_add_marker (layer, CHAMPLAIN_MARKER (marker));
  champlain_path_layer_add_node (*path, CHAMPLAIN_LOCATION (marker));
  marker = champlain_point_new ();
  champlain_label_set_color (CHAMPLAIN_LABEL (marker), &city_color);
  champlain_label_set_text_color (CHAMPLAIN_LABEL (marker), &text_color);
  champlain_location_set_location (CHAMPLAIN_LOCATION (marker), 37.873093, -122.303769);
  champlain_marker_layer_add_marker (layer, CHAMPLAIN_MARKER (marker));
  champlain_path_layer_add_node (*path, CHAMPLAIN_LOCATION (marker));
  marker = champlain_label_new_from_file ("icons/emblem-favorite.png", NULL);
  champlain_label_set_draw_background (CHAMPLAIN_LABEL (marker), FALSE);
  champlain_label_set_color (CHAMPLAIN_LABEL (marker), &city_color);
  champlain_label_set_text_color (CHAMPLAIN_LABEL (marker), &text_color);
  champlain_location_set_location (CHAMPLAIN_LOCATION (marker), 37.873093, -122.303769);
  champlain_marker_layer_add_marker (layer, CHAMPLAIN_MARKER (marker));
  champlain_path_layer_add_node (*path, CHAMPLAIN_LOCATION (marker));
#endif
  champlain_marker_layer_set_all_markers_draggable (layer);
  clutter_actor_show (layer_actor);
  return layer;
}
