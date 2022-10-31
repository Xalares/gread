project('gread', 'c',
          version: '0.1.0',
    meson_version: '>= 0.59.0',
  default_options: [ 'warning_level=2',
                     'werror=false',
                     'cpp_std=gnu++2a',
                     'prefix=./install',
                   ],
)

gread_prefix    = get_option('prefix')
gread_bindir    = join_paths(gread_prefix, get_option('bindir'))
gread_localedir = join_paths(gread_prefix, get_option('localedir'))
gread_datadir   = join_paths(gread_prefix, get_option('datadir'))


cc             = meson.get_compiler('c')
gtk_dep        = dependency('gtk4', version:'>=4.8.1')
libadwaita_dep = dependency('libadwaita-1', version:'>=1.2.0')
m_dep          = cc.find_library('m')

i18n  = import('i18n')
gnome = import('gnome')

top_inc = include_directories('.')

src_inc  = join_paths(meson.project_source_root(), 'src')
po_inc   = join_paths(meson.project_source_root(), 'po')
data_inc = join_paths(meson.project_source_root(), 'data')
subdir('src')
subdir('po')
#subdir('data')
