db_srcs = [
  'db.c',
]
db_deps = [
     cargs_dep,
     cstringfn_dep,
     parson_shared_dep,
     fs_dep,
     ansi_codes_dep, 
     bytes_dep,
     c_string_buffer_dep,
     c_timer_dep,
     boydemdb_dep,
]

db_lib = static_library('db',
  db_srcs,
  install: false,
  dependencies: db_deps,
  include_directories: [],
  )
db_dep = declare_dependency(
  include_directories: [],
  link_with: db_lib,
)
