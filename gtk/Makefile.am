AM_CPPFLAGS = $(GNOME_INTERNET_RADIO_LOCATOR_CFLAGS) \
	-DGTK4_GNOME_RADIO_DATADIR="\"$(pkgdatadir)\"" \
	-DGTK4_GNOME_RADIO_LOCALEDIR="\"$(localedir)\""

AM_CFLAGS = $(GNOME_INTERNET_RADIO_LOCATOR_CFLAGS) \
	-DGTK4_GNOME_RADIO_DATADIR="\"$(pkgdatadir)\"" \
	-DGTK4_GNOME_RADIO_LOCALEDIR="\"$(localedir)\""
	$(NULL)

AM_LDFLAGS= $(GNOME_INTERNET_RADIO_LOCATOR_LIBS) $(GTK4_GNOME_RADIO_LIBS) 

bin_PROGRAMS = org.gnome.Radio

org_gnome_Radio_SOURCES = \
		org.gnome.Radio.c

org_gnome_Radio_CFLAGS  = $(GNOME_INTERNET_RADIO_LOCATOR_CFLAGS) \
	       -DDATADIR=\"$(datadir)\" \
	       -DGTK4_GNOME_RADIO_DEBUG=1 \
	       -DGTK4_GNOME_RADIO_CFG \
               -DGTK4_GNOME_RADIO_LOCALEDIR="\"$(localedir)\""

org_gnome_Radio_LDADD = $(GNOME_INTERNET_RADIO_LOCATOR_LIBS)

org_gnome_Radiodir = $(datadir)/gnome-internet-radio-locator

org_gnome_Radio_DATA = org.gnome.Radio.dtd org.gnome.Radio.xml org.gnome.Radio.xsl

EXTRA_DIST = org.gnome.Radio.c org.gnome.Radio.dtd org.gnome.Radio.xml org.gnome.Radio.xsl

LDADD = $(GNOME_INTERNET_RADIO_LOCATOR_LIBS)
