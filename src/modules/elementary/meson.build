mods = [
  'access_output',
  'prefs',
  'test_entry',
  'test_map',
  join_paths('web', 'none')
]

foreach mod : mods
  if sys_native_windows
    mod_name = 'module'
  else
    mod_name = mod
  endif
  mod_install_dir = join_paths(dir_lib, 'elementary', 'modules', mod, version_name)
  subdir(mod)
  module_files += join_paths(mod_install_dir, 'lib' + mod_name + '.' + sys_mod_extension)
endforeach
