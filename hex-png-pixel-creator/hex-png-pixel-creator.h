#pragma once
#include <ctype.h>
#include <math.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <stdio.h>
#include <string.h>
#include "c_ansi/ansi-codes/ansi-codes.h"
#include "cargs/include/cargs.h"
#include "hex-png-pixel-utils/hex-png-pixel-utils.h"
#include "c_fsio/include/fsio.h"

int parse_args(int, char **);
int debug_args();
int write_png_images();
int hex_png_pixel(char *COLOR);
char *str_substring(char *str, int start, int size);

#define PARSED_COLORS_JSON_FILE    "etc/colornames.json"
////////////////////////////////////////////////////////////////////
#define DEFAULT_INPUT           PARSED_COLORS_JSON_FILE
#define DEFAULT_OUTPUT_DIR      "etc/images"
#define DEFAULT_COUNT           10
#define DEFAULT_VERBOSE         false
#define DEFAULT_MODE            "write"
#define DEFAULT_HEX             "FFFFFF"
////////////////////////////////////////////////////////////////////

typedef struct CLI_ARGS {
  char *input;
  char *output_dir;
  char *mode;
  bool verbose;
  int  count;
  char *hex;
} args_t;

static struct cag_option options[] = {
  { .identifier     = 'i',
    .access_letters = "i",
    .access_name    = "input",
    .value_name     = "INPUT",
    .description    = "Input File Path" },
  { .identifier     = 'o',
    .access_letters = "o",
    .access_name    = "output_dir",
    .value_name     = "OUTPUT_DIR",
    .description    = "Output Directory" },
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
  { .identifier     = 'H',
    .access_letters = "H",
    .access_name    = "hex",
    .value_name     = "HEX",
    .description    = "Color Hex Value" },
  { .identifier     = 'h',
    .access_letters = "h",
    .access_name    = "help",
    .description    = "Shows the command help" }
};


