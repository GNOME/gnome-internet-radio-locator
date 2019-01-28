/* $Id$
 *
 * GNOME Internet Radio Locator
 *
 * Copyright (C) 2014-2019  Aamot Software
 *
 * Author: Ole Aamot <ole@gnome.org>
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

#ifndef GNOME_INTERNET_RADIO_LOCATOR_H
#define GNOME_INTERNET_RADIO_LOCATOR_H 1

#include <config.h>
#include <glib.h>

typedef enum {
	GNOME_INTERNET_RADIO_LOCATOR_CHANNELS_MONO = 0x0001,
	GNOME_INTERNET_RADIO_LOCATOR_CHANNELS_STEREO = 0x0002,
	GNOME_INTERNET_RADIO_LOCATOR_CHANNELS_5_1 = 0x0005,
	GNOME_INTERNET_RADIO_LOCATOR_CHANNELS_NONE = 0xffff
} GNOMEInternetRadioLocatorChannels;

typedef enum {
	GNOME_INTERNET_RADIO_LOCATOR_STREAM_SHOUTCAST = 0x0001,
	GNOME_INTERNET_RADIO_LOCATOR_STREAM_OGG = 0x0002,
	GNOME_INTERNET_RADIO_LOCATOR_STREAM_AAC = 0x0003
} GNOMEInternetRadioLocatorStreamType;

typedef enum {
	GNOME_INTERNET_RADIO_LOCATOR_STREAM_PLAYER = 0x0001,
	GNOME_INTERNET_RADIO_LOCATOR_STREAM_RECORD = 0x0002
} GNOMEInternetRadioLocatorHelperType;

#include "gnome-internet-radio-locator-listener.h"
#include "gnome-internet-radio-locator-program.h"
#include "gnome-internet-radio-locator-runners.h"
#include "gnome-internet-radio-locator-streams.h"
#include "gnome-internet-radio-locator-station.h"

#if GNOME_INTERNET_RADIO_LOCATOR_DEBUG == 0
#define GNOME_INTERNET_RADIO_LOCATOR_DEBUG_MSG(x...) g_message(x)
#else
#define GNOME_INTERNET_RADIO_LOCATOR_DEBUG_MSG(x...)
#endif

#define GNOME_INTERNET_RADIO_LOCATOR_RECORD_TRUE 1
#define GNOME_INTERNET_RADIO_LOCATOR_RECORD_FALSE 0
#define GNOME_INTERNET_RADIO_LOCATOR_PLAYER_TRUE 1
#define GNOME_INTERNET_RADIO_LOCATOR_PLAYER_FALSE 0

void show_error(gchar * msg);
void statusbar_send_msg(const char *a, ...);

void appbar_send_msg(const char *a, ...);
gint gnome_internet_radio_locator_archive_new(gchar *title, gchar *file, gchar *codec);

/* Fetcher for the channels */

void gnome_internet_radio_locator_helper_run(char *url, char *name, GNOMEInternetRadioLocatorStreamType type, GNOMEInternetRadioLocatorHelperType gnome_internet_radio_locator);
void on_search_button_clicked(GtkWidget * button, gpointer user_data);
void on_listen_button_clicked(GtkWidget * button, gpointer user_data);
void on_record_button_clicked(GtkWidget * button, gpointer user_data);
void on_stop_button_clicked(GtkWidget *a, gpointer user_data);
void on_next_station_click(GtkWidget *, gpointer user_data);
void on_new_station_clicked(GtkWidget *a, gpointer user_data);
void on_new_station_selector_changed(GtkWidget *a, gpointer user_data);
void on_previous_station_click(GtkWidget *, gpointer user_data);
void on_listeners_selector_button_clicked(GtkWidget *, gpointer user_data);
void on_listeners_selector_changed(GtkWidget * a, gpointer user_data);
void on_programs_selector_button_clicked(GtkWidget *, gpointer user_data);
void on_programs_selector_changed(GtkWidget * a, gpointer user_data);
void on_stations_selector_button_clicked(GtkWidget *, gpointer user_data);
void on_stations_selector_changed(GtkWidget * a, gpointer user_data);
void on_streams_selector_button_clicked(GtkWidget *, gpointer user_data);
void on_streams_selector_changed(GtkWidget * a, gpointer user_data);
void quit_app(GtkWidget *, gpointer user_data);
void about_app(GtkWidget *, gpointer user_data);
void about_listener(GtkWidget *, gpointer user_data);
void about_station(GSimpleAction *simple, GVariant *parameter, gpointer user_data);
void about_program(GSimpleAction *simple, GVariant *parameter, gpointer user_data);

gboolean on_search_matches(GtkEntryCompletion *widget,
			   GtkTreeModel *model,
			   GtkTreeIter *iter,
			   gpointer user_data);

struct _GNOMEInternetRadioLocatorData {
	GtkImage *pixmap;
	GtkProgressBar *progress;
	GSettings *settings;
	GtkAboutDialog *window;
	GtkStatusbar *statusbar;
	GtkWidget *player_window;
        GtkWidget *about_station;
	gchar *selected_archive_file;
	GNOMEInternetRadioLocatorListenerInfo *selected_listener;
	gchar *selected_listener_uri;
	gchar *selected_listener_name;
	gchar *selected_listener_location;
	gchar *selected_listener_band;
	gchar *selected_listener_description;
	GNOMEInternetRadioLocatorProgramInfo *selected_program;
	gchar *selected_program_uri;
	gchar *selected_program_name;
	gchar *selected_program_location;
	gchar *selected_program_band;
	gchar *selected_program_description;
	GNOMEInternetRadioLocatorRunnersInfo *selected_runners;
	gint timeout_id;
	gint current_station_number;
	GNOMEInternetRadioLocatorStationInfo *previous_station;
	GNOMEInternetRadioLocatorStationInfo *selected_station;
	gchar *selected_station_uri;
	gchar *selected_station_name;
	gchar *selected_station_location;
	gchar *selected_station_band;
        gchar *selected_station_description;
	gchar *selected_station_website;
	gint selected_bitrate;
	GNOMEInternetRadioLocatorStreamsInfo *selected_streams;
	gchar *selected_streams_mime;
	gchar *selected_streams_uri;
	gchar *selected_streams_codec;
	gchar *selected_streams_samplerate;
	gchar *selected_streams_stations;
	gchar *selected_streams_bitrate;
	GNOMEInternetRadioLocatorChannels selected_streams_channels;
	gint selected_samplerate;
	GdkPixbuf *icon;
	gint player_status;
	GPid player_pid;
	GSubprocess *player_subprocess;
	GSubprocess *record_subprocess;
	GSubprocessLauncher *player_launcher;
	GSubprocessLauncher *record_launcher;
	gint record_status;
	GPid record_pid;
	gint station_count;
	gint stream_count;
	GMainLoop *player_loop;
	GMainLoop *record_loop;  
};

typedef struct _GNOMEInternetRadioLocatorData GNOMEInternetRadioLocatorData;

void about_station(GSimpleAction *simple, GVariant *parameter, gpointer user_data);

extern GNOMEInternetRadioLocatorData *gnome_internet_radio_locator;
extern GList *gnome_internet_radio_locator_listeners;
extern GList *gnome_internet_radio_locator_programs;
extern GList *gnome_internet_radio_locator_stations;
extern GList *gnome_internet_radio_locator_streams;

#endif /* GNOME_INTERNET_RADIO_LOCATOR_H */
