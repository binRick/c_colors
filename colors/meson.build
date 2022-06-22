colors_timeout_seconds = '.8'
colors_srcs = [
  'colors.c',
]
colors_inc = [
 inc,
]
colors_deps = [
   djbhash_dep,
   cargs_dep,
   colors_csv_parser_dep,
   colors_json_parser_dep,
   c_stringfn_dep,
   parson_dep,
   fs_dep,
   rgba_dep,
   ansi_codes_dep,
   #ansi_rgb_utils_dep,
   bytes_dep,
   c_string_buffer_dep,
   db_dep,
   ansi_utils_deps, 
]


colors_lib = static_library('colors',
  colors_srcs,
  install: false,
  dependencies: colors_deps,
  c_args: [],
  include_directories: colors_inc,
)

colors_dep = declare_dependency(
  include_directories: colors_inc,
  link_with: colors_lib,
)
