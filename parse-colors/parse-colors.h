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


#include "../colors-json-parser/colors-json-parser.h"
#include "../submodules/c_ansi/ansi-codes/ansi-codes.h"
#include "../submodules/c_ansi/ansi-rgb-utils/ansi-rgb-utils.h"
#include "../subprojects/cargs/include/cargs.h"
//#include "../submodules/c_timer/include/c_timer.h"

#define RENDER_COLOR_OUTPUT     false
#define RENDER_PRETTY_JSON      alse
#define COLORNAMES_CSV_FILE     "vendor/colornames/colornames.csv"
#define COLORNAMES_JSON_FILE    "etc/parsed-colors.json"
////////////////////////////////////////////////////////////////////
#define DEFAULT_CSV_INPUT       COLORNAMES_CSV_FILE
#define DEFAULT_JSON_INPUT      COLORNAMES_JSON_FILE
#define DEFAULT_OUTPUT          ""
#define DEFAULT_COUNT           10
#define DEFAULT_VERBOSE         false
#define DEFAULT_PRETTY          false
#define DEFAULT_COLOR           false
#define DEFAULT_MODE            "csv"
////////////////////////////////////////////////////////////////////

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


#endif
