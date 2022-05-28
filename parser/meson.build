parser_srcs = [
  'main.c',
]
parser_deps = [
     cargs_dep,
     colors_csv_parser_dep,
     cstringfn_dep,
     parson_shared_dep,
     fs_dep,
     ansicodes_dep,
     rgba_dep, 
]
parser_env = {}
if get_option('enable-binaries')
  parser = executable('parser',
     [
      parser_srcs,
     ],
     dependencies: parser_deps,
     include_directories: [
     ],
  )
  if get_option('enable-binary-tests')
    test('parser', parser, args: ['--test'])
  endif
endif
