src = files([
  'elm_web_none.c'
])

if sys_native_windows
  mod_name = 'module'
else
  mod_name = 'none'
endif

shared_module(mod_name,
    src,
    dependencies: [elementary, elementary_deps],
    install: true,
    install_dir : mod_install_dir,
    name_suffix : sys_mod_extension
)

mod = 'none'
