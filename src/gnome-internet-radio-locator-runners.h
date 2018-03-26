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

#ifndef GNOME_INTERNET_RADIO_LOCATOR_RUNNERS_H
#define GNOME_INTERNET_RADIO_LOCATOR_RUNNERS_H

typedef struct _GNOMEInternetRadioLocatorRunnersInfo GNOMEInternetRadioLocatorRunnersInfo;

struct _GNOMEInternetRadioLocatorRunnersInfo {
	GtkWidget *widget;
	GMutex *mutex;
	pid_t pid;
	gchar *name;
	gchar *date;
	gchar *time;
	gchar *file;
	gchar *command;
};

GNOMEInternetRadioLocatorRunnersInfo *gnome_internet_radio_locator_runners_new (pid_t pid,
				   gchar *name,
				   gchar *date,
				   gchar *time,
				   gchar *file);

void gnome_internet_radio_locator_runners_free(GNOMEInternetRadioLocatorRunnersInfo * info);
void gnome_internet_radio_locator_runners_mutex_get(GNOMEInternetRadioLocatorRunnersInfo *info);
void gnome_internet_radio_locator_runners_mutex_release(GNOMEInternetRadioLocatorRunnersInfo *info);
void gnome_internet_radio_locator_runners_mutex_lock(GNOMEInternetRadioLocatorRunnersInfo *info);
void gnome_internet_radio_locator_runners_mutex_unlock(GNOMEInternetRadioLocatorRunnersInfo *info);

void cb_record_execute(GtkButton *button);

#endif /* GNOME_INTERNET_RADIO_LOCATOR_RUNNERS_H */
