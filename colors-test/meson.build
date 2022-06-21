colors_test_srcs = [
  'colors-test.c',
]
colors_test_inc = [
 inc,
 '.',
]
colors_test_deps = [
 reproc_dep,
 list_dep,
 ansicodes_dep,
 c_fsio_dep,
 c_stringfn_dep,
 c_string_buffer_dep,
 greatest_dep,
 ansicodes_dep,
 logh_dep,
 parson_dep,
 ee_dep,
 debug_memory_dep,
 tinydir_dep,
 db_dep,
 dbg_dep,
 colors_dep,
]

if get_option('enable-binaries')
  colors_test_exec = executable('colors-test', 
     colors_test_srcs,
     dependencies: colors_test_deps,
     include_directories: colors_test_inc,
  )
  if get_option('enable-binary-tests')
    test('colors-test', colors_test_exec, args: [])
  endif
endif
