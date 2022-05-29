#pragma once
#ifndef _COLORDB_H_INCLDED
#define _COLORDB_H_INCLDED
#include "../vendor/sqlite3.h"
#include <limits.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#define COLORDB_INIT    NULL

typedef int64_t colordb_id;

typedef struct {
  sqlite3      *db;
  sqlite3_stmt *select, *insert, *delete, *one;
  sqlite3_stmt *ids, *typeids, *typeid_ids, *count_typeids, *count_ids, *count_typeid;
  sqlite3_stmt *distinct_typeids;
} *colordb;

typedef colordb_id colordb_type;

colordb_id colordb_add(colordb db, const colordb_type type, void *blob, size_t size);
void *colordb_get(colordb db, colordb_id id, size_t *size);
void *colordb_one(colordb db, const colordb_type type, colordb_id *id, size_t *size);
void colordb_delete(colordb db, colordb_id id);
colordb colordb_open(const char *path);
void colordb_close(colordb db);

void *colordb_count_typeid(colordb db, const colordb_type type, size_t *size);
void *colordb_count_typeids(colordb db, size_t *size);
void *colordb_count_ids(colordb db, size_t *size);
void *colordb_get_typeid_ids(colordb db, const colordb_type type, size_t *size, size_t *rows_qty);
void *colordb_get_distinct_typeids(colordb db, size_t *size, size_t *rows_qty);

#endif
