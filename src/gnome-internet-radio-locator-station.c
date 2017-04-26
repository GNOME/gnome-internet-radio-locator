/* $id$
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

#include <config.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <sys/resource.h>
#include <glib.h>
#include <glib/gstdio.h>
#include <gio/gio.h>
#include <gtk/gtk.h>
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>
#include "gnome-internet-radio-locator.h"
#include "gnome-internet-radio-locator-station.h"

extern GNOMEInternetRadioLocatorData *gnome_internet_radio_locator;

void gnome_internet_radio_locator_helper_run(gchar *url, gchar *name, GNOMEInternetRadioLocatorStreamType type, GNOMEInternetRadioLocatorHelperType helper)
{
	GError *err = NULL;
	GTimeVal mtime;
	/* const char *mime_info = NULL; */
	/* GnomeVFSMimeApplication *app; */
	char *app = NULL, *command = NULL, *msg = NULL;
	char **argv = NULL;
	gint argc;
	gboolean    ret;
	GError *error = NULL;
	GCancellable *player_cancellable;
	GCancellable *record_cancellable;
	
	if (signal(SIGCHLD, SIG_IGN) == SIG_ERR) {
		perror(0);
		exit(1);
	}
	
	g_return_if_fail(url != NULL);
	GNOME_INTERNET_RADIO_LOCATOR_DEBUG_MSG("%s", url);

	/* mime_info = gnome_vfs_get_mime_type(url); */

	g_get_current_time(&mtime);

	/* app = (gchar *)gnome_vfs_mime_get_default_application (mime_info); */

	// gnome_internet_radio_locator_player_main(url, gnome_internet_radio_locator->selected_station_name);

	if (helper == GNOME_INTERNET_RADIO_LOCATOR_STREAM_PLAYER) {
		/* gnome_internet_radio_locator_player_main(url, gnome_internet_radio_locator->selected_station_name); */
	}

	if (helper == GNOME_INTERNET_RADIO_LOCATOR_STREAM_RECORD) {
		/* gnome_internet_radio_locator_record_main(url, gnome_internet_radio_locator->selected_station_name); */
	}

	return;

