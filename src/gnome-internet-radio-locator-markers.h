/*
 * Copyright (C) 2008 Pierre-Luc Beaudoin <pierre-luc@pierlux.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include <champlain/champlain.h>

#ifndef GNOME_INTERNET_RADIO_LOCATOR_MARKERS_H
#define GNOME_INTERNET_RADIO_LOCATOR_MARKERS_H 1

ChamplainMarkerLayer *create_marker_layer (ChamplainView *view, ChamplainPathLayer **path);

void marker_function (ChamplainMarker *self, gdouble dx, gdouble dy, ClutterEvent *event, gpointer user_data);

#endif
