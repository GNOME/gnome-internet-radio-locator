/* $Id$
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

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <locale.h>
#include <sys/stat.h>

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <gtk/gtk.h>
#include <gtk/gtkcombobox.h>
#include <glib/gstdio.h>
#include "gnome-internet-radio-locator.h"
#include "gnome-internet-radio-locator-gui.h"
#include "gnome-internet-radio-locator-program.h"
#include "gnome-internet-radio-locator-station.h"
#include "gnome-internet-radio-locator-streams.h"
#include "gnome-internet-radio-locator-tz.h"

extern GtkWidget *gnome_internet_radio_locator_app;
extern GtkWidget *search_selector;

GtkWidget *create_stations_selector(char *selected_station_uri,
				    char *filename)
{
	GNOMEInternetRadioLocatorStationInfo *stationinfo, *localstation;
	GtkWidget *stations_selector;
	GtkWidget *align, *menu, *drop_down, *item;

	gchar *station_uri, *station_name, *station_location, *station_band, *station_description, *station_website;
	gchar *label, *world_station_xml_filename, *local_station_xml_file;

	int i = 0, selection = -1;

	GStatBuf stats;

	memset(&stats, 0, sizeof(stats));

	/* The Stations dialog */
	stations_selector = gtk_dialog_new_with_buttons("Select a station", GTK_WINDOW(gnome_internet_radio_locator_app), 0,	/* flags */
							NULL,
							GTK_RESPONSE_ACCEPT,
							NULL);
	/* gtk_container_set_border_width */
	/*     (GTK_CONTAINER(GTK_DIALOG(stations_selector)->vbox), 6); */

	/* align = gtk_alignment_new(0.5, 0.5, 0, 0); */
	/* gtk_container_add(GTK_CONTAINER */
	/* 		  (GTK_DIALOG(stations_selector)->vbox), align); */
	/* gtk_container_set_border_width(GTK_CONTAINER(align), 6); */
	/* gtk_widget_show(align); */

	menu = gtk_menu_new();
	gtk_widget_show(menu);

	/* creating the menu items */

	/* world_station_xml_filename = gnome_program_locate_file(NULL, */
	/* 						       GNOME_FILE_DOMAIN_APP_DATADIR, */
	/* 						       "gnome-internet-radio-locator/gnome-internet-radio-locator.xml", */
	/* 						       FALSE, */
	/* 						       NULL); */

	/* world_station_xml_filename = g_strdup("http://gnome-internet-radio-locator.src.oka.no/gnome-internet-radio-locator.xml"); */

	world_station_xml_filename = g_strconcat(GNOME_INTERNET_RADIO_LOCATOR_DATADIR, "/gnome-internet-radio-locator.xml", NULL);
	GNOME_INTERNET_RADIO_LOCATOR_DEBUG_MSG("world_station_xml_filename = %s\n",
	    world_station_xml_filename);

	if (world_station_xml_filename == NULL) {
		g_warning("Failed to open %s.\n",
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
		g_warning("Failed to open %s\n", local_station_xml_file);
	}

/*   g_free (local_station_xml_file); */

	stationinfo =
	    gnome_internet_radio_locator_station_load_from_file(localstation,
					world_station_xml_filename);

	gnome_internet_radio_locator_stations = NULL;

	while (stationinfo != NULL) {

		label =
		    g_strconcat(stationinfo->name, " (",
				stationinfo->location, ")", NULL);
		station_uri = g_strdup(stationinfo->stream->uri);
		station_name = g_strdup(stationinfo->name);
		station_location = g_strdup(stationinfo->location);
		station_band = g_strdup(stationinfo->band);
		station_description = g_strdup(stationinfo->description);
		station_website = g_strdup(stationinfo->uri);
		
		gnome_internet_radio_locator_stations = g_list_append(gnome_internet_radio_locator_stations,(GNOMEInternetRadioLocatorStationInfo *)stationinfo);

		if (label != NULL) {
			item = gtk_menu_item_new_with_label(label);
			gtk_menu_shell_append(GTK_MENU_SHELL(menu), item);
			g_signal_connect(G_OBJECT(item), "activate",
					 G_CALLBACK
					 (on_stations_selector_changed),
					 NULL);
			g_object_set_data(G_OBJECT(item), "station_uri",
					  (gpointer) station_uri);
			g_object_set_data(G_OBJECT(item), "station_name",
					  (gpointer) station_name);
			g_object_set_data(G_OBJECT(item),
					  "station_location",
					  (gpointer) station_location);
			g_object_set_data(G_OBJECT(item),
					  "station_band",
					  (gpointer) station_band);
			g_object_set_data(G_OBJECT(item),
					  "station_description",
					  (gpointer) station_description);
			g_object_set_data(G_OBJECT(item),
					  "station_website",
					  (gpointer) station_website);
			gtk_widget_show(item);
			g_free(label);

			/* selection */
			if (selected_station_uri != NULL &&
			    !strcmp(selected_station_uri, station_uri))
				selection = i;
		} else {
			g_free(station_uri);
			g_free(station_name);
			g_free(station_location);
			g_free(station_band);
			g_free(station_description);
		}
		i++;
		stationinfo = stationinfo->next;
	}

	/* drop_down = gtk_combo_box_new(); */
	/* gtk_widget_show(drop_down); */
	/* gtk_combo_box_popdown(GTK_OPTION_MENU(drop_down), menu); */
	/* gtk_container_add(GTK_CONTAINER(align), drop_down); */

	if (selection != -1)
		gtk_combo_box_set_active(GTK_COMBO_BOX(drop_down), selection);

	g_signal_connect(G_OBJECT(stations_selector), "response",
			 G_CALLBACK(gtk_widget_hide),
			 (gpointer) stations_selector);
	g_signal_connect(G_OBJECT(stations_selector), "delete-event",
			 G_CALLBACK(gtk_widget_hide),
			 (gpointer) stations_selector);

	return stations_selector;
}

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
	/* gnome_internet_radio_locator_helper_run(gnome_internet_radio_locator->selected_station_uri, */
	/* 		gnome_internet_radio_locator->selected_station_name, */
	/* 		GNOME_INTERNET_RADIO_LOCATOR_STREAM_SHOUTCAST, */
	/* 		GNOME_INTERNET_RADIO_LOCATOR_STREAM_PLAYER); */
	return FALSE;
}

