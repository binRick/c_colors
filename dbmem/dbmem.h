#ifndef __dbmem_h__
#define __dbmem_h__
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

#include "../vendor/sqlite3-3.6.20.h"

#define DBMEM_NAME    "dbmem"

typedef struct spmembuffer_t {
  char *data;
  int  used;
  int  total;
} spmembuffer_t;

typedef struct dbmem_db_t {
  sqlite3       *handle;
  spmembuffer_t *mem;
} dbmem_db_t;

int dbmem_env_init();

void dbmem_env_fini();

int dbmem_open_db(dbmem_db_t *db, const char *path, spmembuffer_t *mem);

int dbmem_close_db(dbmem_db_t *db);

#ifdef __cplusplus
}
#endif

#endif

