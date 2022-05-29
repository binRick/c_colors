#####################################################################################
asprintf_name = 'asprintf'
asprintf_dir = '../../../submodules/asprintf.c'
asprintf_srcs = [
   asprintf_dir + '/asprintf.c',
]
asprintf_cflags = []
#####################################################################################


#####################################################################################
asprintf_lib = static_library(asprintf_name,asprintf_srcs, c_args: asprintf_cflags)
asprintf_dep = declare_dependency(link_with: asprintf_lib)
#####################################################################################
