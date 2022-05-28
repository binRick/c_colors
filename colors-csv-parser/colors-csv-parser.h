#pragma once
#ifndef CSV_PARSER_H
#define CSV_PARSER_H
#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../submodules/c_stringfn/include/stringfn.h"
#include "../submodules/fs.c/fs.h"
#include "../submodules/parson/parson.h"
#include "../submodules/rgba/src/rgba.h"
#include "../vendor/ansicodes/ansicodes.h"
#include "../vendor/ansicodes/rgbansi.h"

typedef struct parse_csv_options parse_csv_options;
struct parse_csv_options {
  char   *input_file;
  char   *output_file;
  bool   verbose_mode, pretty_json_mode, output_color_mode;
  size_t parse_qty;
};

int parse_colors_csv(parse_csv_options *OPTIONS);

#endif
