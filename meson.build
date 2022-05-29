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
m_dep = cc.find_library('m', required : true)
cargs_dep = dependency('cargs',fallback:['cargs', 'cargs_dep'])
parson_shared_dep = dependency('parson',fallback:['parson', 'parson_shared_dep'])
#//////////////////////////////////////////////////////////////////////
subdir('meson/deps/asprintf')
subdir('meson/deps/c_string_buffer')
subdir('meson/deps/c_fsio')
subdir('meson/deps/rgba')
subdir('meson/deps/progress')
subdir('meson/deps/dbg')
subdir('meson/deps/smaz')
subdir('meson/deps/bytes')
subdir('meson/deps/cstringfn')
subdir('meson/deps/fs')
subdir('meson/deps/djbhash')
subdir('submodules/c_ansi/ansi-codes')
subdir('submodules/c_ansi/ansi-rgb-utils')
subdir('submodules/c_ansi/ansi-utils')
subdir('meson/deps/debug-memory')
subdir('meson/deps/c_timer')
#//////////////////////////////////////////////////////////////////////
subdir('db')
subdir('rgb-ansi-utils')
subdir('hex-png-pixel-utils')
#//////////////////////////////////////////////////////////////////////
subdir('colors-csv-parser')
subdir('colors-json-parser')
#//////////////////////////////////////////////////////////////////////
subdir('hex-png-pixel-creator')
subdir('parse-colors')
#//////////////////////////////////////////////////////////////////////
subdir('color-db')
#//////////////////////////////////////////////////////////////////////
