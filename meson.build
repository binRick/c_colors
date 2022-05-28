project('c_colors', ['c'],
  default_options: [
   'warning_level=0',
   'c_std=gnu99',
  ]
)
bins = [
  #  'm1',
]
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
#//***********************************************************/
deps = []
bin_srcs = []
submodule_srcs = []
bin_srcs = [
  #  'submodules/timestamp/timestamp.c',
  #a'submodules/uuid4/src/uuid4.c',
]
cc = meson.get_compiler('c')
#//***********************************************************/
sh = find_program('sh', required : true)
pwd_bin = find_program('pwd', required : true)
#//***********************************************************/
pwd = run_command(pwd_bin, check: true).stdout().strip()
#//***********************************************************/
inc = include_directories(
  #'.',
  #'./include', 
  #'./src', 
  #//  './submodules/timestamp', 
  #'./submodules/djbhash/src',
  #  './submodules',
)
#//***********************************************************/
subdir('meson/deps')
cargs_dep = dependency('cargs',fallback:['cargs', 'cargs_dep'])
spng_dep = dependency('libspng',fallback:['libspng', 'spng_dep'])
parson_shared_dep = dependency('parson',fallback:['parson', 'parson_shared_dep'])
subdir('colors_csv_parser')
subdir('parse-colors')
enabled_bins = [
  {
    'bin':'hex-png-pixel',
    'srcs': [
              'src/hex-png-pixel.c',
              'src/hex-png-pixel-utils.c',
              'vendor/stb_image_write.h',
              'include/hex-png-pixel.h',
              ],
    'tests': [
        { 
          'args': [
                    ['--test'],
                    ['pngs'],
                  ],
        },
      ],
    'deps':  [],
  },  

]

if get_option('enable-binaries')
  foreach b : enabled_bins
   bin_src = [
      'src/' + b['bin'] + '.c',
      b['srcs'],
   ]
   e = {
    'name': b['bin'],
    'exec': executable(b['bin'],
     [
      bin_src,
      files(bin_srcs),
      files(submodule_srcs),
     ],
     dependencies: b['deps'],
     include_directories: [
      inc,
     ],
    ),
   }
   if get_option('enable-binary-tests')
    foreach t: b['tests']
     foreach a: t['args']
      test(b['bin'], e['exec'], args: a)
     endforeach
    endforeach
   endif
  endforeach
endif
