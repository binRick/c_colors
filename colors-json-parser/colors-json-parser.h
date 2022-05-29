#pragma once
#ifndef JSON_PARSER_H
#define JSON_PARSER_H
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

typedef struct parse_json_options   parse_json_options;
typedef struct ParsedColor          ParsedColor;

#include "../submodules/djbhash/src/djbhash.h"
struct parse_json_options {
  char           *input_file, *duration;
  bool           verbose_mode;
  size_t         parse_qty;
  ParsedColor    **ParsedColors;
  size_t         ParsedColorsQty;
  struct djbhash ResultsHash;
};

#include "../submodules/bytes/bytes.h"
#include "../submodules/c_ansi/ansi-codes/ansi-codes.h"
#include "../submodules/c_stringfn/include/stringfn.h"
#include "../submodules/c_timer/include/c_timer.h"
#include "../submodules/debug-memory/debug_memory.h"
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
};


int parse_colors_json(parse_json_options *OPTIONS);
int iterate_parsed_results(parse_json_options *OPTIONS);
int free_parsed_results(parse_json_options *OPTIONS);

#endif
