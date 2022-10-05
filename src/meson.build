src_inc = include_directories('.')

gread_deps = [
  libadwaita_dep,
  gtk_dep,
  m_dep,
]

gread_incs = [
  top_inc,
  include_directories('.'),
]

sources = files()
subdir('theme')
subdir('gui')

libgread_generated_sources_dep = declare_dependency(
  sources: built_sources,
)

gread_deps += libgread_generated_sources_dep

libgread = static_library(
  'gread-private',
  sources: sources,
  include_directories: gread_incs,
  dependencies: gread_deps,
  install: false,
)

libgread_dep = declare_dependency(
  link_with: libgread,
  sources: built_sources,
  dependencies: gread_deps,
  include_directories: gread_incs,
)
executable(meson.project_name(),
           files('main.c'),
           dependencies: libgread_dep,
           install: true,
           install_dir: meson.project_source_root(),
          )
