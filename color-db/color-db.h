#pragma once
#ifndef COLORDB_H
#define COLORDB_H
#include <ctype.h>
#include <math.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>


#include "../colors-json-parser/colors-json-parser.h"
#include "../submodules/c_ansi/ansi-codes/ansi-codes.h"
#include "../submodules/c_ansi/ansi-rgb-utils/ansi-rgb-utils.h"
#include "../subprojects/cargs/include/cargs.h"
#include "../submodules/boydemdb/boydemdb.h"
#include "../vendor/sqlite3.h"

////////////////////////////////////////////////////////////////////
#define DEFAULT_DB_FILE         "etc/db1.sqlite"
#define DEFAULT_OUTPUT          ""
#define DEFAULT_COUNT           10
#define DEFAULT_VERBOSE         false
#define DEFAULT_PRETTY          false
#define DEFAULT_COLOR           false
#define DEFAULT_MODE            "list"
////////////////////////////////////////////////////////////////////

int init_db();
int read_db();

typedef struct CLI_ARGS {
  char *db_file;
  char *mode;
  bool verbose;
  int  count;
} args_t;

static struct cag_option options[] = {
  { .identifier     = 'd',
    .access_letters = "d",
    .access_name    = "db",
    .value_name     = "DATABASE",
    .description    = "DATABASE_FILE" },
  { .identifier     = 'm',
    .access_letters = "m",
    .access_name    = "mode",
    .value_name     = "MODE",
    .description    = "Mode" },
  { .identifier     = 'v',
    .access_letters = "v",
    .access_name    = "verbose",
    .value_name     = NULL,
    .description    = "Verbose Mode" },
  { .identifier     = 'c',
    .access_letters = "c",
    .access_name    = "count",
    .value_name     = "COUNT",
    .description    = "Item Count" },
  { .identifier     = 'h',
    .access_letters = "h",
    .access_name    = "help",
    .description    = "Shows the command help" }
};


#endif
