#pragma once
#ifndef DB_H
#define DB_H
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "../submodules/bytes/bytes.h"
#include "../submodules/boydemdb/boydemdb.h"
#include "../vendor/sqlite3.h"
#include "../submodules/c_ansi/ansi-codes/ansi-codes.h"
#include "../submodules/c_stringfn/include/stringfn.h"
#include "../submodules/c_timer/include/c_timer.h"
#include "../submodules/fs.c/fs.h"
#include "../submodules/parson/parson.h"

typedef struct ColorsDB ColorsDB;
struct ColorsDB {
    char *Path;
    boydemdb db;
};
boydemdb_id add_colors_db_if_not_exist(ColorsDB *DB, boydemdb_type TYPEID, char *RECORD);
boydemdb_id add_colors_db(ColorsDB *DB, boydemdb_type TYPEID, char *RECORD);
int init_colors_db(ColorsDB*);

#endif
