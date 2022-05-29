tempdir_lib = library('tempdir', 
'../../../submodules/tempdir.c/tempdir.c',
install: false, dependencies: [
  strdup_dep,
  ], 
  include_directories: [
    '../../../submodules',
  ]
)
tempdir_dep = declare_dependency(include_directories: [
  ], link_with: tempdir_lib)
