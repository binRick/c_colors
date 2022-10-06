rgb_ansi_utils_srcs = [
  'rgb-ansi-utils.c',
]
rgb_ansi_utils_cargs = [
  '-Wno-pointer-to-int-cast',
  ]
rgb_ansi_utils_dirs = [
  inc,
]
rgb_ansi_utils_deps = [
]
rgb_ansi_utils_lib = library('rgb-ansi-utils',
  rgb_ansi_utils_srcs,
  dependencies: rgb_ansi_utils_deps,
  c_args: rgb_ansi_utils_cargs,
  include_directories: rgb_ansi_utils_dirs,
)
rgb_ansi_utils_dep = declare_dependency(
  include_directories: rgb_ansi_utils_dirs,
  link_with: rgb_ansi_utils_lib,
)

