SUBDIRS = icons

@INTLTOOL_DESKTOP_RULE@

DESKTOP_IN_FILES= gnome-internet-radio-locator.desktop.in.in
DESKTOP_FILES= $(DESKTOP_IN_FILES:.desktop.in.in=.desktop)

desktopdir = $(datadir)/applications
desktop_DATA = $(DESKTOP_FILES)

@INTLTOOL_XML_RULE@
appdatadir = $(datadir)/appdata
appdata_in_files = gnome-internet-radio-locator.appdata.xml.in
appdata_DATA = $(appdata_in_files:.xml.in=.xml)

gsettings_ENUM_NAMESPACE = org.gnome.gnome-internet-radio-locator
gsettings_ENUM_FILES = $(top_srcdir)/src/gnome-internet-radio-locator-gui.h
gsettings_SCHEMAS = org.gnome.gnome-internet-radio-locator.gschema.xml

@GSETTINGS_RULES@

# gsettingsconvertdir = $(datadir)/GConf/gsettings
# gsettingsconvert_DATA = eog.convert

# uidir = $(pkgdatadir)
# ui_DATA = \
# 	eog-toolbar.xml

# pkgconfigdir = $(libdir)/pkgconfig
# pkgconfig_DATA = gnome-internet-radio-locator.pc

EXTRA_DIST = $(DESKTOP_IN_FILES) $(appdata_in_files)

CLEANFILES = $(appdata_DATA)

DISTCLEANFILES = $(DESKTOP_FILES) $(gsettings_SCHEMAS)

-include $(top_srcdir)/git.mk
