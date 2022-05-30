dbmem_test_srcs = [
  'dbmem-test.c',
]
dbmem_test_deps = [
     dbmem_dep,
]
if get_option('enable-binaries')
  dbmem_test_exec = executable('dbmem-test',
     dbmem_test_srcs,
     dependencies: dbmem_test_deps,
     include_directories: [],
  )
  if get_option('enable-binary-tests')
    test('dbmem-test Test', dbmem_test_exec, args: ['--test'])
  endif
endif
