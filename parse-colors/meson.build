test_parse_colors_qty = 500
parse_colors_srcs = [
  'parse-colors.c',
]
parse_colors_dirs = [
  '.',
  '../',
  inc,
]
parse_colors_cargs = [
  '-Wno-macro-redefined',
  ]
parse_colors_deps = [
     rgba_dep, 
     cargs_dep,
     colors_csv_parser_dep,
     colors_json_parser_dep,
     c_stringfn_dep,
     parson_dep,
     fs_dep,
     bytes_dep,
     c_string_buffer_dep,
     db_dep,
]
if get_option('enable-binaries')
  parse_colors_exec = executable('parse-colors',
     parse_colors_srcs,
     c_args: parse_colors_cargs,
     dependencies: parse_colors_deps,
     include_directories: parse_colors_dirs,
  )
  if get_option('enable-binary-tests')
    test('parse-colors-prep', sh_program, args: [
      '-c','rm -rf etc/colornames.test.json etc/colornames.test.sqlite',
      ],
      is_parallel:false,
      workdir: pwd,
    )
    test('parse-colors-csv', parse_colors_exec, args: [
      '-c', test_parse_colors_qty.to_string(),
      '-C','etc/colornames.bestof.csv',
      '-J','etc/colornames.test.json',
      '-m','csv',
      ],
      is_parallel:false,
      workdir: pwd,
    )
    test('parse-colors-json', parse_colors_exec, args: [
      '-c', test_parse_colors_qty.to_string(),
      '-J','etc/colornames.test.json',
      '-D','etc/colornames.test.sqlite',
      '-m','json',
      ],
      depends: [],
      is_parallel:false,
      workdir: pwd,
    )
    test('parse-colors-db', parse_colors_exec, args: [
      '-c', test_parse_colors_qty.to_string(),
      '-D','etc/colornames.test.sqlite',
      '-m','db',
      '-v',
      ],
      depends: [],
      is_parallel:false,
      workdir: pwd,
    )
  endif
endif
