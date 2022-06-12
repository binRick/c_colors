#pragma once
#ifndef DB_H
#define DB_H
#define COLORDB_MAX_HASH_BUCKETS    65536 * 1024
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
//#include "../submodules/djbhash/src/djbhash.h"
#include "../submodules/c_ansi/ansi-codes/ansi-codes.h"
/////////////////////////////////////////////////////////
/*#include "../submodules/bytes/bytes.h"
 #include "../submodules/c_stringfn/include/stringfn.h"
 #include "../submodules/c_timer/include/c_timer.h"
 #include "../submodules/fs.c/fs.h"
 #include "../submodules/parson/parson.h"*/
/////////////////////////////////////////////////////////
#include "../vendor/sqlite3.h"
#include "db-mgr.h"
/////////////////////////////////////////////////////////
typedef struct ColorsDB ColorsDB;
struct ColorsDB {
  char    *Path;
  colordb db;
};
/////////////////////////////////////////////////////////
int init_colors_db(ColorsDB *);
int db_list_ids(ColorsDB *DB);
int db_list_typeids(ColorsDB *DB);
int db_list_type_ids(ColorsDB *DB, colordb_type TYPEID);
int db_count_ids(ColorsDB *DB);
int db_count_type_ids(ColorsDB *DB, colordb_type TYPEID);
colordb_id add_colors_db_if_not_exist(ColorsDB *DB, colordb_type TYPEID, char *RECORD, char *HEX, char *NAME);
colordb_id add_colors_db_typeid_hex(ColorsDB *DB, colordb_type TYPEID, char *HEX);
colordb_id add_colors_db_typeid_name(ColorsDB *DB, colordb_type TYPEID, char *NAME);
colordb_id add_colors_db(ColorsDB *DB, colordb_type TYPEID, char *RECORD);

/////////////////////////////////////////////////////////
struct djbhash db_get_typeids_hash(ColorsDB *DB);
unsigned long colordb_hash(char *key, int length);

#endif
