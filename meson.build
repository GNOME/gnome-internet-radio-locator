project('gnome-internet-radio-locator', 'c',
  version : '3.4.0',
  meson_version: '>= 0.49.2',
  license : 'GPL')
prefix = get_option('prefix')
localedir = join_paths(prefix, get_option('localedir'))
arguments = join_paths('-DGNOME_INTERNET_RADIO_LOCATOR_LOCALEDIR=', localedir)
add_project_arguments('-DDEBUG=0', language : 'c')
add_project_arguments('-DGETTEXT_PACKAGE="gnome-internet-radio-locator"', language:'c')
add_project_arguments(arguments, language:'c')
glib_dep = dependency('glib-2.0')
gtk3_dep = dependency('gtk+-3.0')
gst_dep = dependency('gstreamer-player-1.0')
champlain_gtk_dep = dependency('champlain-gtk-0.12')
champlain_dep = dependency('champlain-0.12')
geocode_dep = dependency('geocode-glib-1.0')
geoclue_dep = dependency('libgeoclue-2.0')
libxml_dep = dependency('libxml-2.0')
cc = meson.get_compiler('c')
config_h = configuration_data()
config_h.set_quoted('GETTEXT_PACKAGE', meson.project_name())
config_h.set_quoted('GNOME_INTERNET_RADIO_LOCATOR_LOCALEDIR', localedir)
configure_file(output: 'config.h', configuration: config_h)
m_dep = cc.find_library('m', required : false)
inc = include_directories('src')
subdir('src')
pkg_mod = import('pkgconfig')
pkg_mod.generate(libraries : gnomeinternetradiolibrary,
                 version : '3.4.0',
                 name : 'libgnomeinternetradiolibrary',
                 filebase : 'gnomeinternetradiolibrary',
                 description : 'GNOME Internet Radio Library')
