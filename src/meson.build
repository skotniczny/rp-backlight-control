sources = files (
  'backlight.c',
  'brightness.c'
)

gtk = dependency ('gtk+-3.0')

deps = [ gtk ]

executable (
  meson.project_name(),
  sources,
  dependencies : deps,
  c_args : [
    '-DPACKAGE_DATA_DIR="' + resource_dir + '"',
    '-DGETTEXT_PACKAGE="' + meson.project_name() + '"'
  ],
  install : true,
  install_dir : bin_dir
)
