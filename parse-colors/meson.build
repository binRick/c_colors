parse_colors_srcs = [
  'parse-colors.c',
]
parse_colors_dirs = [
  inc,
]
parse_colors_deps = [
     cargs_dep,
     colors_csv_parser_dep,
     colors_json_parser_dep,
     c_stringfn_dep,
     parson_dep,
     fs_dep,
     #rgba_dep, 
     #   rgb_ansi_utils_dep, 
     #ansi_codes_dep, 
     #ansi_rgb_utils_dep,
     bytes_dep,
     c_string_buffer_dep,
     db_dep,
     #ansi_utils_deps,
]
if get_option('enable-binaries')
  parse_colors_exec = executable('parse-colors',
     parse_colors_srcs,
     dependencies: parse_colors_deps,
     include_directories: parse_colors_dirs,
  )
  if get_option('enable-binary-tests')
    test('parse-colors Test', parse_colors_exec, args: ['--test'])
  endif
endif
