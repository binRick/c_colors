colors_csv_srcs = [
  'colors-csv-parser.c',
]
colors_csv_parser_dirs = [
  inc,
]
colors_csv_parser_cargs = [
  '-Wno-macro-redefined',
  ]
colors_csv_parser_deps = [
  ansi_codes_dep,
  rgb_ansi_utils_dep,
  hex_png_pixel_utils_dep,
  progress_dep,
  parson_dep,
  c_fsio_dep,
  fs_dep,
  rgba_dep,
  debug_memory_dep,
  djbhash_dep,
]
colors_csv_parser_lib = static_library('colors-csv-parser',
  colors_csv_srcs,
  install: true, 
  dependencies: colors_csv_parser_deps,
  c_args: colors_csv_parser_cargs,
  include_directories: colors_csv_parser_dirs,
)
colors_csv_parser_dep = declare_dependency(
  include_directories: colors_csv_parser_dirs,
  link_with: colors_csv_parser_lib,
)