#if 0
	if (g_strcmp0(app,"no")!=0) {
		command = g_strconcat(app, " ", url, NULL);
		GNOME_INTERNET_RADIO_LOCATOR_DEBUG_MSG("Helper application is %s\n", command);
		if (type == GNOME_INTERNET_RADIO_LOCATOR_STREAM_SHOUTCAST) {
			if (helper == GNOME_INTERNET_RADIO_LOCATOR_STREAM_PLAYER) {
				command = g_strconcat(app, " ", url, NULL);
				/* argv[0] = g_strdup(app); */
				/* argv[1] = g_strdup(url); */
			}
			if (helper == GNOME_INTERNET_RADIO_LOCATOR_STREAM_RECORD) {

				GtkWidget *dialog;
				char *filename, *default_filename;
				GDate gdate;
				time_t t;
				struct tm *tmp;
				char outstr[200];
				t = time(NULL);
				tmp = localtime(&t);
				strftime(outstr, sizeof(outstr), "%a, %d %b %Y %T %z", tmp);
				dialog = gtk_file_chooser_dialog_new ("Save Recorded Broadcast File",
								      GTK_WINDOW(gnome_internet_radio_locator_app),
								      GTK_FILE_CHOOSER_ACTION_SAVE,
								      GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
								      GTK_STOCK_SAVE, GTK_RESPONSE_ACCEPT,
								      NULL);
				default_filename = g_strconcat(gnome_internet_radio_locator->selected_station_name, " - Broadcast Recording - ", outstr, ".mp3", NULL);  
				gtk_file_chooser_set_current_name (GTK_FILE_CHOOSER (dialog), default_filename);
				if (gtk_dialog_run (GTK_DIALOG (dialog)) == GTK_RESPONSE_ACCEPT)
				{
					filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (dialog));
					gnome_internet_radio_locator->selected_archive_file = g_strconcat(filename, NULL);
				}
				gtk_widget_destroy (dialog);
				
				/* printf("Archiving program at %s\n", archive); */
				
				appbar_send_msg(_("Recording from %s in %s to %s"),
						gnome_internet_radio_locator->selected_station_name,
						gnome_internet_radio_locator->selected_station_location,
						gnome_internet_radio_locator->selected_archive_file);		
				
				command = g_strconcat(app, " ", url, " -d ", g_get_home_dir(), "/.gnome_internet_radio_locator/ -D %D", NULL);
				GNOME_INTERNET_RADIO_LOCATOR_DEBUG_MSG("%s\n", command);

				// gnome_internet_radio_locator_archive_new("Archive", gnome_internet_radio_locator->selected_archive_file);
				
				/* " -d ", g_get_home_dir(), "/.gnome_internet_radio_locator -D \"", name, "\" -s -a -u gnome_internet_radio_locator/", VERSION, NULL); */
				/* command = g_strconcat(command, " -d ", g_get_home_dir(), "/.gnome_internet_radio_locator/", name, " -D %S%A%T -t 10 -u gnome_internet_radio_locator/", VERSION, NULL); */
			}
		        GNOME_INTERNET_RADIO_LOCATOR_DEBUG_MSG("Helper command is %s\n", command);
		}
		/* gnome_vfs_mime_application_free (app); */
	} else {
		if (helper == GNOME_INTERNET_RADIO_LOCATOR_STREAM_PLAYER) {
			msg = g_strdup_printf(_("An error happened trying to play "
						"%s\nEither the file doesn't exist, or you "
						"don't have a player for it."),
					      url);
		}
		if (helper == GNOME_INTERNET_RADIO_LOCATOR_STREAM_RECORD) {
			msg = g_strdup_printf(_("An error happened trying to record "
						"%s\nEither the file doesn't exist, or you "
						"don't have a recorder for it."),
					      url);
		}
		if (msg != NULL) {
			show_error(msg);
			g_free(msg);
		}
		return;
	}

	if (helper == GNOME_INTERNET_RADIO_LOCATOR_STREAM_PLAYER) {
#if 0
		g_shell_parse_argv(command,
				   &argc,
				   &argv,
				   NULL);
		ret = g_spawn_async_with_pipes (".",
						argv,
						NULL,
						G_SPAWN_SEARCH_PATH|G_SPAWN_STDOUT_TO_DEV_NULL|G_SPAWN_STDERR_TO_DEV_NULL|G_SPAWN_DO_NOT_REAP_CHILD,
						NULL,
						NULL,
						&gnome_internet_radio_locator->player_pid,
						NULL,
						NULL,
						NULL,
						&err);
		if( ! ret )
		{
			msg = g_strdup_printf(_("Failed to run %s (%i)\n"), command, gnome_internet_radio_locator->player_pid);
			show_error(msg);
			g_free(msg);
			return;
		}
		/* Add watch function to catch termination of the process. This function
		 * will clean any remnants of process. */
		g_child_watch_add( gnome_internet_radio_locator->player_pid, (GChildWatchFunc)cb_child_watch_player, gnome_internet_radio_locator);
		
		/* gnome_internet_radio_locator->player_pid = pid; */
		gnome_internet_radio_locator->player_status = GNOME_INTERNET_RADIO_LOCATOR_PLAYER_TRUE;
		/* Install timeout fnction that will move the progress bar */
		gnome_internet_radio_locator->timeout_id = g_timeout_add(100,(GSourceFunc)cb_timeout,gnome_internet_radio_locator);
/* #endif */
/* #if 0 */
		ret = g_spawn_async_with_pipes( NULL, /* command */ argv, NULL,
						G_SPAWN_DO_NOT_REAP_CHILD|G_SPAWN_DEFAULT, NULL,
						NULL, &gnome_internet_radio_locator->player_pid, NULL, &out, &error, NULL );
		if( ! ret )
		{
			msg = g_strdup_printf(_("Failed to run %s (%i)\n"), command, gnome_internet_radio_locator->player_pid);
			show_error(msg);
			g_free(msg);
			return;
		}
		/* Add watch function to catch termination of the process. This function
		 * will clean any remnants of process. */
		g_child_watch_add( gnome_internet_radio_locator->player_pid, (GChildWatchFunc)cb_child_watch_player, gnome_internet_radio_locator );
		/* Create channels that will be used to read gnome_internet_radio_locator from pipes. *
			
#ifdef G_OS_WIN32
		out_ch = g_io_channel_win32_new_fd( out );
		err_ch = g_io_channel_win32_new_fd( error );
#else
		out_ch = g_io_channel_unix_new( out );
		err_ch = g_io_channel_unix_new( error );
#endif
		*/

                /* Add watches to channels */
		g_io_add_watch( out_ch, G_IO_IN | G_IO_HUP, (GIOFunc)cb_out_watch, gnome_internet_radio_locator );
		g_io_add_watch( err_ch, G_IO_IN | G_IO_HUP, (GIOFunc)cb_err_watch, gnome_internet_radio_locator );
		/* Install timeout fnction that will move the progress bar */
		gnome_internet_radio_locator->timeout_id = g_timeout_add( 100, (GSourceFunc)cb_timeout, gnome_internet_radio_locator );
/* #endif */
#if 0
		if (!g_spawn_command_line_sync(command, stdout, stderr, status, &err)) {
			msg = g_strdup_printf(_("Failed to open URL: '%s'\n"
						"Status code: %i\n"
						"Details: %s"), url, status, err->message);
			show_error(msg);
			g_error_free(err);
			g_free(msg);
		} else {
			GNOME_INTERNET_RADIO_LOCATOR_DEBUG_MSG("Launching %s\n", command);
		}
		/* Add watch function to catch termination of the process. This function
		 * will clean any remnants of process. */
		g_child_watch_add( gnome_internet_radio_locator->player_pid, (GChildWatchFunc)cb_child_watch_player, gnome_internet_radio_locator );
#endif
		/* Original async player code */

		gnome_internet_radio_locator->player_launcher = g_subprocess_launcher_new (G_SUBPROCESS_FLAGS_NONE);
		gnome_internet_radio_locator->player_subprocess = g_subprocess_launcher_spawn (gnome_internet_radio_locator->player_launcher, &error, app, url, NULL);
		if (gnome_internet_radio_locator->player_subprocess == NULL) {
			msg = g_strdup_printf(_("Failed to open URL: '%s'\n"
						"Details: %s"), url, error->message);
			show_error(msg);
			g_error_free(error);
			g_free(msg);
			goto quit_player;
		} else {
			gnome_internet_radio_locator->player_status = GNOME_INTERNET_RADIO_LOCATOR_PLAYER_TRUE;
			GNOME_INTERNET_RADIO_LOCATOR_DEBUG_MSG("Launching %s player\n", command);
		}

#endif
#if 0
		if (!g_spawn_command_line_async(command, &err)) {
			msg = g_strdup_printf(_("Failed to open URL: '%s'\n"
						"Details: %s"), url, err->message);
			show_error(msg);
			g_error_free(err);
			g_free(msg);
		} else {
			gnome_internet_radio_locator->player_status = GNOME_INTERNET_RADIO_LOCATOR_PLAYER_TRUE;
			GNOME_INTERNET_RADIO_LOCATOR_DEBUG_MSG("Launching %s player\n", command);
			g_subprocess_wait_async (gnome_internet_radio_locator->player_subprocess,
						 player_cancellable,
						 (GAsyncReadyCallback)cb_subprocess_player,
						 NULL);
		}
#endif

	}
	
	if (helper == GNOME_INTERNET_RADIO_LOCATOR_STREAM_RECORD) {
		
/* #if 0 */
		/* gchar *argv[] = { command, NULL }; */
		/* Spawn child process */
		gchar *recording_path_name = g_strconcat(g_get_home_dir(), "/.gnome_internet_radio_locator/", NULL);
		gchar *formatting_argument = g_strdup("-D %D");
		gnome_internet_radio_locator->record_launcher = g_subprocess_launcher_new (G_SUBPROCESS_FLAGS_NONE);
		/* gnome_internet_radio_locator->record_subprocess = g_subprocess_launcher_spawn (gnome_internet_radio_locator->record_launcher, &error, GNOME_INTERNET_RADIO_LOCATOR_HELPER_RECORD, url, "-d", recording_path_name, "-D", "%D", NULL); */
		if (gnome_internet_radio_locator->record_subprocess == NULL) {
			/* msg = g_strdup_printf(_("Failed to run '%s'\n" */
			/* 			"Details: %s"), GNOME_INTERNET_RADIO_LOCATOR_HELPER_RECORD, error->message); */
			show_error(msg);
			g_error_free(error);
			g_free(msg);
			goto quit_record;
		} else {
			GNOME_INTERNET_RADIO_LOCATOR_DEBUG_MSG("Launching %s\n", command);
			g_subprocess_wait_async (gnome_internet_radio_locator->record_subprocess,
						 record_cancellable,
						 (GAsyncReadyCallback)cb_subprocess_record, NULL);
		}

#if 0
		g_shell_parse_argv(command,
				   &argc,
				   &argv,
				   NULL);
		ret = g_spawn_async_with_pipes (".",
						argv,
						NULL,
						G_SPAWN_SEARCH_PATH|G_SPAWN_STDOUT_TO_DEV_NULL|G_SPAWN_STDERR_TO_DEV_NULL|G_SPAWN_DO_NOT_REAP_CHILD,
						NULL,
						NULL,
						&gnome_internet_radio_locator->record_pid,
						NULL,
						NULL,
						NULL,
						&err);
		if( ! ret )
		{
			msg = g_strdup_printf(_("Failed to run %s (%i)\n"), command, gnome_internet_radio_locator->record_pid);
			show_error(msg);
			g_free(msg);
			return;
		}
		/* Add watch function to catch termination of the process. This function
		 * will clean any remnants of process. */
		g_child_watch_add( gnome_internet_radio_locator->record_pid, (GChildWatchFunc)cb_child_watch_record, gnome_internet_radio_locator);

		/* gnome_internet_radio_locator->record_pid = pid; */
		gnome_internet_radio_locator->record_status = GNOME_INTERNET_RADIO_LOCATOR_RECORD_TRUE;

		gnome_internet_radio_locator_archive_new("Archive", gnome_internet_radio_locator->selected_archive_file, gnome_internet_radio_locator->selected_streams_codec);
				
		/* Install timeout fnction that will move the progress bar */
		gnome_internet_radio_locator->timeout_id = g_timeout_add(100,(GSourceFunc)cb_timeout,gnome_internet_radio_locator);
/* #endif */
#if 0 
		ret = g_spawn_async_with_pipes( NULL, /* command */ argv, NULL,
						G_SPAWN_DO_NOT_REAP_CHILD|G_SPAWN_DEFAULT, NULL,
						NULL, &gnome_internet_radio_locator->record_pid, NULL, &out, &error, NULL );
		if( ! ret )
		{
			msg = g_strdup_printf(_("Failed to run %s (%i)\n"), command, gnome_internet_radio_locator->record_pid);
			show_error(msg);
			g_free(msg);
			return;
		}
		/* Add watch function to catch termination of the process. This function
		 * will clean any remnants of process. */
		g_child_watch_add( gnome_internet_radio_locator->record_pid, (GChildWatchFunc)cb_child_watch_record, gnome_internet_radio_locator );
		/* Create channels that will be used to read gnome_internet_radio_locator from pipes. */
#ifdef G_OS_WIN32
		out_ch = g_io_channel_win32_new_fd( out );
		err_ch = g_io_channel_win32_new_fd( error );
#else
		out_ch = g_io_channel_unix_new( out );
		err_ch = g_io_channel_unix_new( error );
#endif
		/* Add watches to channels */
		g_io_add_watch( out_ch, G_IO_IN | G_IO_HUP, (GIOFunc)cb_out_watch, gnome_internet_radio_locator );
		g_io_add_watch( err_ch, G_IO_IN | G_IO_HUP, (GIOFunc)cb_err_watch, gnome_internet_radio_locator );
		/* Install timeout fnction that will move the progress bar */
		gnome_internet_radio_locator->timeout_id = g_timeout_add( 100, (GSourceFunc)cb_timeout, gnome_internet_radio_locator );
#endif
#if 0
		if (!g_spawn_command_line_sync(command, stdout, stderr, status, &err)) {
			msg = g_strdup_printf(_("Failed to open URL: '%s'\n"
						"Status code: %i\n"
						"Details: %s"), url, status, err->message);
			show_error(msg);
			g_error_free(err);
			g_free(msg);
		} else {
			GNOME_INTERNET_RADIO_LOCATOR_DEBUG_MSG("Launching %s\n", command);
		}
		/* Add watch function to catch termination of the process. This function
		 * will clean any remnants of process. */
		g_child_watch_add( gnome_internet_radio_locator->record_pid, (GChildWatchFunc)cb_child_watch_record, gnome_internet_radio_locator );
#endif
#endif
	}

