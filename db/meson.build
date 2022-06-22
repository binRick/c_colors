db_srcs = [
  'db.c',
  'db-mgr.c',
  '../vendor/sqlite/sqlite3.c',
]
db_dirs = [
  '../',
  '../vendor',
  '../vendor/sqlite',
  inc,
  ]
db_deps = [
     cargs_dep,
     c_stringfn_dep,
     parson_dep,
     fs_dep,
     ansi_codes_dep, 
     bytes_dep,
     djbhash_dep,
     c_string_buffer_dep,
     smaz_dep,
     c_timer_dep,
]
sqlite_cflags = [
    '-DSQLITE_OMIT_LOAD_EXTENSION',
    '-DSQLITE_THREADSAFE=0',
    '-DSQLITE_DEFAULT_AUTOVACUUM=1',
    '-DSQLITE_TEMP_STORE=3',
]

db_lib = static_library('db',
  db_srcs,
  install: false,
  dependencies: db_deps,
  include_directories: db_dirs,
  c_args: sqlite_cflags,
)

db_dep = declare_dependency(
  include_directories: db_dirs,
  link_with: db_lib,
)