GtkWidget *create_new_station_selector(void) {

	GNOMEInternetRadioLocatorStationInfo *localstation, *stationinfo;
	GtkWidget *station_selector, *content_area;
	GtkWidget *align;
	GtkWidget *bandentry, *descriptionentry, *nameentry, *locationentry, *urientry, *websiteentry;
	GtkEntryCompletion *completion;
	GtkListStore *location_model;
	GtkTreeIter iter;
	TzDB *db;
	GPtrArray *locs;
	guint i;
	char *pixmap_dir = NULL;
	gchar *path = NULL;
	setlocale (LC_ALL, "C");

	gchar *world_station_xml_filename, *local_station_xml_file;
	gint retval;
	
	/* int i = 0, search_selection = -1; */

	GStatBuf stats;

	memset(&stats, 0, sizeof(stats));

	/* The Stations dialog */
	/* station_selector = gtk_dialog_new_with_buttons(_("New radio station"), GTK_WINDOW(GNOMEInternetRadioLocator_app), 0,	/\* flags *\/ */
	/* 					       GTK_STOCK_SAVE, */
	/* 					       GTK_RESPONSE_ACCEPT, */
	/* 					       NULL); */
	content_area = gtk_dialog_get_content_area (GTK_DIALOG (station_selector));
	/* gtk_container_set_border_width */
	/*     (GTK_CONTAINER(GTK_DIALOG(station_selector)->vbox), 6); */

	/* align = gtk_alignment_new(0.5, 0.5, 0, 0); */
	/* gtk_container_add(GTK_CONTAINER */
	/* 		  (GTK_DIALOG(station_selector)->vbox), align); */
	gtk_container_set_border_width(GTK_CONTAINER(align), 6);
	gtk_widget_show(align);

	nameentry = gtk_entry_new();
	locationentry = gtk_entry_new();
	urientry = gtk_entry_new();
	bandentry = gtk_entry_new();
	websiteentry = gtk_entry_new();
	descriptionentry = gtk_entry_new();

	gtk_entry_set_text(GTK_ENTRY(nameentry), "Station name");
	gtk_entry_set_text(GTK_ENTRY(locationentry), "City name");
	gtk_entry_set_text(GTK_ENTRY(urientry), "http://uri-to-stream/");
	gtk_entry_set_text(GTK_ENTRY(bandentry), "FM/AM bandwidth");
	gtk_entry_set_text(GTK_ENTRY(websiteentry), "http://uri-to-website/");
	gtk_entry_set_text(GTK_ENTRY(descriptionentry), "Description");
	completion = gtk_entry_completion_new();
	gtk_entry_completion_set_text_column(completion, STATION_LOCATION);
	gtk_entry_set_completion(GTK_ENTRY(locationentry), completion);
	g_signal_connect(G_OBJECT(completion), "match-selected",
			 G_CALLBACK(on_location_matches), NULL);
	location_model = gtk_list_store_new(2, G_TYPE_STRING, G_TYPE_STRING);

	world_station_xml_filename = g_strconcat(GNOME_INTERNET_RADIO_LOCATOR_DATADIR, "/gnome-internet-radio-locator.xml", NULL);
	GNOME_INTERNET_RADIO_LOCATOR_DEBUG_MSG("world_station_xml_filename = %s\n",
	    world_station_xml_filename);

	if (world_station_xml_filename == NULL) {
		g_warning(("Failed to open %s.  Please install it.\n"),
			  world_station_xml_filename);
	}

	local_station_xml_file =
	    g_strconcat(g_get_home_dir(), "/.gnome_internet_radio_locator/gnome_internet_radio_locator.xml", NULL);

	if (!g_stat(local_station_xml_file, &stats)) {
		localstation = gnome_internet_radio_locator_station_load_from_file(NULL, local_station_xml_file);
	} else {
		localstation = NULL;
	}

	if (localstation == NULL) {
		g_warning("Failed to open %s.\n", local_station_xml_file);
	}

/*   g_free (local_station_xml_file); */

	stationinfo =
	    gnome_internet_radio_locator_station_load_from_file(localstation,
					world_station_xml_filename);

	// gnome_internet_radio_locator_stations = NULL;

	while (stationinfo != NULL) {
	/* Timezone map */
	  db = tz_load_db ();
	  locs = tz_get_locations (db);
	  for (i = 0; i < locs->len ; i++) { 
	     	TzLocation *loc = locs->pdata[i]; 
		TzInfo *info; 
		char *filename; 
		gdouble selected_offset; 
		char buf[16]; 
		info = tz_info_from_location (loc);
		selected_offset = tz_location_get_utc_offset (loc) 
		   		/ (60.0*60.0) + ((info->daylight) ? -1.0 : 0.0); 
		filename = g_strdup_printf ("timezone_%s.png", 
					    g_ascii_formatd (buf, sizeof (buf), 
							     "%g", selected_offset)); 
		path = g_build_filename (pixmap_dir, filename, NULL);
		g_printf("Name is %s\n", tz_info_get_clean_name(db, loc->zone)); 
		GNOME_INTERNET_RADIO_LOCATOR_DEBUG_MSG("%s\n", loc->zone); 
		if (g_file_test (path, G_FILE_TEST_IS_REGULAR) == FALSE) { 
		  g_message ("File '%s' missing for zone '%s'", filename, loc->zone); 
		  gtk_list_store_append(location_model, &iter);
		  g_print("%s %s", stationinfo->location, loc->zone); 
		  if (g_strcmp0(stationinfo->location, loc->zone)==0) {
		    gtk_list_store_set(location_model,
				       &iter,
				       STATION_LOCATION,
				       loc->zone,
				       -1);
		  }
		  retval = 1;
		}
		stationinfo = stationinfo->next; 
	  } 
	  gtk_entry_completion_set_model(completion, GTK_TREE_MODEL(location_model));
	  gtk_widget_show(nameentry);
	  gtk_widget_show(locationentry);
	  gtk_widget_show(urientry);
	  gtk_widget_show(bandentry);
	  gtk_widget_show(descriptionentry);
	  gtk_widget_show(websiteentry);
	  gtk_container_add(GTK_CONTAINER(content_area), nameentry);
	  gtk_container_add(GTK_CONTAINER(content_area), locationentry);
	  gtk_container_add(GTK_CONTAINER(content_area), urientry);
	  gtk_container_add(GTK_CONTAINER(content_area), bandentry);
	  gtk_container_add(GTK_CONTAINER(content_area), descriptionentry);
	  gtk_container_add(GTK_CONTAINER(content_area), websiteentry);
	  /* g_signal_connect(G_OBJECT(station_selector), GTK_RESPONSE_ACCEPT, */
	  /* 		 G_CALLBACK(on_new_station_selector_changed), */
	  /* 		 NULL); */
	  g_object_set_data(G_OBJECT(station_selector), "station_location",
			    (gpointer) gtk_entry_get_text(GTK_ENTRY(locationentry)));
	  g_object_set_data(G_OBJECT(station_selector), "station_name",
			    (gpointer) gtk_entry_get_text(GTK_ENTRY(nameentry)));
	  g_object_set_data(G_OBJECT(station_selector), "station_uri",
			    (gpointer) gtk_entry_get_text(GTK_ENTRY(urientry)));
	  g_object_set_data(G_OBJECT(station_selector), "station_band",
			    (gpointer) gtk_entry_get_text(GTK_ENTRY(bandentry)));
	  g_object_set_data(G_OBJECT(station_selector), "station_description",
			    (gpointer) gtk_entry_get_text(GTK_ENTRY(descriptionentry)));
	  g_object_set_data(G_OBJECT(station_selector), "station_website",
			    (gpointer) gtk_entry_get_text(GTK_ENTRY(websiteentry)));
	  
#if 0 /* FIXME: Add input fields */
	  g_object_set_data(G_OBJECT(station_selector), "station_band",
			    (gpointer) station_band);
	  g_object_set_data(G_OBJECT(station_selector), "station_description",
			    (gpointer) station_description);
	  g_object_set_data(G_OBJECT(station_selector), "station_website",
			    (gpointer) station_website);
#endif
	  // gtk_widget_show(station_selector);
	  // g_free(label);
	  g_signal_connect(G_OBJECT(station_selector), "response",
			   G_CALLBACK(gtk_widget_hide),
			   (gpointer) station_selector);
	  g_signal_connect(G_OBJECT(station_selector), "delete-event",
			   G_CALLBACK(gtk_widget_hide),
			   (gpointer) station_selector);
	  /* tz_db_free (db); */
	  /* g_free (pixmap_dir); */
	  /* g_free (filename); */
	  /* g_free (path); */
	  return station_selector;
	}
}

