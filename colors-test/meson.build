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
 c_fsio_dep,
 c_stringfn_dep,
 c_string_buffer_dep,
 greatest_dep,
 logh_dep,
 parson_dep,
 ee_dep,
 debug_memory_dep,
 tinydir_dep,
 db_dep,
 dbg_dep,
 colors_dep,
 ansi_codes_dep,
 ansi_utils_dep,
]

if get_option('enable-binaries')
  colors_test_exec = executable('colors-test', 
     colors_test_srcs,
     dependencies: colors_test_deps,
     include_directories: colors_test_inc,
  )
  if get_option('enable-binary-tests')
    test('colors-test', passh_program, args:[colors_test_exec])
  endif
endif
