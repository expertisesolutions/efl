if sys_windows
  check_src = [
  ]

  check_header_src = [
    'check.h',
    'check_stdint.h',
    'check_error.h',
    'check_impl.h',
    'check_list.h',
    'check_log.h',
    'check_msg.h',
    'check_pack.h',
    'check_print.h',
    'check_str.h',
  ]

  check_lib = static_library('check',
    check_src,
    include_directories: [include_directories('.'), config_dir],
    dependencies : [],
    install: true,
    version: meson.project_version(),
  )

  check = declare_dependency(
    include_directories: [include_directories('.')],
    dependencies: [],
    link_with: check_lib,
  )

  install_headers(check_header_src,
    install_dir : dir_package_include,
  )
else
  check = declare_dependency()
endif
