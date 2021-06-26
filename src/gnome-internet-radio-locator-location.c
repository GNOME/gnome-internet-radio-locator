/* $Id$
 *
 * GNOME Internet Radio Locator
 *
 * Copyright (C) 2021  Aamot Software
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

#include <config.h>
#include <gtk/gtk.h>
#include <gst/player/player.h>
#include <champlain/champlain.h>
#include <champlain-gtk/champlain-gtk.h>
#include <clutter-gtk/clutter-gtk.h>
#include <geocode-glib/geocode-glib.h>
#include <glib/gstdio.h>
#include <glib/gi18n.h>
#include <string.h>
#include <geoclue.h>

#include "gnome-internet-radio-locator.h"
#include "gnome-internet-radio-locator-gui.h"
#include "gnome-internet-radio-locator-markers.h"
#include "gnome-internet-radio-locator-player.h"

/* Commandline options */
static gint timeout = 30; /* seconds */
static GClueAccuracyLevel accuracy_level = GCLUE_ACCURACY_LEVEL_EXACT;
static gint time_threshold;
static GClueLocation *location;	
static gdouble altitude, speed, heading;
static GVariant *timestamp;
static GOptionEntry entries[] =
{
        { "timeout",
          't',
          0,
          G_OPTION_ARG_INT,
          &timeout,
          N_("Exit after T seconds. Default: 30"),
          "T" },
        { "time-threshold",
          'i',
          0,
          G_OPTION_ARG_INT,
          &time_threshold,
          N_("Only report location update after T seconds. "
             "Default: 0 (report new location without any delay)"),
          "T" },
        { "accuracy-level",
          'a',
          0,
          G_OPTION_ARG_INT,
          &accuracy_level,
          N_("Request accuracy level A. "
             "Country = 1, "
             "City = 4, "
             "Neighborhood = 5, "
             "Street = 6, "
             "Exact = 8."),
          "A" },
        { NULL }
};

GClueSimple *simple = NULL;
GClueClient *client = NULL;
GMainLoop *main_loop;

static void
print_location (GClueSimple *simple, ChamplainView *view)
{
        GClueLocation *location;
        gdouble altitude, speed, heading;
        GVariant *timestamp;
        GTimeVal tv = { 0 };
        const char *desc;

        location = gclue_simple_get_location (simple);
        GNOME_INTERNET_RADIO_LOCATOR_DEBUG_MSG ("\nNew location:\n");
        GNOME_INTERNET_RADIO_LOCATOR_DEBUG_MSG ("Latitude:    %f°\nLongitude:   %f°\nAccuracy:    %f meters\n",
                 gclue_location_get_latitude (location),
                 gclue_location_get_longitude (location),
                 gclue_location_get_accuracy (location));

	champlain_view_center_on (CHAMPLAIN_VIEW (view),
				  gclue_location_get_latitude (location),
				  gclue_location_get_longitude (location));
	
        altitude = gclue_location_get_altitude (location);
        if (altitude != -G_MAXDOUBLE)
                GNOME_INTERNET_RADIO_LOCATOR_DEBUG_MSG ("Altitude:    %f meters\n", altitude);
        speed = gclue_location_get_speed (location);
        if (speed >= 0)
                GNOME_INTERNET_RADIO_LOCATOR_DEBUG_MSG ("Speed:       %f meters/second\n", speed);
        heading = gclue_location_get_heading (location);
        if (heading >= 0)
                GNOME_INTERNET_RADIO_LOCATOR_DEBUG_MSG ("Heading:     %f°\n", heading);

        desc = gclue_location_get_description (location);
        if (strlen (desc) > 0)
                GNOME_INTERNET_RADIO_LOCATOR_DEBUG_MSG ("Description: %s\n", desc);

        timestamp = gclue_location_get_timestamp (location);
        if (timestamp) {
                GDateTime *date_time;
                gchar *str;

                g_variant_get (timestamp, "(tt)", &tv.tv_sec, &tv.tv_usec);

                date_time = g_date_time_new_from_timeval_local (&tv);
                str = g_date_time_format
                      (date_time,
                       "%c (%s seconds since the Epoch)");
                g_date_time_unref (date_time);

                GNOME_INTERNET_RADIO_LOCATOR_DEBUG_MSG ("Timestamp:   %s\n", str);
                g_free (str);
        }
}

static gboolean
on_location_timeout (gpointer user_data)
{
        g_clear_object (&client);
        g_clear_object (&simple);
        g_main_loop_quit (main_loop);

        return FALSE;
}

static void
on_client_active_notify (GClueClient *client,
                         GParamSpec *pspec,
                         gpointer    user_data)
{
        if (gclue_client_get_active (client))
                return;

        GNOME_INTERNET_RADIO_LOCATOR_DEBUG_MSG ("Geolocation disabled. Quitting..\n");
        on_location_timeout (NULL);
}

static void
on_simple_ready (GObject      *source_object,
                 GAsyncResult *res,
                 gpointer      user_data)
{
        GError *error = NULL;

        simple = gclue_simple_new_finish (res, &error);
        if (error != NULL) {
            g_critical ("Failed to connect to GeoClue2 service: %s", error->message);

            exit (-1);
        }
        client = gclue_simple_get_client (simple);
        if (client) {
                g_object_ref (client);
                GNOME_INTERNET_RADIO_LOCATOR_DEBUG_MSG ("Client object: %s\n",
                         g_dbus_proxy_get_object_path (G_DBUS_PROXY (client)));

                g_signal_connect (client,
                                  "notify::active",
                                  G_CALLBACK (on_client_active_notify),
                                  NULL);
        }
        print_location (simple, user_data);

        g_signal_connect (simple,
                          "notify::location",
                          G_CALLBACK (print_location),
                          user_data);
}

void location_main (ChamplainView *view) {
        gdouble altitude, speed, heading;
	gclue_simple_new ("gnome-internet-radio-locator",
			  accuracy_level,
			  time_threshold,
			  on_simple_ready,
			  CHAMPLAIN_VIEW (view));
	return;
}
