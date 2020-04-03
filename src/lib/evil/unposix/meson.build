dir_package_include = '.'

if sys_windows
  evil_unposix_src = []
  evil_unposix_header_src = [
    'fnmatch.h',
    'libgen.h',
    'limits.h',
    'locale.h',
    'math.h',
    'pthread.h',
    'stdlib.h',
    'string.h',
    'strings.h',
    'unimplemented.h',
    'unistd.h',
  ]

  subdir('sys')

  evil_unposix_lib = static_library('evil_unposix',
    evil_unposix_src,
    c_args: ['-DUNPOSIX_SOURCE', '-DWIN32_LEAN_AND_MEAN'],
    include_directories: [include_directories('.'), config_dir],
    dependencies : [pcre_dep, getopt_dep],
    install: true,
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
