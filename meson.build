project('gread', 'c',
          version: '1.0.0',
    meson_version: '>= 0.59.0',
  default_options: [ 'warning_level=2',
                     'werror=false',
                     'cpp_std=gnu++2a',
                   ],
)
gread_version = meson.project_version()


gread_prefix    = get_option('prefix')
gread_bindir    = join_paths(gread_prefix, get_option('bindir'))
gread_localedir = join_paths(gread_prefix, get_option('localedir'))
gread_datadir   = join_paths(gread_prefix, get_option('datadir'))


cc             = meson.get_compiler('c')

config_h = configuration_data()

package_bugreport = 'https://github.com/Xalares/' +  meson.project_name() + '/issues'

# package
set_defines = [
  ['PACKAGE', meson.project_name()],
  ['PACKAGE_BUGREPORT', package_bugreport],
  ['PACKAGE_NAME', meson.project_name()],
  ['PACKAGE_STRING', '@0@ @1@'.format(meson.project_name(), gread_version)],
  ['PACKAGE_TARNAME', meson.project_name()],
  ['PACKAGE_URL', 'https://github.com/Xalares/gread/'],
  ['PACKAGE_VERSION', gread_version],
  ['VERSION', gread_version],
  ['GETTEXT_PACKAGE', meson.project_name()],
  ['DATADIR', gread_datadir],
]


foreach define: set_defines
  config_h.set_quoted(define[0], define[1])
endforeach

common_flags = [
  '-DHAVE_CONFIG_H',
  '-DPACKAGE_LOCALE_DIR="@0@"'.format(gread_localedir),
  '-DPACKAGE_DATA_DIR="@0@"'.format(gread_datadir)
]

add_project_arguments(common_flags, language: 'c')

gtk_dep        = dependency('gtk4', version:'>=4.8.1')
libadwaita_dep = dependency('libadwaita-1', version:'>=1.2.0')
m_dep          = cc.find_library('m')

i18n  = import('i18n')
gnome = import('gnome')

# define GETTEXT_PACKAGE

add_project_arguments('-DGETTEXT_PACKAGE="gread"', language:'c')

top_inc = include_directories('.')

src_inc  = join_paths(meson.project_source_root(), 'src')
po_inc   = join_paths(meson.project_source_root(), 'po')
data_inc = join_paths(meson.project_source_root(), 'data')

subdir('src')
subdir('po')
subdir('data')

configure_file(
  output: 'config.h',
  configuration: config_h
)

executable(meson.project_name(),
           sources,
           dependencies: libgread_dep,
           install: true,
           install_dir: gread_bindir,
          )
