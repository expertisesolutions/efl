efl_deps = []
efl_pub_deps = []

efl_header_src = [
  'Efl.h',
  'Efl_MVVM_Common.h'
]

efl_src = ['efl_main.c']

efl_lib_deps = [eina]

if not sys_windows
  efl_lib_deps += m
endif

efl_lib = library('efl',
  efl_src,
  c_args : ['-DEFL_BUILD'],
  dependencies: efl_lib_deps,
  install: true,
  version : meson.project_version()
)

efl = declare_dependency(
  include_directories: [include_directories('.'), config_dir],
  link_with: efl_lib,
  dependencies: efl_deps + efl_pub_deps,
)

install_headers(efl_header_src,
  install_dir : dir_package_include,
)

