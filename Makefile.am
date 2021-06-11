SUBDIRS = data doc help man src po

datadir = @datadir@

EXTRA_DIST = m4/ChangeLog  \
	AMFM \
	AUTHORS \
	ChangeLog \
	BROADCAST \
	DEBIAN \
	EFF \
	GNU \
	FEDORA \
	HACKING \
	Makefile \
	NEWS \
	README \
	THANKS \
	TODO \
	UBUNTU \
        gnome-internet-radio-locator.spec \
        gnome-internet-radio-locator.spec.in

DISTCLEANFILES =                \
        intltool-extract        \
        intltool-merge          \
        intltool-update

MAINTAINERCLEANFILES = \
	$(srcdir)/aclocal.m4 \
	$(srcdir)/config.guess \
	$(srcdir)/config.h.in \
	$(srcdir)/config.sub \
	$(srcdir)/depcomp \
	$(srcdir)/gtk-doc.make \
	$(srcdir)/install-sh \
	$(srcdir)/ltmain.sh \
	$(srcdir)/missing \
	$(srcdir)/mkinstalldirs

-include $(top_srcdir)/git.mk

ACLOCAL_AMFLAGS = -I m4

# Build ChangeLog from GIT history
ChangeLog:
	@if test -f $(top_srcdir)/.git/HEAD; then \
		git log --stat --after="Mon Jul 20 23:47:57 2009" > $@; \
	fi

dist: ChangeLog

.PHONY: ChangeLog
