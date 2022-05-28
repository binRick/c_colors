parse_colors_srcs = [
  'parse-colors.c',
]
parse_colors_deps = [
     cargs_dep,
     colors_csv_parser_dep,
     cstringfn_dep,
     parson_shared_dep,
     fs_dep,
     #     ansicodes_dep,
     rgba_dep, 
     ansi_codes_dep, 
     ansi_rgb_utils_dep,
     bytes_dep,
     c_string_buffer_dep,
     #c_ansi_dep, 
]
if get_option('enable-binaries')
  parse_colors_exec = executable('parse-colors',
     parse_colors_srcs,
     dependencies: parse_colors_deps,
     include_directories: [],
  )
  if get_option('enable-binary-tests')
    test('parse-colors Test', parse_colors_exec, args: ['--test'])
  endif
endif
