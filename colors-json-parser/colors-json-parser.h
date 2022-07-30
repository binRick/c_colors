#pragma once
///////////////////////////////////////////////////////////////////////////
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
///////////////////////////////////////////////////////////////////////////
#include "ansi-codes/ansi-codes.h"
#include "bytes/bytes.h"
#include "c_stringfn/include/stringfn.h"
#include "c_timer/include/c_timer.h"
#include "db/db.h"
#include "djbhash/src/djbhash.h"
#include "fs.c/fs.h"
#include "hex-png-pixel-utils/hex-png-pixel-utils.h"
#include "parson/parson.h"
///////////////////////////////////////////////////////////////////////////
typedef struct parse_json_options   parse_json_options;
typedef struct ParsedColor          ParsedColor;
typedef struct ParsedRGB            ParsedRGB;
typedef struct ParsedSeq            ParsedSeq;
typedef struct ParsedAnsiSeq        ParsedAnsiSeq;
typedef struct ParsedTruecolorSeq   ParsedTruecolorSeq;
typedef void (*json_item_cb_t)(ParsedColor *PARSED_COLOR_ITEM);
///////////////////////////////////////////////////////////////////////////
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
///////////////////////////////////////////////////////////////////////////
int parse_colors_json(parse_json_options *OPTIONS);
int iterate_parsed_results(parse_json_options *OPTIONS);
int free_parsed_results(parse_json_options *OPTIONS);

///////////////////////////////////////////////////////////////////////////