quit_player:

        if (gnome_internet_radio_locator->player_subprocess)
		g_object_unref(gnome_internet_radio_locator->player_subprocess);
        if (gnome_internet_radio_locator->player_launcher)
		g_object_unref(gnome_internet_radio_locator->player_launcher);
quit_record:
        if (gnome_internet_radio_locator->record_subprocess)
		g_object_unref(gnome_internet_radio_locator->record_subprocess);
        if (gnome_internet_radio_locator->record_launcher)
		g_object_unref(gnome_internet_radio_locator->record_launcher);
#endif
}

static void
gnome_internet_radio_locator_station_parser(GNOMEInternetRadioLocatorStationInfo * station, xmlDocPtr doc,
		    xmlNodePtr cur)
{
	xmlNodePtr sub;
	char *chans;

	g_return_if_fail(station != NULL);
	g_return_if_fail(doc != NULL);
	g_return_if_fail(cur != NULL);

	station->id = (gchar *)xmlGetProp(cur, (const xmlChar *)"id");
	GNOME_INTERNET_RADIO_LOCATOR_DEBUG_MSG("station->id = %s\n", station->id);
	station->name = (gchar *)xmlGetProp(cur, (const xmlChar *)"name");
	GNOME_INTERNET_RADIO_LOCATOR_DEBUG_MSG("station->name = %s\n", station->name);
	station->rank = (gchar *)xmlGetProp(cur, (const xmlChar *)"rank");
	GNOME_INTERNET_RADIO_LOCATOR_DEBUG_MSG("station->rank = %s\n", station->rank);
	station->type = (gchar *)xmlGetProp(cur, (const xmlChar *)"type");
	GNOME_INTERNET_RADIO_LOCATOR_DEBUG_MSG("station->type = %s\n", station->type);
	station->band = (gchar *)xmlGetProp(cur, (const xmlChar *)"band");
	GNOME_INTERNET_RADIO_LOCATOR_DEBUG_MSG("station->band = %s\n", station->band);

	sub = cur->xmlChildrenNode;

	while (sub != NULL) {

		if ((!xmlStrcmp(sub->name, (const xmlChar *) "frequency"))) {
			station->frequency = (gchar *)
			    xmlNodeListGetString(doc, sub->xmlChildrenNode,
						 1);
			GNOME_INTERNET_RADIO_LOCATOR_DEBUG_MSG("station->frequency = %s\n",
			    station->frequency);
		}

		if ((!xmlStrcmp(sub->name, (const xmlChar *) "location"))) {
			station->location = (gchar *)
			    xmlNodeListGetString(doc, sub->xmlChildrenNode,
						 1);
			GNOME_INTERNET_RADIO_LOCATOR_DEBUG_MSG("station->location = %s\n", station->location);

		}

		if ((!xmlStrcmp
		     (sub->name, (const xmlChar *) "description"))) {
			station->description = (gchar *)
			    xmlNodeListGetString(doc, sub->xmlChildrenNode,
						 1);
			GNOME_INTERNET_RADIO_LOCATOR_DEBUG_MSG("station->description = %s\n", station->description);
		}

		if ((!xmlStrcmp(sub->name, (const xmlChar *) "uri"))) {
			station->uri = (gchar *)
			    xmlNodeListGetString(doc, sub->xmlChildrenNode,
						 1);
			GNOME_INTERNET_RADIO_LOCATOR_DEBUG_MSG("station->uri = %s\n", station->uri);
			/* fprintf(stdout, "%s (%s)\n%s\n\n", station->name, station->location, station->uri); */
		}

		if ((!xmlStrcmp(sub->name, (const xmlChar *) "stream"))) {

			GNOMEInternetRadioLocatorStreamInfo *stream = g_new0(GNOMEInternetRadioLocatorStreamInfo, 1);

			gnome_internet_radio_locator->stream_count++;

			station->stream = stream;

			station->stream->mimetype = (gchar *)
				xmlGetProp(sub, (const xmlChar *)"mime");
			GNOME_INTERNET_RADIO_LOCATOR_DEBUG_MSG("station->stream->mimetype = %s\n",
			    station->stream->mimetype);
			if (xmlGetProp(sub, (const xmlChar *)"bitrate") != NULL) {
				station->stream->bitrate =
					(glong)atol((char *)xmlGetProp(sub, (const xmlChar *)"bitrate"));
				GNOME_INTERNET_RADIO_LOCATOR_DEBUG_MSG("station->stream->bitrate = %li\n",
				    station->stream->bitrate);
			}

			if (xmlGetProp(sub, (const xmlChar *)"samplerate") != NULL) {
				station->stream->samplerate = (glong) atol((char *)xmlGetProp(sub, (const xmlChar *)"samplerate"));
			}

			GNOME_INTERNET_RADIO_LOCATOR_DEBUG_MSG("station->stream->samplerate = %li\n",
			    station->stream->samplerate);
			station->stream->uri = (gchar *)xmlGetProp(sub, (const xmlChar *)"uri");
			GNOME_INTERNET_RADIO_LOCATOR_DEBUG_MSG("station->stream->uri = %s\n",
			    station->stream->uri);

			chans = (gchar *)xmlGetProp(sub, (const xmlChar *)"stations");

			if (chans != NULL) {
				if (strcmp(chans, "stereo") == 0) {
					station->stream->channels =
					    GNOME_INTERNET_RADIO_LOCATOR_CHANNELS_STEREO;
					GNOME_INTERNET_RADIO_LOCATOR_DEBUG_MSG("station->stream->channels = %d\n", station->stream->channels);
				} else if (strcmp(chans, "mono") == 0) {
					station->stream->channels =
					    GNOME_INTERNET_RADIO_LOCATOR_CHANNELS_MONO;
					GNOME_INTERNET_RADIO_LOCATOR_DEBUG_MSG("station->stream->channels = %d\n", station->stream->channels);
				} else if (strcmp(chans, "5:1") == 0) {
					station->stream->channels =
					    GNOME_INTERNET_RADIO_LOCATOR_CHANNELS_5_1;
					GNOME_INTERNET_RADIO_LOCATOR_DEBUG_MSG("station->stream->channels = %d\n", station->stream->channels);
				}
				g_free(chans);
			}

		}
		/* if... "stream" */
		sub = sub->next;
	}

	gnome_internet_radio_locator->station_count++;
	return;
}

