chan_lib = library('chan', 
    '../../../submodules/chan/src/chan.c',
    '../../../submodules/chan/src/queue.c',
    install: false, dependencies: [], 
    include_directories: [
        '../../../submodules/chan/src',
    ]
)

chan_dep = declare_dependency(include_directories: [
        '../../../submodules/chan/src',
    ], link_with: chan_lib
)

#deps += djbhash_dep
