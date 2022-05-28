project('c_colors', ['c'],
  default_options: [
   'warning_level=0',
   'c_std=gnu99',
  ]
)
config = configuration_data()
version = run_command('git', 'describe', '--tags')
config.set('version', version.stdout().strip())
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
subdir('meson/deps/rgba')
subdir('meson/deps/cstringfn')
subdir('meson/deps/fs')
subdir('meson/deps/ansicodes')
cargs_dep = dependency('cargs',fallback:['cargs', 'cargs_dep'])
parson_shared_dep = dependency('parson',fallback:['parson', 'parson_shared_dep'])
#//////////////////////////////////////////////////////////////////////
subdir('rgb-ansi-utils')
subdir('hex-png-pixel-utils')
subdir('hex-png-pixel-creator')
subdir('colors-csv-parser')
subdir('parse-colors')
#//////////////////////////////////////////////////////////////////////
