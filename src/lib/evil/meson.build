if target_machine.system() == 'windows'
  evil_deps = []
  evil_pub_deps = [getopt, pcre]
  evil_ext_deps = [getopt, pcre]
  evil_src = []

  evil_src += files([
    'evil_strings.c',
    'evil_vasprintf.h',
    'evil_dlfcn.c',
    'evil_fcntl.c',
    'evil_langinfo.c',
    'evil_libgen.c',
    'evil_locale.c',
    'evil_main.c',
    'evil_mman.c',
    'evil_stdio.c',
    'evil_stdlib.c',
    'evil_string.c',
    'evil_time.c',
    'evil_unistd.c',
    'evil_util.c',
    'evil_private.h',
    'evil_sys_time.c',
  ])

  evil_header_src = [
    'evil_private.h',
    'evil_api.h',
  ]

  psapi = cc.find_library('psapi')
  ole32 = cc.find_library('ole32')
  ws2_32 = cc.find_library('ws2_32')
  secur32 = cc.find_library('secur32')
  uuid = cc.find_library('uuid')

  evil_pub_deps += [psapi, ole32, ws2_32, secur32, uuid]
  evil_ext_deps += [psapi, ole32, ws2_32, secur32, uuid]

  evil_lib = library('evil', evil_src,
    c_args : [package_c_args, '-DEVIL_BUILD'],
    dependencies : evil_ext_deps,
    include_directories : [config_dir],
    install: true,
    version: meson.project_version(),
  )

  evil = declare_dependency(
    include_directories: [include_directories('.')],
    dependencies : evil_pub_deps,
    link_with: evil_lib,
    compile_args : ['-DPCRE_STATIC'],
  )

  install_headers(evil_header_src,
    install_dir : dir_package_include,
  )
else
  evil = declare_dependency()
  evil_src = []
  evil_ext_deps = []
endif

automatic_pkgfile = false

