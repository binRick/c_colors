hex_png_pixel_creator_srcs = [
  'hex-png-pixel-creator.c',
]
hex_png_pixel_creator_deps = [
     cargs_dep,
     hex_png_pixel_utils_dep,
     parson_shared_dep,
]
if get_option('enable-binaries')
  hex_png_pixel_creator_exec = executable('hex-png-pixel-creator',
     [
      hex_png_pixel_creator_srcs,
     ],
     dependencies: hex_png_pixel_creator_deps,
     include_directories: [],
  )
  if get_option('enable-binary-tests')
    test('hex-png-pixel-creator Test', hex_png_pixel_creator_exec, args: ['--test'])
  endif
endif
