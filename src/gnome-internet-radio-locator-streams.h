/* $Id$
 *
 * GNOME Internet Radio Locator for GNOME 3
 *
 * Copyright (C) 2014, 2018  Ole Aamot Software
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

#ifndef GNOME_INTERNET_RADIO_LOCATOR_STREAMS_H
#define GNOME_INTERNET_RADIO_LOCATOR_STREAMS_H

#include "gnome-internet-radio-locator.h"

typedef struct _GNOMEInternetRadioLocatorStreamsInfo GNOMEInternetRadioLocatorStreamsInfo;
typedef struct _GNOMEInternetRadioLocatorEncoderInfo GNOMEInternetRadioLocatorEncoderInfo;

struct _GNOMEInternetRadioLocatorStreamsInfo {
	GtkWidget *widget;
	gchar *mime;
	gchar *uri;
	gchar *codec;
	gchar *samplerate;
	gchar *streams;
	gchar *bitrate;
	GNOMEInternetRadioLocatorChannels channels;
	GNOMEInternetRadioLocatorEncoderInfo *encoder;
	GNOMEInternetRadioLocatorStreamsInfo *next;
	GNOMEInternetRadioLocatorStreamsInfo *prev;
};

struct _GNOMEInternetRadioLocatorEncoderInfo {
	gchar *id;
	gchar *name;
	GNOMEInternetRadioLocatorEncoderInfo *next;
	gchar *mimetype;
	glong bitrate;
	glong samplerate;
	GNOMEInternetRadioLocatorChannels channels;
	gchar *uri;
};

GNOMEInternetRadioLocatorStreamsInfo *gnome_internet_radio_locator_streams_new (GNOMEInternetRadioLocatorStreamsInfo * head,
				   gchar *mime,
				   gchar *uri,
				   gchar *codec,
				   gchar *samplerate,
				   gchar *channels,
	                           gchar *bitrate);

void gnome_internet_radio_locator_streams_free(GNOMEInternetRadioLocatorStreamsInfo * info);
GNOMEInternetRadioLocatorStreamsInfo *gnome_internet_radio_locator_streams_load_from_file(GNOMEInternetRadioLocatorStreamsInfo * head,
					     char *filename);
GNOMEInternetRadioLocatorStreamsInfo *gnome_internet_radio_locator_streams_load_from_http(GNOMEInternetRadioLocatorStreamsInfo * head,
					     gpointer data);

#endif /* GNOME_INTERNET_RADIO_LOCATOR_STREAMS_H */