GtkWidget *create_gnome_internet_radio_locator_app(void)
{
	GtkWidget *gnome_internet_radio_locator_app;
	GtkWidget *vbox1;

	GtkWidget *gnome_internet_radio_locator_pixmap;

	GtkWidget *appbar;
	GtkWidget *progress;
	
	GNOMEInternetRadioLocatorData *gnome_internet_radio_locator_data = g_new0(GNOMEInternetRadioLocatorData, 1);
	char *pmf;

	gtk_window_set_title(GTK_WINDOW(gnome_internet_radio_locator_app), "GNOME Internet Radio Locator");

	gnome_internet_radio_locator = gnome_internet_radio_locator_data;

#if GNOME_INTERNET_RADIO_LOCATOR_CFG_GNOME_CONFIG
	gnome_config_push_prefix("/gnome-internet-radio-locator/General/");

	gnome_internet_radio_locator->selected_listener_uri =
	    gnome_config_get_string("selected_listener_uri=");
	gnome_internet_radio_locator->selected_listener_name =
	    gnome_config_get_string("selected_listener_name=");
	gnome_internet_radio_locator->selected_listener_location =
	    gnome_config_get_string("selected_listener_location=");
	gnome_internet_radio_locator->selected_listener_band =
	    gnome_config_get_string("selected_listener_band=");
	gnome_internet_radio_locator->selected_listener_description =
	    gnome_config_get_string("selected_listener_description=");

	GNOME_INTERNET_RADIO_LOCATOR_DEBUG_MSG("gnome_internet_radio_locator->selected_listener_uri: %s\n",
	       gnome_internet_radio_locator->selected_listener_uri);
	GNOME_INTERNET_RADIO_LOCATOR_DEBUG_MSG("gnome_internet_radio_locator->selected_listener_name: %s\n",
	       gnome_internet_radio_locator->selected_listener_name);
	GNOME_INTERNET_RADIO_LOCATOR_DEBUG_MSG("gnome_internet_radio_locator->selected_listener_location: %s\n",
	       gnome_internet_radio_locator->selected_listener_location);
	GNOME_INTERNET_RADIO_LOCATOR_DEBUG_MSG("gnome_internet_radio_locator->selected_listener_band: %s\n",
	       gnome_internet_radio_locator->selected_listener_band);
	GNOME_INTERNET_RADIO_LOCATOR_DEBUG_MSG("gnome_internet_radio_locator->selected_listener_description: %s\n",
	       gnome_internet_radio_locator->selected_listener_description);

	gnome_internet_radio_locator->selected_station_uri =
	    gnome_config_get_string("selected_station_uri=");
	gnome_internet_radio_locator->selected_station_name =
	    gnome_config_get_string("selected_station_name=");
	gnome_internet_radio_locator->selected_station_location =
	    gnome_config_get_string("selected_station_location=");
	gnome_internet_radio_locator->selected_station_band =
	    gnome_config_get_string("selected_station_band=");
	gnome_internet_radio_locator->selected_station_description =
	    gnome_config_get_string("selected_station_description=");

	gnome_internet_radio_locator->selected_station_name =
	    gnome_config_get_string("selected_station_name=");
	gnome_internet_radio_locator->selected_station_location =
	    gnome_config_get_string("selected_station_location=");
	gnome_internet_radio_locator->selected_station_band =
	    gnome_config_get_string("selected_station_band=");
	gnome_internet_radio_locator->selected_station_description =
	    gnome_config_get_string("selected_station_description=");

	GNOME_INTERNET_RADIO_LOCATOR_DEBUG_MSG("gnome_internet_radio_locator->selected_station_uri: %s\n",
	       gnome_internet_radio_locator->selected_station_uri);
	GNOME_INTERNET_RADIO_LOCATOR_DEBUG_MSG("gnome_internet_radio_locator->selected_station_name: %s\n",
	       gnome_internet_radio_locator->selected_station_name);
	GNOME_INTERNET_RADIO_LOCATOR_DEBUG_MSG("gnome_internet_radio_locator->selected_station_location: %s\n",
	       gnome_internet_radio_locator->selected_station_location);
	GNOME_INTERNET_RADIO_LOCATOR_DEBUG_MSG("gnome_internet_radio_locator->selected_station_band: %s\n",
	       gnome_internet_radio_locator->selected_station_band);
	GNOME_INTERNET_RADIO_LOCATOR_DEBUG_MSG("gnome_internet_radio_locator->selected_station_description: %s\n",
	       gnome_internet_radio_locator->selected_station_description);

	if (strcmp(gnome_internet_radio_locator->selected_station_uri,"")==0) {
		gnome_internet_radio_locator->selected_station_uri = g_strdup("http://fm939.wnyc.org/wnycfm");
	}
	if (strcmp(gnome_internet_radio_locator->selected_station_name,"")==0) {
		gnome_internet_radio_locator->selected_station_name = g_strdup("WNYC");
	}
	if (strcmp(gnome_internet_radio_locator->selected_station_location,"")==0) {
		gnome_internet_radio_locator->selected_station_location = g_strdup("New York City, NY");
	}
	if (strcmp(gnome_internet_radio_locator->selected_station_band,"")==0) {
		gnome_internet_radio_locator->selected_station_band = g_strdup("ONLINE");
	}
	if (strcmp(gnome_internet_radio_locator->selected_station_description,"")==0) {
		gnome_internet_radio_locator->selected_station_description = g_strdup("WNYC 93.9 FM and AM 820 are New York's flagship public radio stations, broadcasting the finest programs from NPR, American Public Media, Public Radio International and the BBC World Service, as well as a wide range of award-winning local programming.");
	}

	gnome_internet_radio_locator->selected_streams_uri =
		gnome_config_get_string("selected_streams_uri=");
	gnome_internet_radio_locator->selected_streams_mime =
	    gnome_config_get_string("selected_streams_mime=");
	gnome_internet_radio_locator->selected_streams_codec =
	    gnome_config_get_string("selected_streams_codec=");
	gnome_internet_radio_locator->selected_streams_bitrate =
	    gnome_config_get_string("selected_streams_bitrate=");
	gnome_internet_radio_locator->selected_streams_samplerate =
	    gnome_config_get_string("selected_streams_samplerate=");
	gnome_internet_radio_locator->selected_streams_channels =
		(Gnome_Internet_Radio_LocatorChannels)gnome_config_get_string("selected_streams_channels=");

	GNOME_INTERNET_RADIO_LOCATOR_DEBUG_MSG("gnome_internet_radio_locator->selected_streams_uri: %s\n",
	       gnome_internet_radio_locator->selected_streams_uri);
	GNOME_INTERNET_RADIO_LOCATOR_DEBUG_MSG("gnome_internet_radio_locator->selected_streams_mime: %s\n",
	       gnome_internet_radio_locator->selected_streams_mime);
	GNOME_INTERNET_RADIO_LOCATOR_DEBUG_MSG("gnome_internet_radio_locator->selected_streams_codec: %s\n",
	       gnome_internet_radio_locator->selected_streams_codec);
	GNOME_INTERNET_RADIO_LOCATOR_DEBUG_MSG("gnome_internet_radio_locator->selected_streams_bitrate: %s\n",
	       gnome_internet_radio_locator->selected_streams_bitrate);
	GNOME_INTERNET_RADIO_LOCATOR_DEBUG_MSG("gnome_internet_radio_locator->selected_streams_samplerate: %s\n",
	       gnome_internet_radio_locator->selected_streams_samplerate);
	GNOME_INTERNET_RADIO_LOCATOR_DEBUG_MSG("gnome_internet_radio_locator->selected_channels: %0x\n",
	       gnome_internet_radio_locator->selected_streams_channels);

	gnome_config_pop_prefix();
#endif

	return gnome_internet_radio_locator_app;
}