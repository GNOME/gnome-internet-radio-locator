project('gnome-internet-radio-locator', 'c',
  version : '1.0.3',
  license : 'GPL')
add_project_arguments('-DDEBUG=0', language : 'c')
as_version = meson.project_version() # set in project() below
conf = configuration_data()
conf.set_quoted('VERSION', as_version)
conf.set_quoted('GNOME_INTERNET_RADIO_LOCATOR_DATADIR', join_paths(join_paths(get_option('prefix'), get_option('datadir')), meson.project_name()))
configure_file(output : 'config.h', configuration : conf)
glib_dep = dependency('glib-2.0')
gtk3_dep = dependency('gtk+-3.0')
gst_dep = dependency('gstreamer-player-1.0')
champlain_gtk_dep = dependency('champlain-gtk-0.12')
champlain_dep = dependency('champlain-0.12')
geocode_dep = dependency('geocode-glib-1.0')
libxml_dep = dependency('libxml-2.0')
cc = meson.get_compiler('c')
m_dep = cc.find_library('m', required : false)
inc = include_directories('src')
subdir('src')
pkg_mod = import('pkgconfig')
pkg_mod.generate(libraries : gnomeinternetradiolocatorlib,
                 version : '1.0.3',
                 name : 'libgnomeinternetradiolocator',
                 filebase : 'gnomeinternetradiolocator',
                 description : 'A Library for Free Internet Radio')
