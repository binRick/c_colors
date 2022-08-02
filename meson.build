project('c_colors', ['c'],default_options:['warning_level=0','c_std=gnu99'],
  subproject_dir: 'submodules/c_deps/subprojects',
)
cc = meson.get_compiler('c')
#//////////////////////////////////////////////////////////////////////
c_deps_dir = join_paths(meson.current_source_dir(), 'submodules/c_deps')
sh_program = find_program('sh',required:true)
pwd = meson.current_source_dir()
#//////////////////////////////////////////////////////////////////////
inc = [
  include_directories(
   '.',
  ),
  include_directories(
   'submodules/c_deps/submodules',
   'submodules/c_deps',
  ),
]
#############################################################################
subdir('submodules')
subdir('meson')
#//////////////////////////////////////////////////////////////////////
subdir('db')
subdir('hex-png-pixel-utils')
subdir('rgb-ansi-utils')
subdir('colors-csv-parser')
subdir('colors-json-parser')
subdir('dbmem')
subdir('hex-png-pixel-creator')
subdir('colors')
subdir('parse-colors')
#//////////////////////////////////////////////////////////////////////
subdir('colors-test')
subdir('dbmem-test')
#//////////////////////////////////////////////////////////////////////
