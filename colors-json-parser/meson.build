colors_json_srcs = [
  'colors-json-parser.c',
]
colors_json_parser_deps = [
  rgb_ansi_utils_dep,
  parson_dep,
  fs_dep,
  debug_memory_dep,
  c_timer_dep,
  djbhash_dep,
  hex_png_pixel_utils_dep,
  db_dep,
  progress_dep,
]
colors_json_parser_lib = static_library('colors-json-parser',
  colors_json_srcs,
  install: false, 
  dependencies: colors_json_parser_deps,
  include_directories: [
    inc,

    ],
  )
colors_json_parser_dep = declare_dependency(
  include_directories: [
    inc,

    ],
  link_with: colors_json_parser_lib,
)