GNOMEInternetRadioLocatorStationInfo *gnome_internet_radio_locator_station_load_from_http(GNOMEInternetRadioLocatorStationInfo * head,
					     gpointer data)
{
	GNOMEInternetRadioLocatorStationInfo *gstation;
	gstation = gnome_internet_radio_locator_station_load_from_file (head, "http://gnome-internet-radio-locator.src.oka.no/gnome_internet_radio_locator.xml");
	return gstation;
}

GNOMEInternetRadioLocatorStationInfo *gnome_internet_radio_locator_station_load_from_file(GNOMEInternetRadioLocatorStationInfo * head,
					     char *filename)
{
	xmlDocPtr doc = NULL;
	xmlNodePtr cur = NULL;
	GNOMEInternetRadioLocatorStationInfo *curr = NULL;
	char *version;
	GNOMEInternetRadioLocatorStationInfo *mem_station;

	g_return_val_if_fail(filename != NULL, NULL);

	doc = xmlReadFile(filename, NULL, 0);

	if (doc == NULL) {
		perror("xmlParseFile");
		xmlFreeDoc(doc);
		return NULL;
	}

	cur = xmlDocGetRootElement(doc);

	if (cur == NULL) {
		fprintf(stderr, "Empty document\n");
		xmlFreeDoc(doc);
		return NULL;
	}

	if (xmlStrcmp(cur->name, (const xmlChar *) "gnome_internet_radio_locator")) {
		fprintf(stderr,
			"Document of wrong type, root node != gnome_internet_radio_locator\n");
		xmlFreeDoc(doc);
		return NULL;
	}

	version = (gchar *)xmlGetProp(cur, (const xmlChar *)"version");

	GNOME_INTERNET_RADIO_LOCATOR_DEBUG_MSG("Valid GNOMEInternetRadioLocator %s XML document... Parsing stations...\n",
	    version);

	free(version);

	cur = cur->xmlChildrenNode;

	while (cur != NULL) {

		if ((!xmlStrcmp(cur->name, (const xmlChar *) "station"))) {

			GNOME_INTERNET_RADIO_LOCATOR_DEBUG_MSG("Found a new station.\n");

			curr = g_new0(GNOMEInternetRadioLocatorStationInfo, 1);
			mem_station = g_new0(GNOMEInternetRadioLocatorStationInfo, 1);

			gnome_internet_radio_locator_station_parser(curr, doc, cur);

			curr->next = head;

			head = curr;

			mem_station = head;

			gnome_internet_radio_locator_stations = g_list_append(gnome_internet_radio_locator_stations, (GNOMEInternetRadioLocatorStationInfo *)mem_station);

			GNOME_INTERNET_RADIO_LOCATOR_DEBUG_MSG("Done with parsing the station.\n");

		}
		cur = cur->next;
	}

	GNOME_INTERNET_RADIO_LOCATOR_DEBUG_MSG("Finished parsing XML document.\n");

	xmlFreeDoc(doc);

	return curr;
}

