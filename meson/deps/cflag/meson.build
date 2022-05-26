cflag_dir = '../../../submodules/cflag'

cflag_lib = library('cflag', 
  cflag_dir + '/cflag.c',
install: false, dependencies: [], 
  include_directories: [
    cflag_dir,
  ]
)
cflag_dep = declare_dependency(include_directories: [
    cflag_dir,
  ], link_with: cflag_dir,
  )
#deps += b64_dep
