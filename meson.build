project('c_colors', ['c'],default_options:['warning_level=0','c_std=gnu99'])
cc = meson.get_compiler('c')
#//////////////////////////////////////////////////////////////////////
meson_deps_dir = join_paths(meson.current_source_dir(), 'submodules/meson_deps')
pwd = meson.current_source_dir()
#//////////////////////////////////////////////////////////////////////
inc = [
  include_directories(
   '.','vendor','vendor/sqlite','rgb-ansi-utils','colors',
  ),
  include_directories(
   'submodules/meson_deps/submodules/c_ansi',
   'submodules/meson_deps/submodules/ee.c','submodules/meson_deps/submodules/ee.c/src',
   'submodules/meson_deps/submodules/list/src',
   'submodules/meson_deps/submodules','submodules/meson_deps',
  ),
]
#############################################################################
subdir('submodules')
#//////////////////////////////////////////////////////////////////////
subdir('db')
subdir('hex-png-pixel-utils')
subdir('rgb-ansi-utils')
subdir('colors-csv-parser')
subdir('colors-json-parser')
#subdir('parse-colors')
subdir('dbmem')
#//////////////////////////////////////////////////////////////////////
subdir('hex-png-pixel-creator')
#//////////////////////////////////////////////////////////////////////
subdir('dbmem-test')
#//////////////////////////////////////////////////////////////////////
subdir('colors')
subdir('colors-test')
