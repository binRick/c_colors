colors_json_srcs = [
  'colors-json-parser.c',
]
colors_json_parser_dirs = [
  inc,
  '../submodules/c_deps/submodules/progress.c',
  '../submodules/c_deps/submodules/debug-memory',
  '../submodules/c_deps/submodules/bytes',
  '../submodules/c_deps/submodules/djbhash',
  '../submodules/c_deps/submodules/djbhash/src',
]
colors_json_parser_cargs = [
  '-Wno-macro-redefined',
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
  c_args: colors_json_parser_cargs,
  include_directories: colors_json_parser_dirs,
  )
colors_json_parser_dep = declare_dependency(
  include_directories: colors_json_parser_dirs,
  link_with: colors_json_parser_lib,
)

