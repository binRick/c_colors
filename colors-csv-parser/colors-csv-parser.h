#pragma once
#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct parse_csv_options parse_csv_options;
struct parse_csv_options {
  char   *input_file;
  char   *output_file;
  bool   verbose_mode, pretty_json_mode, output_color_mode;
  size_t parse_qty;
};
int parse_colors_csv(parse_csv_options *OPTIONS);

#include "bytes/bytes.h"
#include "c_ansi/ansi-codes/ansi-codes.h"
#include "c_ansi/ansi-rgb-utils/ansi-rgb-utils.h"
#include "c_string_buffer/include/stringbuffer.h"
#include "c_stringfn/include/stringfn.h"
#include "colors-csv-parser/colors-csv-parser.h"
#include "fs.c/fs.h"
#include "hex-png-pixel-utils/hex-png-pixel-utils.h"
#include "parson/parson.h"
#include "progress.c/progress.h"
#include "rgb-ansi-utils/rgb-ansi-utils.h"
#include "rgba/src/rgba.h"

