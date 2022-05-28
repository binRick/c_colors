#pragma once
#ifndef CSV_PARSER_H
#define CSV_PARSER_H
#include <assert.h>
#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

#include "../submodules/c_stringfn/include/stringfn.h"
#include "../submodules/djbhash/src/djbhash.h"
#include "../submodules/fs.c/fs.h"
#include "../submodules/rgba/src/rgba.h"
#include "../submodules/slug.c/src/slug.h"
#include "../submodules/timestamp/timestamp.h"
#include "../submodules/uuid4/src/uuid4.h"
#include "../subprojects/parson/parson.h"
#include "../vendor/ansicodes/ansicodes.h"
#include "../vendor/ansicodes/rgbansi.h"

#define RENDER_COLOR_OUTPUT     false
#define RENDER_PRETTY_JSON      true
#define COLORNAMES_CSV_FILE     "vendor/colornames/colornames.csv"
#define COLORNAMES_JSON_FILE    "vendor/colornames/colornames.json"
#define DEFAULT_INPUT           COLORNAMES_CSV_FILE
#define DEFAULT_OUTPUT          NULL
#define DEFAULT_COUNT           10
#define DEFAULT_VERBOSE         false
#define DEFAULT_PRETTY          false
#define DEFAULT_COLOR           false
#define DEFAULT_MODE            "csv"
#define C256_FG_CODE            "\x1b[38;5;%dm"
#define C256_BG_CODE            "\x1b[48;5;%dm"
#define TRUECOLOR_FG_CODE       "\x1b[38;2;%d;%d;%dm"
#define TRUECOLOR_BG_CODE       "\x1b[48;2;%d;%d;%dm"
#define RESET_CODE              "\x1b[0m"


typedef struct parse_csv_options parse_csv_options;
struct parse_csv_options {
  char   *input_file;
  char   *output_file;
  bool   verbose_mode, pretty_json_mode, output_color_mode;
  size_t parse_qty;
};

int parse_colors_csv(parse_csv_options *OPTIONS);

#endif
