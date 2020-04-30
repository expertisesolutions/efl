dir_package_include = '.'

if sys_windows
  evil_unposix_src = []

  evil_unposix_header_src = [
    'fnmatch.h', # Removed by 36
    'libgen.h',
    'pthread.h',
    'semaphore.h', # Removed by #34
    'strings.h', # Removed by #42
  ]

  subdir('sys')

  evil_unposix_lib = static_library('evil_unposix',
    evil_unposix_src,
    include_directories: [include_directories('.'), config_dir],
    dependencies : [pcre_dep, getopt_dep],
    install: true,
    version: meson.project_version(),
  )

  evil_unposix = declare_dependency(
    include_directories: [include_directories('.')],
    dependencies: [pcre_dep, getopt_dep],
    link_with: evil_unposix_lib,
  )

  install_headers(evil_unposix_header_src,
    install_dir : dir_package_include,
  )
else
  evil_unposix = declare_dependency()
endif

automatic_pkgfile = (sys_windows == false)
