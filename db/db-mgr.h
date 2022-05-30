#pragma once
#ifndef _COLORDB_H_INCLDED
#define _COLORDB_H_INCLDED
////////////////////////////////////////////////
#include "../vendor/sqlite3.h"
#include <limits.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
////////////////////////////////////////////////
#define COLORDB_INIT    NULL
////////////////////////////////////////////////
typedef int64_t      colordb_id;
typedef colordb_id   colordb_type;
typedef struct {
  sqlite3      *db;
  sqlite3_stmt *select, *insert, *delete, *one;
  sqlite3_stmt *insert_typeid_hex;
  sqlite3_stmt *insert_typeid_name;
  sqlite3_stmt *ids, *typeids;
  sqlite3_stmt *typeid_ids;
  sqlite3_stmt *count_typeid;
  sqlite3_stmt *count_typeids, *count_ids;
  sqlite3_stmt *distinct_typeids;
} *colordb;
////////////////////////////////////////////////
colordb colordb_open(const char *path);
void colordb_close(colordb db);
colordb_id colordb_add(colordb db, const colordb_type type, void *blob, size_t size);
colordb_id colordb_add_typeid_hex(colordb db, const colordb_type type, char *hex, size_t size);
colordb_id colordb_add_typeid_name(colordb db, const colordb_type type, char *name, size_t size);
void *colordb_get(colordb db, colordb_id id, size_t *size);
void *colordb_one(colordb db, const colordb_type type, colordb_id *id, size_t *size);
void colordb_delete(colordb db, colordb_id id);
void *colordb_count_typeid(colordb db, const colordb_type type, size_t *size);
void *colordb_count_typeids(colordb db, size_t *size);
void *colordb_count_ids(colordb db, size_t *size);
void *colordb_get_typeid_ids(colordb db, const colordb_type type, size_t *size, size_t *rows_qty);
void *colordb_get_distinct_typeids(colordb db, size_t *size, size_t *rows_qty);

////////////////////////////////////////////////
#endif
