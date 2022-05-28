colors_csv_srcs = [
  'colors-csv-parser.c',
]
colors_csv_parser_deps = [
  rgb_ansi_utils_dep,
]
colors_csv_parser_lib = static_library('colors-csv-parser',
  colors_csv_srcs,
  install: false, 
  dependencies: colors_csv_parser_deps,
  include_directories: [],
  )
colors_csv_parser_dep = declare_dependency(
  include_directories: [], 
  link_with: colors_csv_parser_lib,
)

