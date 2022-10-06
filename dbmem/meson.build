dbmem_c_args = [
  '-Wno-missing-field-initializers',
  ]
dbmem_lib = library('dbmem', 
  'dbmem.c',
install: false, 
c_args: dbmem_c_args,
dependencies: [
  sqlite3_dep,
  ], 
  include_directories: [
  ]
)
dbmem_dep = declare_dependency(include_directories: [], link_with: dbmem_lib)
