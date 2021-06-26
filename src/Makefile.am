AM_CPPFLAGS = $(GNOME_INTERNET_RADIO_LOCATOR_CFLAGS) \
	-DGNOME_INTERNET_RADIO_LOCATOR_DATADIR="\"$(pkgdatadir)\"" \
	-DGNOME_INTERNET_RADIO_LOCATOR_LOCALEDIR="\"$(localedir)\""

AM_CFLAGS = $(GNOME_INTERNET_RADIO_LOCATOR_CFLAGS) \
	-DGNOME_INTERNET_RADIO_LOCATOR_DATADIR="\"$(pkgdatadir)\"" \
	-DGNOME_INTERNET_RADIO_LOCATOR_LOCALEDIR="\"$(localedir)\""
	$(NULL)

AM_LDFLAGS= $(GNOME_INTERNET_RADIO_LOCATOR_LIBS) 

bin_PROGRAMS = gnome-internet-radio-locator

gnome_internet_radio_locator_SOURCES = \
		gnome-internet-radio-locator-gui.c \
		gnome-internet-radio-locator-listener.c \
		gnome-internet-radio-locator-location.c \
		gnome-internet-radio-locator-markers.c \
		gnome-internet-radio-locator-player.c \
		gnome-internet-radio-locator-program.c \
		gnome-internet-radio-locator-radius.c \
		gnome-internet-radio-locator-runners.c \
		gnome-internet-radio-locator-station.c \
		gnome-internet-radio-locator-stations-map.c \
		gnome-internet-radio-locator-streams.c \
		gnome-internet-radio-locator-tz.c \
		gnome-internet-radio-locator.c

gnome_internet_radio_locator_CFLAGS  = $(GNOME_INTERNET_RADIO_LOCATOR_CFLAGS) \
	       -DGNOME_INTERNET_RADIO_LOCATOR_DATADIR=\"$(datadir)/gnome-internet-radio-locator\" \
	       -DDATADIR=\"$(datadir)\" \
	       -DGNOME_INTERNET_RADIO_LOCATOR_DEBUG=1 \
	       -DGNOME_INTERNET_RADIO_LOCATOR_CFG \
               -DGNOME_INTERNET_RADIO_LOCATOR_LOCALEDIR="\"$(localedir)\""

gnome_internet_radio_locator_LDADD = $(GNOME_INTERNET_RADIO_LOCATOR_LIBS)

gnomeinternetradiolocatordir = $(datadir)/gnome-internet-radio-locator

gnomeinternetradiolocator_DATA = gnome-internet-radio-locator-5.0.dtd gnome-internet-radio-locator.xml

EXTRA_DIST = gnome-internet-radio-locator.h gnome-internet-radio-locator-location.h gnome-internet-radio-locator-player.h gnome-internet-radio-locator-keys.h gnome-internet-radio-locator-markers.h gnome-internet-radio-locator-player-kb.h gnome-internet-radio-locator-player-resource.h gnome-internet-radio-locator-radius.h gnome-internet-radio-locator-station.h gnome-internet-radio-locator-stations-map.h gnome-internet-radio-locator-tz.h gnome-internet-radio-locator-gui.h gnome-internet-radio-locator-listener.h gnome-internet-radio-locator-program.h gnome-internet-radio-locator-runners.h gnome-internet-radio-locator-streams.h gnome-internet-radio-locator-5.0.dtd gnome-internet-radio-locator.xml gnome-internet-radio-locator.xsl gnome-internet-radio-locator.gresource.xml

LDADD = $(GNOME_INTERNET_RADIO_LOCATOR_LIBS)

noinst_HEADERS = gnome-internet-radio-locator-player-renderer.h
