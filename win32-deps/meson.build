
if sys_windows

  # Libjpeg-Turbo
  libjpeg_inc = include_directories('Libjpeg-Turbo.1.5.15/build/native/include')
  libjpeg_lib = static_library('libjpeg', 
    include_directories: 'Libjpeg-Turbo.1.5.15/lib/native/v140/windesktop/msvcstl/x64/Release/md'
  )
  libjpeg = declare_dependency(
    link_with: freetype_lib,
    include_directories: freetype_inc
  )

endif

