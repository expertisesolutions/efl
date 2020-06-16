
if sys_windows

  # Freetype
  freetype_inc = include_directories('freetype.2.8.0.1/build/native/include/freetype')
  freetype_lib = static_library('freetype',
    include_directories: 'freetype.2.8.0.1/build/native/lib/x64/v141/static/Release'
  )
  freetype = declare_dependency(
    link_with: freetype_lib,
    include_directories: freetype_inc
  )

endif