gint gnome_internet_radio_locator_station_update (GNOMEInternetRadioLocatorStationInfo *head, gchar *station_band, gchar *station_description, gchar *station_name, gchar *station_location, gchar *station_uri, gchar *station_website) {
	gchar msg[256];
	gchar *msg1, *msg2;
	/* Open ~/.gnome_internet_radio_locator/gnome_internet_radio_locator.xml.  Parse structure.  Insert new item.  Save structure. */
	GNOMEInternetRadioLocatorStationInfo *new_station;
	GNOMEInternetRadioLocatorStationInfo *stationinfo;
	/* GList *gnome_internet_radio_locator_local_stations = NULL; */
	gchar *local_station_uri, *local_station_name, *local_station_location, *local_station_band, *local_station_description, *local_station_website;
	gchar *stations = g_strconcat(g_get_home_dir(), "/.gnome_internet_radio_locator/gnome_internet_radio_locator.xml", NULL);

	gboolean local_gnome_internet_radio_locator_file = g_file_test (".gnome_internet_radio_locator/gnome_internet_radio_locator.xml", G_FILE_TEST_EXISTS);

	GNOME_INTERNET_RADIO_LOCATOR_DEBUG_MSG("local_gnome_internet_radio_locator_file = %i\n", local_gnome_internet_radio_locator_file);

	if (local_gnome_internet_radio_locator_file == 0) {

		gchar *local_gnome_internet_radio_locator_directory = g_strconcat(g_get_home_dir(), "/.gnome_internet_radio_locator", NULL);
		g_mkdir_with_parents (local_gnome_internet_radio_locator_directory, 0700);
		
	}
	
	FILE *fp;
	
	stationinfo = gnome_internet_radio_locator_station_load_from_file(NULL, stations);

	new_station = g_new0(GNOMEInternetRadioLocatorStationInfo, 1);
	new_station->name = g_strdup(station_name);
	new_station->band = g_strdup(station_band);
	new_station->description = g_strdup(station_description);
	new_station->location = g_strdup(station_location);
	new_station->stream = g_new0(GNOMEInternetRadioLocatorStreamInfo, 1);
	new_station->stream->uri = g_strdup(station_uri);
	new_station->uri = g_strdup(station_website);
	fp = g_fopen(stations, "w+");
	g_fprintf(fp, "<?xml version='1.0' encoding='UTF-8'?>\n<!DOCTYPE gnome_internet_radio_locator SYSTEM 'gnome_internet_radio_locator-9.1.dtd'>\n<gnome_internet_radio_locator version='9.1'>\n");
	// stationinfo-> = l->data;
	while (stationinfo != NULL) {
		local_station_uri = g_strdup(stationinfo->stream->uri);
		local_station_name = g_strdup(stationinfo->name);
		local_station_location = g_strdup(stationinfo->location);
		local_station_band = g_strdup(stationinfo->band);
		local_station_description = g_strdup(stationinfo->description);
		local_station_website = g_strdup(stationinfo->uri);
		/* FIXME: Save mime='audio/mp3' uri='%s' codec='MPEG 1 Audio, Layer 3 (MP3)' samplerate='24000 Hz' channels='Mono' bitrate='32 kbps' */
		g_fprintf(fp, "  <station band=\"%s\" id=\"%s\" lang=\"en\" name=\"%s\" rank=\"1.0\" type=\"org\">\n    <frequency uri=\"%s\">%s in %s</frequency>\n    <location>%s</location>\n    <description lang=\"en\">%s</description>\n    <stream uri=\"%s\" />\n    <uri>%s</uri>\n  </station>\n", local_station_band, local_station_name, local_station_name, local_station_website, local_station_band, local_station_location, local_station_location, local_station_description, local_station_uri, local_station_website);
		stationinfo = stationinfo->next;

	}
	g_fprintf(fp, "  <station band=\"%s\" id=\"%s\" lang=\"en\" name=\"%s\" rank=\"1.0\" type=\"org\">\n    <frequency uri=\"%s\">%s in %s</frequency>\n    <location>%s</location>\n    <description lang=\"en\">%s</description>\n    <stream uri=\"%s\" />\n    <uri>%s</uri>\n  </station>\n", new_station->band, new_station->name, new_station->name, new_station->uri, new_station->band, new_station->location, new_station->location, new_station->description, new_station->stream->uri, new_station->uri);
	g_fprintf(fp, "</gnome_internet_radio_locator>\n");
	fclose(fp);
	gnome_internet_radio_locator_stations = g_list_append(gnome_internet_radio_locator_stations, (GNOMEInternetRadioLocatorStationInfo *)new_station);
	g_free(stations);
	g_free(new_station);
	g_free(stationinfo);

	return (0);
}
