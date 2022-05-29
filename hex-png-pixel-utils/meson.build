hex_png_pixel_utils_srcs = [
  'hex-png-pixel-utils.c',
  'stb_image_write.h',
]
hex_png_pixel_utils_deps = [
     cargs_dep,
     #//     dbg_dep,
     asprintf_dep,
     bytes_dep,
     c_fsio_dep,
     cstringfn_dep,
     tempdir_dep,
     b64_dep,
     debug_memory_dep,
]

hex_png_pixel_utils_lib = static_library('hex-png-pixel-utils',
  hex_png_pixel_utils_srcs,
  install: false,
  dependencies: hex_png_pixel_utils_deps,
  include_directories: [],
)

hex_png_pixel_utils_dep = declare_dependency(
  include_directories: [],
  link_with: hex_png_pixel_utils_lib,
)
