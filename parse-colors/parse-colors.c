#include "parse-colors.h"
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#define streq(a, b)    (strcmp(a, b) == 0)
#define DEBUG_TERMINAL_CAPABILITIES    false
#define ALT_SCREEN_MODE_ENABLED        true
void cleanup();
void restore_screen();
void setup_screen();
void load_new_palette_type_id();

args_t                 args = {
  DEFAULT_MODE,
  DEFAULT_VERBOSE,
  DEFAULT_COUNT,
  DEFAULT_PRETTY,
  DEFAULT_SQLITE_DB_PATH,
  DEFAULT_CSV_FILE,
  DEFAULT_JSON_FILE
};

TerminalCapabilities_t TerminalCapabilities = {
  .RestorePalette            = false,
  .AltScreenInitiallyEnabled = false,
  .IsTTY                     = false,
};


char *strdup_escaped(const char *tmp) {
  char *ret = malloc(strlen(tmp) * 4 + 1);
  char *dst = ret;

  for ( ; *tmp; tmp++) {
    if (*tmp >= ' ' && *tmp <= 126 && *tmp != '\\') {
      *dst = *tmp;
      ++dst;
    } else {
      dst += sprintf(dst, "\\x%02hhx", (unsigned char)*tmp);
    }
  }
  *dst = 0;
  return(ret);
}


void load_new_palette_type_id(int PALETTE_TYPE_ID){
  if (!TerminalCapabilities.RestorePalette || !TerminalCapabilities.IsTTY) {
    return;
  }
  struct StringFNStrings p = stringfn_split(WorkerPaletteTypes[PALETTE_TYPE_ID].Colors, ' ');

  if (args.verbose) {
    fprintf(stdout, "Loading %s Colors\n", WorkerPaletteTypes[PALETTE_TYPE_ID].Name);
  }
  char *new_palette_codes = malloc(1024);

  sprintf(new_palette_codes, NEWPALETTE,
          p.strings[18],
          p.strings[17],
          p.strings[16],
          p.strings[15],
          p.strings[14],
          p.strings[13],
          p.strings[12],
          p.strings[11],
          p.strings[10],
          p.strings[9],
          p.strings[8],
          p.strings[7],
          p.strings[6],
          p.strings[5],
          p.strings[4],
          p.strings[3],
          p.strings[2],
          p.strings[1],
          p.strings[0]
          );

  printf("%s", new_palette_codes);
  free(new_palette_codes);
  stringfn_release_strings_struct(p);
}


void print_color_name_handler(ParsedColor *PARSED_COLOR_ITEM){
  if (VERBOSE_DEBUG_HANDLER || args.verbose) {
    fprintf(stderr, "\nhandler, name: %s!\n", PARSED_COLOR_ITEM->Name);
  }
}


