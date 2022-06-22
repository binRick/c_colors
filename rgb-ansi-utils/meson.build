rgb_ansi_utils_srcs = [
  'rgb-ansi-utils.c',
]
rgb_ansi_utils_dirs = [
  inc,
]
rgb_ansi_utils_deps = [
  m_dep,
]
rgb_ansi_utils_lib = static_library('rgb-ansi-utils',
  rgb_ansi_utils_srcs,
  install: false, 
  dependencies: rgb_ansi_utils_deps,
  include_directories: rgb_ansi_utils_dirs,
)
rgb_ansi_utils_dep = declare_dependency(
  include_directories: rgb_ansi_utils_dirs,
  link_with: rgb_ansi_utils_lib,
)

