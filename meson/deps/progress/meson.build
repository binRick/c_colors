progress_cargs = [
  '-Wno-macro-redefined',
  ]
progress_lib = library('process', 
'../../../submodules/progress.c/progress.c',
install: false, dependencies: [
  ], 
  include_directories: [
  ],
  c_args: progress_cargs,
)
progress_dep = declare_dependency(include_directories: [
  ], link_with: progress_lib,
  )
