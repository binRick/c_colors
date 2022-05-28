colors_csv_srcs = [
  'colors_csv_parser.c',
  'rgb2ansi.c',
]
colors_csv_parser_deps = [
]
colors_csv_parser_lib = static_library('colors_csv_parser',
  colors_csv_srcs,
  install: false, 
  dependencies: colors_csv_parser_deps,
  include_directories: [],
  )
colors_csv_parser_dep = declare_dependency(
  include_directories: [], 
  link_with: colors_csv_parser_lib,
)

