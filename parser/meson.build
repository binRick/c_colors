parser_srcs = [
  'main.c',
  'rgb2ansi.c',
]
parser_deps = [
     parson_shared_dep,
     cstringfn_dep,
     fs_dep,
     ansicodes_dep,
     rgba_dep, 
                                                          svpng_dep, spng_dep,
                                                          dep_cargs,
                                                          ]
_parser_deps = [
]
parser_env = {}
if get_option('enable-binaries')
  parser = executable('parser',
     [
      parser_srcs,
      files(submodule_srcs),
     ],
     dependencies: parser_deps,
     include_directories: [
       inc,
     ],
  )
  if get_option('enable-binary-tests')
    test('parser', parser, args: ['--test'])
  endif
endif
