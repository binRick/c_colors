sqlite = [
  '../../../vendor/sqlite3.c',
]

sqlite_cflags = [
	'-DSQLITE_OMIT_LOAD_EXTENSION',
	'-DSQLITE_THREADSAFE=0',
	'-DSQLITE_DEFAULT_AUTOVACUUM=1',
	'-DSQLITE_TEMP_STORE=3',
]

boydemdb_includes = include_directories('../../../submodules/boydemdb')

libboydemdb_lib = static_library('boydemdb',
                             '../../../submodules/boydemdb/boydemdb.c', sqlite,
                             c_args: sqlite_cflags + ['-DBOYDEMDB_INTERNAL'],
                             include_directories: boydemdb_includes
)

boydemdb_dep = declare_dependency(
  link_with: libboydemdb_lib,
  include_directories: boydemdb_includes
)
