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

#ifndef GNOME_INTERNET_RADIO_LOCATOR_LISTENER_H
#define GNOME_INTERNET_RADIO_LOCATOR_LISTENER_H

#include <gtk/gtk.h>
#include <glib.h>
#include "gnome-internet-radio-locator.h"

typedef struct _GNOMEInternetRadioLocatorListenerInfo GNOMEInternetRadioLocatorListenerInfo;
typedef struct _GNOMEInternetRadioLocatorLocationInfo GNOMEInternetRadioLocatorLocationInfo;

struct _GNOMEInternetRadioLocatorListenerInfo {
	gchar *id;
	gchar *location;
	gchar *mail;
	gchar *name;
	gchar *pass;
	gchar *uri;
	gchar *band;
	gchar *description;
	GNOMEInternetRadioLocatorLocationInfo *locationinfo;
	GNOMEInternetRadioLocatorListenerInfo *next;
};

struct _GNOMEInternetRadioLocatorLocationInfo {
	gchar *name;
	gchar *link;
	gchar *glat;
	gchar *glon;
	gchar *grad;
	gchar *vote;
	gchar *rack;
};

GNOMEInternetRadioLocatorListenerInfo *gnome_internet_radio_locator_listener_load_from_file(GNOMEInternetRadioLocatorListenerInfo * head,
					       char *filename);
GNOMEInternetRadioLocatorListenerInfo *gnome_internet_radio_locator_listener_load_from_http(GNOMEInternetRadioLocatorListenerInfo * head,
					       gpointer data);

#endif /* GNOME_INTERNET_RADIO_LOCATOR_LISTENER_H */
