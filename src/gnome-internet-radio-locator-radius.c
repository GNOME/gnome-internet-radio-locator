/* $id$
 *
 * GNOME Internet Radio Locator
 *
 * Copyright (C) 2017  Ole Aamot Software
 *
 * Author: Ole Aamot <oka@oka.no>
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <geoclue/geoclue-geocode.h>

#define foreach(item, array) \
	for(int keep = 1, \
		    count = 0,\
		    size = sizeof (array) / sizeof *(array); \
	    keep && count != size; \
	    keep = !keep, count++) \
		for(item = (array) + count; keep; keep = !keep)

#define min(a,b) \
	({ typedef (a) _a = (a); \
		typeof (b) _b = (b); \
		_a < _b ? _a : _b; })
	
typedef struct _Position {
	char *url;
	float lat;
	float lon;
} Position;

int
gnome_internet_radio_locator_radius (float latitude, float longitude, int radius)
{
	int values[] = {0,1,2};
	Position pos;
        float xdelta;
	float ydelta;
	float zdelta;
	char *nearest_station;
	float shortest = 1000;
	Position stations[3][3] = {{"http://www.nrk.no/",60.18526,11.08224},
				   {"http://www.srib.no/",60.29230,5.22210},
				   {"http://www.nrksørlandet.no/",58.20733,8.07991}};
	foreach(int *v, values) {
		if ((stations[*v]->lat+radius >= latitude) && (stations[*v]->lon+radius >= longitude) && (stations[*v]->lat <= latitude+radius) && (stations[*v]->lon <= (longitude+radius))) {
			g_print ("value: %d = %s\n", *v, stations[*v]->url); 
			g_print ("value: %d = %4.2f\n", *v, stations[*v]->lat);
		        g_print ("value: %d = %4.2f\n", *v, stations[*v]->lon);
			xdelta = abs(latitude-stations[*v]->lat);
			ydelta = abs(longitude-stations[*v]->lon);
			zdelta = abs(xdelta-ydelta);
			if (shortest > zdelta) {
				printf("url = %s\n", stations[*v]->url);
				nearest_station = strdup(stations[*v]->url);
				shortest = zdelta;
			}
		}
	}
	g_print ("%s\n", nearest_station);
}
