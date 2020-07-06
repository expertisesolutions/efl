if sys_windows

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

