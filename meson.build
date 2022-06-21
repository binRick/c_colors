project('c_colors', ['c'],default_options:['warning_level=0','c_std=gnu99'])
cc = meson.get_compiler('c')
#//////////////////////////////////////////////////////////////////////
meson_deps_dir = join_paths(meson.current_source_dir(), 'submodules/meson_deps')
pwd = join_paths(meson.current_source_dir())
#meson_deps_dir = pwd+'/submodules/meson_deps'
#//////////////////////////////////////////////////////////////////////
inc = [
  include_directories(
   './',
  ),
]
#############################################################################
subdir('submodules/meson_deps/meson/args')
subdir('submodules/meson_deps/meson/inc')
#//////////////////////////////////////////////////////////////////////
subdir('submodules/meson_deps/meson/deps/m')
subdir('submodules/meson_deps/meson/deps/cargs')
subdir('submodules/meson_deps/meson/deps/c_string_buffer')
subdir('submodules/meson_deps/meson/deps/c_stringfn')
subdir('submodules/meson_deps/meson/deps/c_fsio')
subdir('submodules/meson_deps/meson/deps/djbhash')
subdir('submodules/meson_deps/meson/deps/parson')
subdir('submodules/meson_deps/meson/deps/fs')
subdir('submodules/meson_deps/meson/deps/b64')
subdir('submodules/meson_deps/meson/deps/bytes')
subdir('submodules/meson_deps/meson/deps/termpaint')
subdir('submodules/meson_deps/meson/deps/rgba')
subdir('submodules/meson_deps/meson/deps/strdup')
subdir('submodules/meson_deps/meson/deps/tempdir')
subdir('submodules/meson_deps/meson/deps/asprintf')
subdir('submodules/meson_deps/meson/deps/dbg')
subdir('submodules/meson_deps/meson/deps/progress')
subdir('submodules/meson_deps/meson/deps/smaz')
subdir('submodules/meson_deps/meson/deps/debug-memory')
subdir('submodules/meson_deps/meson/deps/c_timer')
subdir('submodules/meson_deps/meson/deps/embedded_palettes')
subdir('submodules/meson_deps/submodules/c_ansi/ansi-codes')
subdir('submodules/meson_deps/submodules/c_ansi/ansi-rgb-utils')
subdir('submodules/meson_deps/submodules/c_ansi/ansi-utils')
subdir('submodules/meson_deps/submodules/c_ansi/term-utils')
#//////////////////////////////////////////////////////////////////////
subdir('db')
subdir('hex-png-pixel-utils')
subdir('rgb-ansi-utils')
subdir('colors-csv-parser')
subdir('colors-json-parser')
subdir('parse-colors')
subdir('dbmem')
#//////////////////////////////////////////////////////////////////////
subdir('hex-png-pixel-creator')
#//////////////////////////////////////////////////////////////////////
subdir('dbmem-test')
#//////////////////////////////////////////////////////////////////////
