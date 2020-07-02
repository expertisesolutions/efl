evil_test_src = files(
'evil_suite.c',
'evil_suite.h',
'evil_test_dlfcn.c',
'evil_test_main.c',
'evil_test_stdio.c',
'evil_test_stdlib.c',
'evil_test_unistd.c',
)

evil_test_exe = executable('evil_suite',
  include_directories : config_dir,
  sources : evil_test_src,
  dependencies: [m, check, eina, evil],
  c_args : [
  '-DTESTS_WD="`pwd`"',
  '-DTESTS_BUILD_DIR="'+meson.current_build_dir()+'"',
  '-DTESTS_SRC_DIR="'+meson.current_source_dir()+'"']
)

test('evil', evil_test_exe,
  env : test_env,
  timeout : 60,
)
