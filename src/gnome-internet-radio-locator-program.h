/* $Id$
 *
 * GNOME Internet Radio Locator
 *
 * Copyright (C) 2014  Ole Aamot Software
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

#ifndef GNOME_INTERNET_RADIO_LOCATOR_PROGRAM_H
#define GNOME_INTERNET_RADIO_LOCATOR_PROGRAM_H

#include <gtk/gtk.h>
#include "gnome-internet-radio-locator.h"

typedef struct _GNOMEInternetRadioLocatorProgramInfo GNOMEInternetRadioLocatorProgramInfo;
typedef struct _GNOMEInternetRadioLocatorArchiveInfo GNOMEInternetRadioLocatorArchiveInfo;

struct _GNOMEInternetRadioLocatorProgramInfo {
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
	/*   gchar *category; */
	GNOMEInternetRadioLocatorArchiveInfo *archive;
	GNOMEInternetRadioLocatorProgramInfo *next;
	GNOMEInternetRadioLocatorProgramInfo *prev;
};

struct _GNOMEInternetRadioLocatorArchiveInfo {
	gchar *id;
	gchar *name;
	GNOMEInternetRadioLocatorArchiveInfo *next;
	gchar *mimetype;
	glong bitrate;
	glong samplerate;
	GNOMEInternetRadioLocatorChannels channels;
	gchar *uri;
};

GNOMEInternetRadioLocatorProgramInfo *gnome_internet_radio_locator_program_new (GNOMEInternetRadioLocatorProgramInfo * head,
				   gchar *id,
				   gchar *name,
				   gchar *date,
				   gchar *time,
				   gchar *file);

void gnome_internet_radio_locator_program_free(GNOMEInternetRadioLocatorProgramInfo * info);
GNOMEInternetRadioLocatorProgramInfo *gnome_internet_radio_locator_program_load_from_file(GNOMEInternetRadioLocatorProgramInfo * head,
					     char *filename);
GNOMEInternetRadioLocatorProgramInfo *gnome_internet_radio_locator_program_load_from_http(GNOMEInternetRadioLocatorProgramInfo * head,
					     gpointer data);

#endif /* GNOME_INTERNET_RADIO_LOCATOR_PROGRAM_H */
