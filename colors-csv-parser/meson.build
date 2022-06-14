colors_csv_srcs = [
  'colors-csv-parser.c',
]
colors_csv_parser_deps = [
  rgb_ansi_utils_dep,
  hex_png_pixel_utils_dep,
  progress_dep,
  debug_memory_dep,
  djbhash_dep,
]
colors_csv_parser_lib = static_library('colors-csv-parser',
  colors_csv_srcs,
  install: false, 
  dependencies: colors_csv_parser_deps,
  include_directories: [
    inc,
    ],
  )
colors_csv_parser_dep = declare_dependency(
  include_directories: [
    inc,
    ], 
  link_with: colors_csv_parser_lib,
)