int main(int argc, char **argv) {
  parse_args(argc, argv);
  if ((argc >= 2) && (strcmp(argv[1], "--test") == 0)) {
    printf("Test OK\n"); return(0);
  }
  TerminalCapabilities.IsTTY = isatty(STDIN_FILENO);


  load_new_palette_type_id(DEFAULT_PALETTE);
  if ((strcmp(args.mode, "debug_args") == 0)) {
    load_new_palette_type_id(ARGS_PALETTE);
    return(debug_args());
  }else if ((strcmp(args.mode, "csv") == 0)) {
    load_new_palette_type_id(CSV_PALETTE);
    parse_csv_options *options = malloc(sizeof(parse_csv_options));
    options->input_file       = args.csv_file;
    options->verbose_mode     = args.verbose;
    options->pretty_json_mode = args.pretty;
    options->output_file      = args.json_file;
    options->parse_qty        = args.count;
    return(parse_colors_csv(options));
  }else if ((strcmp(args.mode, "typeids_hash") == 0)) {
    load_new_palette_type_id(TYPEIDS_PALETTE);
    ColorsDB       *DB = malloc(sizeof(ColorsDB));
    DB->Path = args.sqlite_file;
    struct djbhash TYPEIDS_HASH = db_get_typeids_hash(DB);
    djbhash_reset_iterator(&TYPEIDS_HASH);
    fprintf(stdout,
            AC_BRIGHT_BLUE_BLACK AC_ITALIC  "TypeIDs Hash qty:"
            AC_RESETALL " "
            AC_RESETALL AC_GREEN AC_BOLD AC_REVERSED "%d" AC_RESETALL
            "\n",
            TYPEIDS_HASH.active_count
            );

    return(TYPEIDS_HASH.active_count > 0);
  }else if ((strcmp(args.mode, "typeids") == 0)) {
    load_new_palette_type_id(TYPEIDS_PALETTE);
    ColorsDB *DB = malloc(sizeof(ColorsDB));
    DB->Path = args.sqlite_file;
    return(db_list_typeids(DB));
  }else if ((strcmp(args.mode, "db") == 0)) {
    load_new_palette_type_id(DB_PALETTE);
    ColorsDB *DB = malloc(sizeof(ColorsDB));
    DB->Path = args.sqlite_file;
    return(db_list_ids(DB));
  }else if ((strcmp(args.mode, "json") == 0)) {
    load_new_palette_type_id(JSON_PALETTE);
    parse_json_options *options = malloc(sizeof(parse_json_options));
    options->DB                 = malloc(sizeof(ColorsDB));
    options->DB->Path           = args.sqlite_file;
    options->input_file         = args.json_file;
    options->verbose_mode       = args.verbose;
    options->parse_qty          = args.count;
    options->ParsedColorHandler = print_color_name_handler;
    if (init_colors_db(options->DB) != 0) {
      printf("failed to initialize DB!\n");
      exit(1);
    }
    int r = parse_colors_json(options);
    if (options) {
      free(options);
    }
    return(r);
  }

  printf(AC_RESETALL AC_RED "No mode selected: %s\n" AC_RESETALL, args.mode);
  return(1);
} /* main */


int debug_args(){
  fprintf(stderr,
          acs(AC_BRIGHT_BLUE_BLACK AC_ITALIC  "Verbose: %d") "\n"
          ansistr(AC_RESETALL AC_BRIGHT_GREEN_BLACK "Mode: %s") "\n"
          ansistr(AC_RESETALL AC_UNDERLINE "Count: %d") "\n"
          ansistr(AC_RESETALL AC_UNDERLINE "Pretty Mode: %d") "\n"
          ansistr(AC_RESETALL AC_UNDERLINE "Sqlite File: %s") "\n"
          ansistr(AC_RESETALL AC_UNDERLINE "JSON File: %s") "\n"
          ansistr(AC_RESETALL AC_UNDERLINE "CSV File: %s") "\n"
          ,
          args.verbose,
          args.mode,
          args.count,
          args.pretty,
          args.sqlite_file,
          args.json_file,
          args.csv_file
          );

  return(EXIT_SUCCESS);
}


int parse_args(int argc, char *argv[]){
  char               identifier;
  const char         *value;
  cag_option_context context;

  cag_option_prepare(&context, options, CAG_ARRAY_SIZE(options), argc, argv);
  while (cag_option_fetch(&context)) {
    identifier = cag_option_get(&context);

    switch (identifier) {
    case 'm':
      value     = cag_option_get_value(&context);
      args.mode = value;
      break;
    case 'v':
      args.verbose = true;
      break;
    case 'c':
      value      = cag_option_get_value(&context);
      args.count = atoi(value);
      break;
    case 'p':
      args.pretty = true;
      break;
    case 'D':
      value            = cag_option_get_value(&context);
      args.sqlite_file = value;
      break;
    case 'C':
      value         = cag_option_get_value(&context);
      args.csv_file = value;
      break;
    case 'J':
      value          = cag_option_get_value(&context);
      args.json_file = value;
      break;
    case 'h':
      fprintf(stderr, AC_RESETALL AC_YELLOW AC_BOLD "Usage: parse-colors [OPTION]\n" AC_RESETALL);
      cag_option_print(options, CAG_ARRAY_SIZE(options), stdout);
      exit(EXIT_SUCCESS);
    }
  }
  return(EXIT_SUCCESS);
} /* parse_args */

