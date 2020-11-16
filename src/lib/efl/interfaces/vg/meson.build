if not get_option('ector')
  subdir_done()
endif

pub_eo_files = [
  'efl_text_style.eo',
]

foreach eo_file : pub_eo_files
  pub_eo_file_target += custom_target('eolian_gen_' + eo_file,
    input : eo_file,
    output : [eo_file + '.h'],
    depfile : eo_file + '.d',
    install : true,
    install_dir : dir_package_include / 'interfaces' / 'vg',
    command : eolian_gen + [ '-I', meson.current_source_dir() / '..', eolian_include_directories,
                           '-o', 'h:' + meson.current_build_dir() / eo_file + '.h',
                           '-o', 'c:' + meson.current_build_dir() / eo_file + '.c',
                           '-o', 'd:' + meson.current_build_dir() / eo_file + '.d',
                           '-gchd', '@INPUT@'])
endforeach
