parse_colors_srcs = [
  'color-db.c',
]
parse_colors_deps = [
     cargs_dep,
     cstringfn_dep,
     parson_shared_dep,
     fs_dep,
     ansi_codes_dep, 
     bytes_dep,
     c_string_buffer_dep,
     c_timer_dep,
     db_dep,
     asprintf_dep,
]
if get_option('enable-binaries')
  parse_colors_exec = executable('color-db',
     parse_colors_srcs,
     dependencies: parse_colors_deps,
     include_directories: [],
  )
  if get_option('enable-binary-tests')
    test('color-db Test', parse_colors_exec, args: ['--test'])
  endif
endif
