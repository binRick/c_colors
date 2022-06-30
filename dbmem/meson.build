dbmem_lib = static_library('dbmem', 
  'dbmem.c',
install: false, 
dependencies: [
  sqlite3_dep,
  ], 
  include_directories: [
  ]
)
dbmem_dep = declare_dependency(include_directories: [], link_with: dbmem_lib)
