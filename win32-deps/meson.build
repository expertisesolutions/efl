if sys_windows

  # Freetype2
  freetype2_inc_dir = include_directories('freetype.2.8.0.1/build/native/include')
  freetype2_lib_dir = 'freetype.2.8.0.1/build/native/lib/x64/v141/static/Release'
  freetype2_lib = cc.find_library('freetype28',
    dirs: meson.build_root() + '/../win32-deps/' + freetype2_lib_dir,
    required: true,
  )
  freetype2 = declare_dependency(
    dependencies: freetype2_lib,
    include_directories: freetype2_inc_dir,
  )

  # Libjpeg-Turbo
  jpeg_inc_dir = include_directories('Libjpeg-Turbo.1.5.15/build/native/include')
  jpeg_lib_dir = 'Libjpeg-Turbo.1.5.15/lib/native/v140/windesktop/msvcstl/x64/Debug/md'
  jpeg_lib = cc.find_library('libjpeg-turbo',
    dirs: meson.build_root() + '/../win32-deps/' + jpeg_lib_dir,
    required: true,
  )

  jpeg = declare_dependency(
    dependencies: jpeg_lib,
    include_directories: jpeg_inc_dir,
  )

endif

