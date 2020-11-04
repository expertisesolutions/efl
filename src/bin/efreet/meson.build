efreetd_src = files([
  'efreetd.c',
  'efreetd_cache.c',
])

efreetd_header_src = files([
  'efreetd.h',
  'efreetd_cache.h',
])

if get_option('efreetd')
  efreetd_src += files([
    'efreetd_cache_ipc.c',
  ])

  efreetd_header_src += files([
    'efreetd_cache_ipc.h',
  ])

  config_h.set('HAVE_EFREETD', '1')

  # disable strict as this causes efreet to be less functional for people
  #config_h.set('STRICT_SPEC', '1')

  efreetd = executable('efreetd',
          efreetd_src + efreetd_header_src,
          dependencies: [efreet, ecore, ecore_ipc, ecore_file, eio, intl],
          install: true,
          c_args : package_c_args,
  )
else
  efreetd_src += files([
    'efreetd_cache_local.c',
  ])
endif

efreet_bin_deps = [efreet, ecore, eet, ecore_file, intl]

efreet_desktop_cache_create = executable('efreet_desktop_cache_create',
        'efreet_desktop_cache_create.c',
        dependencies: efreet_bin_deps,
        install: true,
        install_dir : join_paths(dir_lib, 'efreet', version_name),
        c_args : package_c_args,
)

efreet_icon_cache_create = executable('efreet_icon_cache_create',
        'efreet_icon_cache_create.c',
        dependencies: efreet_bin_deps,
        install: true,
        install_dir : join_paths(dir_lib, 'efreet', version_name),
        c_args : package_c_args,
)

efreet_mime_cache_create = executable('efreet_mime_cache_create',
        'efreet_mime_cache_create.c',
        dependencies: efreet_bin_deps,
        install: true,
        install_dir : join_paths(dir_lib, 'efreet', version_name),
        c_args : package_c_args,
)
