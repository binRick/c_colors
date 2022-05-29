#pragma once
#ifndef DB_H
#define DB_H
#include "../submodules/bytes/bytes.h"
#include "../submodules/c_ansi/ansi-codes/ansi-codes.h"
#include "../submodules/c_stringfn/include/stringfn.h"
#include "../submodules/c_timer/include/c_timer.h"
#include "../submodules/fs.c/fs.h"
#include "../submodules/parson/parson.h"
#include "../submodules/smaz/smaz.h"
#include "../vendor/sqlite3.h"
#include "db-mgr.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

typedef struct ColorsDB ColorsDB;
struct ColorsDB {
  char    *Path;
  colordb db;
};

int db_list_ids(ColorsDB *DB);

int db_list_type_ids(ColorsDB *DB, colordb_type TYPEID);
int db_count_ids(ColorsDB *DB);
int db_count_type_ids(ColorsDB *DB, colordb_type TYPEID);

colordb_id add_colors_db_if_not_exist(ColorsDB *DB, colordb_type TYPEID, char *RECORD);
colordb_id add_colors_db(ColorsDB *DB, colordb_type TYPEID, char *RECORD);
int init_colors_db(ColorsDB *);

#endif
