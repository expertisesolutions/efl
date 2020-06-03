edje_cxx_suite_deps = [check_dep, edje_cxx]

executable('edje_cxx_suite_compile_test',
  'cxx_compile_test.cc',
  dependencies: [edje_cxx_suite_deps, check_dep],
  cpp_args : [
  '-DTESTS_BUILD_DIR="'+meson.current_build_dir()+'"',
  '-DTESTS_SRC_DIR="'+meson.current_source_dir()+'"']
)
