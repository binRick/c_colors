#pragma once
#ifndef PARSER_H
#define PARSER_H
#include <ctype.h>
#include <math.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>


#include "../colors-csv-parser/colors-csv-parser.h"
#include "../colors-json-parser/colors-json-parser.h"
#include "../db/db.h"
#include "../db/db.h"
#include "../submodules/c_ansi/ansi-codes/ansi-codes.h"
#include "../submodules/c_ansi/ansi-rgb-utils/ansi-rgb-utils.h"
#include "../subprojects/cargs/include/cargs.h"
//#include "../submodules/c_timer/include/c_timer.h"
//#ifndef DBG_H_c49aa87c_54eb_46d2_8d72_a51f5efce1ac
//#include "../submodules/dbg.h/dbg.h"
//#endif

#define RENDER_COLOR_OUTPUT  false
#define RENDER_PRETTY_JSON   alse
#define COLORNAMES_CSV_FILE  "vendor/colornames/colornames.csv"
#define COLORNAMES_JSON_FILE "etc/parsed-colors.json"
#define COLOR_NAMES_DB_PATH  "etc/colornames.sqlite"
////////////////////////////////////////////////////////////////////
#define DEFAULT_CSV_INPUT    COLORNAMES_CSV_FILE
#define DEFAULT_JSON_INPUT   COLORNAMES_JSON_FILE
#define DEFAULT_OUTPUT       ""
#define DEFAULT_COUNT        10
#define DEFAULT_VERBOSE      false
#define DEFAULT_PRETTY       false
#define DEFAULT_COLOR        false
#define DEFAULT_MODE         "csv"
////////////////////////////////////////////////////////////////////
#define VERBOSE_DEBUG_HANDLER false

int parse_args(int argc, char *argv[]);
int debug_args();
int hex_to_closest_ansi_code(const uint32_t trp);
int parse_csv();

typedef struct CLI_ARGS {
  char *input;
  char *output;
  char *mode;
  bool verbose;
  int  count;
  bool pretty;
  int  color;
} args_t;

static struct cag_option options[] = {
  { .identifier     = 'i',
    .access_letters = "i",
    .access_name    = "input",
    .value_name     = "INPUT",
    .description    = "Input File Path" },
  { .identifier     = 'o',
    .access_letters = "o",
    .access_name    = "output",
    .value_name     = "OUTPUT",
    .description    = "Output File Path" },
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
  { .identifier     = 'p',
    .access_letters = "p",
    .access_name    = "pretty",
    .value_name     = NULL,
    .description    = "JSON Pretty Mode" },
  { .identifier     = 'x',
    .access_letters = "C",
    .access_name    = "color",
    .value_name     = NULL,
    .description    = "Color Mode" },
  { .identifier     = 'h',
    .access_letters = "h",
    .access_name    = "help",
    .description    = "Shows the command help" }
};


const char CURPALETTE[] = "\
\x1b[48;5;0m  \x1b[0m\
\x1b[48;5;1m  \x1b[0m\
\x1b[48;5;2m  \x1b[0m\
\x1b[48;5;3m  \x1b[0m\
\x1b[48;5;4m  \x1b[0m\
\x1b[48;5;5m  \x1b[0m\
\x1b[48;5;6m  \x1b[0m\
\x1b[48;5;7m  \x1b[0m\n\
\x1b[48;5;8m  \x1b[0m\
\x1b[48;5;9m  \x1b[0m\
\x1b[48;5;10m  \x1b[0m\
\x1b[48;5;11m  \x1b[0m\
\x1b[48;5;12m  \x1b[0m\
\x1b[48;5;13m  \x1b[0m\
\x1b[48;5;14m  \x1b[0m\
\x1b[48;5;15m  \x1b[0m\n";

const char NEWPALETTE[] = "\
\x1b]4;0;#%s\x1b\\ \
\x1b]4;1;#%s\x1b\\ \
\x1b]4;2;#%s\x1b\\ \
\x1b]4;3;#%s\x1b\\ \
\x1b]4;4;#%s\x1b\\ \
\x1b]4;5;#%s\x1b\\ \
\x1b]4;6;#%s\x1b\\ \
\x1b]4;7;#%s\x1b\\ \
\x1b]4;8;#%s\x1b\\ \
\x1b]4;9;#%s\x1b\\ \
\x1b]4;10;#%s\x1b\\ \
\x1b]4;11;#%s\x1b\\ \
\x1b]4;12;#%s\x1b\\ \
\x1b]4;13;#%s\x1b\\ \
\x1b]4;14;#%s\x1b\\ \
\x1b]4;15;#%s\x1b\\ \
\x1b]10;#%s\x1b\\ \
\x1b]11;#%s\x1b\\ \
\x1b]12;#%s\x1b\\ \
\x1b[21D\n";

typedef struct WorkerPaletteType_t WorkerPaletteType_t;
struct WorkerPaletteType_t {
  char *Name;
  char *Colors;
};


#ifndef AC_SHOW_CURSOR
#define AC_SHOW_CURSOR                 "\x1b[?25h"
#endif
#define DEBUG_COLORS "EBEEF9 0D101B EBEEF9 000009 BB4F6C 72CCAE C65E3D 58B6CA 6488C4 8D84C6 858893 1F222D EE829F A5FFE1 F99170 8BE9FD 97BBF7 C0B7F9 FFFFFF"
#define CSV_COLORS "D9D8D8 231F20 D9D8D8 231F20 EE2E24 00853E FFD204 009DDC 98005D 85CEBC D9D8D8 737171 EE2E24 00853E FFD204 009DDC 98005D 85CEBC FFFFFF"
#define DB_COLORS "F2F2F2 041A3B F2F2F2 303030 E1321A 6AB017 FFC005 729FCF EC0048 2AA7E7 F2F2F2 5D5D5D FF361E 7BC91F FFD00A 0071FF FF1D62 4BB8FD A020F0"
#define JSON_COLORS "D0D0D0 1C1C1C D0D0D0 2F2E2D A36666 90A57D D7AF87 7FA5BD C79EC4 8ADBB4 D0D0D0 4A4845 D78787 AFBEA2 E4C9AF A1BDCE D7BEDA B1E7DD EFEFEF"
#define DEFAULT_COLORS "D0D0D0 000000 D0D0D0 000000 FF0000 33FF00 FF0099 0066FF CC00FF 00FFFF D0D0D0 808080 FF0000 33FF00 FF0099 0066FF CC00FF 00FFFF FFFFFF"

static WorkerPaletteType_t WorkerPaletteTypes[] = {
  { .Name = "DEFAULT_COLORS", .Colors = DEFAULT_COLORS },
  { .Name = "CSV",            .Colors = CSV_COLORS     },
  { .Name = "JSON",           .Colors = JSON_COLORS    },
  { .Name = "DB",             .Colors = DB_COLORS      },
  { .Name = "DEBUG",          .Colors = DEBUG_COLORS   },
};


#endif
