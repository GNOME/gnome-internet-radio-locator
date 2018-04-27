project('gnome-internet-radio-locator', 'c',
  version : '1.1.3',
  license : 'GPL')
add_project_arguments('-DDEBUG=0', language : 'c')
glib_dep = dependency('glib-2.0')
gtk3_dep = dependency('gtk+-3.0')
gst_dep = dependency('gstreamer-player-1.0')
champlain_gtk_dep = dependency('champlain-gtk-0.12')
champlain_dep = dependency('champlain-0.12')
geocode_dep = dependency('geocode-glib-1.0')
libxml_dep = dependency('libxml-2.0')
gsettings_dep = dependency('gsettings-desktop-schemas', version: '>= 2.91.92')
cc = meson.get_compiler('c')
m_dep = cc.find_library('m', required : false)
inc = include_directories('src')
subdir('src')
pkg_mod = import('pkgconfig')
pkg_mod.generate(libraries : gnomeinternetradiolibrary,
                 version : '1.1.3',
                 name : 'libgnomeinternetradiolibrary',
                 filebase : 'gnomeinternetradiolibrary',
                 description : 'GNOME Internet Radio Library')
