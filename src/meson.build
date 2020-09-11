as_version = meson.project_version() # set in project() below
ver_arr = as_version.split('.')
as_major_version = ver_arr[0]
as_minor_version = ver_arr[1]
as_micro_version = ver_arr[2]
conf = configuration_data()
conf.set_quoted('VERSION', as_version)
conf.set_quoted('GNOME_INTERNET_RADIO_LOCATOR_DATADIR', join_paths(join_paths(get_option('prefix'), get_option('datadir')), meson.project_name()))
conf.set_quoted('GNOME_INTERNET_RADIO_LOCATOR_LOCALEDIR', join_paths(join_paths(get_option('prefix'), get_option('localedir')), meson.project_name()))
configure_file(output : 'config.h', configuration : conf)
gnomeinternetradio_sources = ['gnome-internet-radio-locator.c', 'gnome-internet-radio-locator-gui.c', 'gnome-internet-radio-locator-listener.c', 'gnome-internet-radio-locator-markers.c', 'gnome-internet-radio-locator-player.c', 'gnome-internet-radio-locator-program.c', 'gnome-internet-radio-locator-radius.c', 'gnome-internet-radio-locator-runners.c', 'gnome-internet-radio-locator-station.c', 'gnome-internet-radio-locator-stations-map.c', 'gnome-internet-radio-locator-streams.c', 'gnome-internet-radio-locator-tz.c', 'gnome-internet-radio-locator.h', 'gnome-internet-radio-locator-gui.h', 'gnome-internet-radio-locator-listener.h', 'gnome-internet-radio-locator-markers.h', 'gnome-internet-radio-locator-player.h', 'gnome-internet-radio-locator-program.h', 'gnome-internet-radio-locator-radius.h', 'gnome-internet-radio-locator-runners.h', 'gnome-internet-radio-locator-station.h', 'gnome-internet-radio-locator-stations-map.h', 'gnome-internet-radio-locator-streams.h', 'gnome-internet-radio-locator-tz.h' ]
gnomeinternetradiolibrary = shared_library('gnomeinternetradiolibrary', gnomeinternetradio_sources, dependencies : [glib_dep, gtk3_dep, gst_dep, champlain_gtk_dep, champlain_dep, geocode_dep, geoclue_dep, libxml_dep, m_dep], install : true)
install_data(
    meson.project_name() + '.xml',
    install_dir: join_paths(get_option('datadir'), 'gnome-internet-radio-locator')
)
gnome = import('gnome')
gnomeinternetradiolocator_resources = gnome.compile_resources(
  'gnome-internet-radio-locator resources',
  'gnome-internet-radio-locator.gresource.xml',
  source_dir: '.'
)
gnomeinternetradiolocator_schemas = gnome.compile_schemas()
gnomeinternetradiolocator = executable('gnome-internet-radio-locator', gnomeinternetradio_sources, gnomeinternetradiolocator_resources, dependencies: [glib_dep, gtk3_dep, gst_dep, champlain_gtk_dep, champlain_dep, geocode_dep, geoclue_dep, libxml_dep, m_dep], link_with: gnomeinternetradiolibrary, install: true)
