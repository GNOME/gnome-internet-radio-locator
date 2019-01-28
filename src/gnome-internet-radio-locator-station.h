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

#ifndef GNOME_INTERNET_RADIO_LOCATOR_STATION_H
#define GNOME_INTERNET_RADIO_LOCATOR_STATION_H

#include "gnome-internet-radio-locator.h"

typedef struct _GNOMEInternetRadioLocatorStationInfo GNOMEInternetRadioLocatorStationInfo;
typedef struct _GNOMEInternetRadioLocatorStreamInfo GNOMEInternetRadioLocatorStreamInfo;

typedef enum {
	STATION_LANG = 11,
	STATION_ID = 10,
	STATION_NAME = 0,
	STATION_RANK = 7,
	STATION_TYPE = 6,
	STATION_BAND = 5,
	STATION_FREQUENCY = 4,
	STATION_LOCATION = 1,
	STATION_DESCRIPTION = 3,
	STATION_BITRATE = 8,
	STATION_SAMPLERATE = 9,
	STATION_URI = 2
} GNOMEInternetRadioLocatorStationColumn;

struct _GNOMEInternetRadioLocatorStationInfo {
	GtkWidget *widget;
	gchar *id;
	gchar *name;
	gchar *rank;
	gchar *type;
	gchar *band;
	gchar *frequency;
	gchar *location;
	gchar *description;
	gchar *bitrate;
	gchar *samplerate;
	gchar *uri;
	gchar *lang;
	/*   gchar *category; */
	GNOMEInternetRadioLocatorStreamInfo *stream;
	GNOMEInternetRadioLocatorStationInfo *next;
	GNOMEInternetRadioLocatorStationInfo *prev;
};

struct _GNOMEInternetRadioLocatorStreamInfo {
	gchar *id;
	gchar *name;
	GNOMEInternetRadioLocatorStreamInfo *next;
	gchar *mimetype;
	glong bitrate;
	glong samplerate;
	GNOMEInternetRadioLocatorChannels channels;
	gchar *uri;
};

void show_error(gchar * msg);
void gnome_internet_radio_locator_station_free(GNOMEInternetRadioLocatorStationInfo * info);
GNOMEInternetRadioLocatorStationInfo *gnome_internet_radio_locator_station_load_from_file(GNOMEInternetRadioLocatorStationInfo * head,
					     char *filename);
GNOMEInternetRadioLocatorStationInfo *gnome_internet_radio_locator_station_load_from_http(GNOMEInternetRadioLocatorStationInfo * head,
					     gpointer data);
void gnome_internet_radio_locator_helper_run(char *url, char *name, GNOMEInternetRadioLocatorStreamType type, GNOMEInternetRadioLocatorHelperType gnome_internet_radio_locator);

gint gnome_internet_radio_locator_station_update (GNOMEInternetRadioLocatorStationInfo *head, gchar *station_band, gchar *station_description, gchar *station_name, gchar *station_location, gchar *station_uri, gchar *station_website);

#endif /* GNOME_INTERNET_RADIO_LOCATOR_STATION_H */
