project('c_colors', ['c'],
  default_options: [
   'warning_level=0',
   'c_std=gnu99',
  ]
)
config = configuration_data()
version = run_command('git', 'describe', '--tags').stdout().strip()
config.set('version', version)
add_project_arguments('-D_GNU_SOURCE', language : 'c')
add_project_arguments('-DDBG_H_DEF_ONCE', language : 'c')
add_project_arguments('-DDBG_H_NO_WARNING', language : 'c')
add_project_arguments('-DASSERTF_DEF_ONCE', language : 'c')
add_project_arguments('-DLOG_USE_COLOR', language : 'c')
add_project_arguments('-DLOG_LEVEL=TRACE', language : 'c')
add_project_arguments('-Wno-pragma-once-outside-header', language : 'c')
add_project_arguments('-Wno-incompatible-pointer-types', language : 'c')
add_project_arguments('-Wno-pointer-sign', language : 'c')
cc = meson.get_compiler('c')
#//////////////////////////////////////////////////////////////////////
pwd_bin = find_program('pwd', required : true)
pwd = run_command(pwd_bin, check: true).stdout().strip()
#//////////////////////////////////////////////////////////////////////
inc = include_directories(
  './',
  'db/',
  'embeds/',
  'submodules/',
  'submodules/c_ansi',
  'submodules/c_string_buffer/include',
  'submodules/c_stringfn/include',
  'submodules/c_fsio/include',
  'submodules/rgba/',
)
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
#subdir('submodules/meson_deps/meson/deps/c_ansi')
subdir('submodules/c_ansi/ansi-codes')
subdir('submodules/c_ansi/ansi-rgb-utils')
subdir('submodules/c_ansi/ansi-utils')
subdir('submodules/c_ansi/term-utils')
subdir('submodules/c_ansi/term-termpaint')
#//////////////////////////////////////////////////////////////////////
#//////////////////////////////////////////////////////////////////////
subdir('db')
subdir('rgb-ansi-utils')
subdir('hex-png-pixel-utils')
#//////////////////////////////////////////////////////////////////////
subdir('colors-csv-parser')
subdir('colors-json-parser')
#//////////////////////////////////////////////////////////////////////
subdir('parse-colors')
#//////////////////////////////////////////////////////////////////////
subdir('hex-png-pixel-creator')
#//////////////////////////////////////////////////////////////////////
subdir('dbmem')
subdir('dbmem-test')
#//////////////////////////////////////////////////////////////////////
