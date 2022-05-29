#pragma once
#ifndef JSON_PARSER_H
#define JSON_PARSER_H
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#ifndef AC_HIDE_CURSOR
#define AC_HIDE_CURSOR                 "\x1b[?25l"
#endif
#ifndef AC_SHOW_CURSOR
#define AC_SHOW_CURSOR                 "\x1b[?25h"
#endif

typedef struct parse_json_options   parse_json_options;
typedef struct ParsedColor          ParsedColor;

#include "../db/db.h"
#include "../submodules/djbhash/src/djbhash.h"

typedef void (*json_item_cb_t)(ParsedColor *PARSED_COLOR_ITEM);
struct parse_json_options {
  char           *input_file, *duration;
  bool           verbose_mode;
  size_t         parse_qty;
  ParsedColor    **ParsedColors;
  size_t         ParsedColorsQty;
  struct djbhash ResultsHash;
  ColorsDB       *DB;
  json_item_cb_t ParsedColorHandler;
};

#include "../db/db.h"
#include "../hex-png-pixel-utils/hex-png-pixel-utils.h"
#include "../submodules/bytes/bytes.h"
#include "../submodules/c_ansi/ansi-codes/ansi-codes.h"
#include "../submodules/c_stringfn/include/stringfn.h"
#include "../submodules/c_timer/include/c_timer.h"
#include "../submodules/progress.c/progress.h"
#include "../vendor/sqlite3.h"
#ifdef DEBUG_MEMORY
#include "../submodules/debug-memory/debug_memory.h"
#endif
#include "../submodules/fs.c/fs.h"
#include "../submodules/parson/parson.h"

typedef struct ParsedRGB            ParsedRGB;
typedef struct ParsedSeq            ParsedSeq;
typedef struct ParsedAnsiSeq        ParsedAnsiSeq;
typedef struct ParsedTruecolorSeq   ParsedTruecolorSeq;

struct ParsedTruecolorSeq { char *fg, *bg; };
struct ParsedAnsiSeq { char *fg, *bg; };
struct ParsedRGB { int red, green, blue; };
struct ParsedSeq { ParsedAnsiSeq *Ansi; ParsedTruecolorSeq *Truecolor; };
struct ParsedColor {
  size_t    Props;
  char      *Name, *Hex;
  int       Ansicode;
  ParsedRGB *RGB;
  ParsedSeq *Seq;
  char      *JSON;
  char      *PngEncodedContent;
  size_t    PngDecodedLength;
};


int parse_colors_json(parse_json_options *OPTIONS);
int iterate_parsed_results(parse_json_options *OPTIONS);
int free_parsed_results(parse_json_options *OPTIONS);

#endif
