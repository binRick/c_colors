#include "parse-colors.h"
#include "termpaint.h"
#include "termpaintx.h"
#include "termpaintx_ttyrescue.h"
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#define streq(a, b) (strcmp(a, b) == 0)
#define DEBUG_TERMINAL_CAPABILITIES false
#define ALT_SCREEN_MODE_ENABLED true
void cleanup();
void restore_screen();
void setup_screen();
void load_new_palette_type_id();

args_t                 args = {
  DEFAULT_CSV_INPUT,
  DEFAULT_OUTPUT,
  DEFAULT_MODE,
  DEFAULT_VERBOSE,
  DEFAULT_COUNT,
  DEFAULT_COLOR,
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


void null_callback(void *ctx, termpaint_event *event) {
  (void)ctx; (void)event;
}

typedef struct {
  int        id;
  const char *name;
  _Bool      state;
} Cap;

Cap  caps[] = {
#define C(name) { TERMPAINT_CAPABILITY_ ## name, #name, 0 }
  C(CSI_POSTFIX_MOD),
  C(TITLE_RESTORE),
  C(MAY_TRY_CURSOR_SHAPE_BAR),
  C(CURSOR_SHAPE_OSC50),
  C(EXTENDED_CHARSET),
  C(TRUECOLOR_MAYBE_SUPPORTED),
  C(TRUECOLOR_SUPPORTED),
  C(88_COLOR),
  C(CLEARED_COLORING),
  C(7BIT_ST),
  C(MAY_TRY_TAGGED_PASTE),
  C(TRUECOLOR_SUPPORTED),
  C(SAFE_POSITION_REPORT),
  C(EXTENDED_CHARSET),
#undef C
  { 0,                             NULL,  0 }
};

char *debug     = NULL;
bool debug_used = true;


void debug_log(termpaint_integration *integration, const char *data, int length) {
  (void)integration;
  if (debug_used && !debug) {
    return;                           // memory allocaton failure
  }
  if (debug) {
    const int oldlen     = strlen(debug);
    char      *debug_old = debug;
    debug = realloc(debug, oldlen + length + 1);
    if (debug) {
      memcpy(debug + oldlen, data, length);
      debug[oldlen + length] = 0;
    } else {
      free(debug_old);
    }
  } else {
    debug = strndup(data, length);
  }
  debug_used = true;
}


void cleanup(){
  if (args.verbose) {
#ifdef DEBUG_MEMORY
    print_allocated_memory();
#endif
  }
  exit(0);
}


int TermpaintQuery(){
  termpaint_integration *integration = termpaintx_full_integration("+kbdsigint +kbdsigtstp");

  if (!integration) {
    puts("Could not init termpaint!");
    return(1);
  }
  termpaint_integration_set_logging_func(integration, debug_log);
  termpaint_terminal *terminal = termpaint_terminal_new(integration);

  termpaint_terminal_set_log_mask(terminal, TERMPAINT_LOG_AUTO_DETECT_TRACE | TERMPAINT_LOG_TRACE_RAW_INPUT);
  termpaint_terminal_set_event_cb(terminal, null_callback, NULL);
  termpaintx_full_integration_set_terminal(integration, terminal);
  termpaint_terminal_auto_detect(terminal);
  termpaintx_full_integration_wait_for_ready_with_message(integration, 3000, "Terminal auto detection is taking unusually long, press space to abort.");

  char buff[1000];
  char *self_reported_name_and_version = NULL;

  termpaint_terminal_auto_detect_result_text(terminal, buff, sizeof(buff));
  if (termpaint_terminal_self_reported_name_and_version(terminal)) {
    self_reported_name_and_version = strdup_escaped(termpaint_terminal_self_reported_name_and_version(terminal));
  }
  termpaint_terminal_free_with_restore(terminal);
  for (Cap *c = caps; c->name; c++) {
    c->state = termpaint_terminal_capable(terminal, c->id);
    if (strcmp(c->name, "TITLE_RESTORE") == 0) {
      TerminalCapabilities.RestorePalette = c->state;
    }
    if (DEBUG_TERMINAL_CAPABILITIES || args.verbose) {
      printf(AC_RESETALL AC_YELLOW AC_ITALIC "%30s" AC_RESETALL
             " => "
             "[" AC_REVERSED AC_BOLD "%3s" AC_RESETALL "]"
             "\n",
             c->name,
             c->state ? AC_GREEN "Yes" : AC_RED "No"
             );
    }
  }
  if (DEBUG_TERMINAL_CAPABILITIES || args.verbose) {
    printf(AC_RED AC_BOLD "%s" AC_RESETALL "\n", self_reported_name_and_version);
    printf(AC_RED AC_BOLD "%s" AC_RESETALL "\n", buff);
    printf(AC_BLUE AC_BOLD "Restore Palette?" AC_RESETALL " " "%s" AC_RESETALL "\n", TerminalCapabilities.RestorePalette ? AC_GREEN "Yes" : AC_RED "No");
    sleep(0);
  }
  return(EXIT_SUCCESS);
} /* QUERY_TERMINAL */


void setup_screen(){
  if (ALT_SCREEN_MODE_ENABLED) {
    printf(AC_ALT_SCREEN_ON);
  }
  TermpaintQuery();
  if (TerminalCapabilities.RestorePalette) {
    printf(AC_SAVE_PALETTE);
  }
  printf(AC_HIDE_CURSOR);
  signal(SIGTERM, restore_screen);
  signal(SIGQUIT, restore_screen);
  signal(SIGINT, restore_screen);
  atexit(restore_screen);
}


void restore_screen(){
  printf(AC_SHOW_CURSOR);
  if (ALT_SCREEN_MODE_ENABLED) {
    printf(AC_ALT_SCREEN_OFF);
  }
  if (TerminalCapabilities.RestorePalette) {
    printf(AC_RESTORE_PALETTE);
  }
  cleanup();
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

  if (TerminalCapabilities.IsTTY) {
    setup_screen();
  }

  load_new_palette_type_id(DEFAULT_PALETTE);
  if ((strcmp(args.mode, "debug_args") == 0)) {
    load_new_palette_type_id(ARGS_PALETTE);
    return(debug_args());
  }

  if ((strcmp(args.mode, "csv") == 0)) {
    load_new_palette_type_id(CSV_PALETTE);
    parse_csv_options *options = malloc(sizeof(parse_csv_options));
    options->input_file       = args.input;
    options->verbose_mode     = args.verbose;
    options->pretty_json_mode = args.pretty;
    options->output_file      = args.output;
    options->parse_qty        = args.count;
    return(parse_colors_csv(options));
  }
  if ((strcmp(args.mode, "typeids_hash") == 0)) {
    load_new_palette_type_id(TYPEIDS_PALETTE);
    ColorsDB       *DB = malloc(sizeof(ColorsDB));
    DB->Path = COLOR_NAMES_DB_PATH;
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
  }
  if ((strcmp(args.mode, "typeids") == 0)) {
    load_new_palette_type_id(TYPEIDS_PALETTE);
    ColorsDB *DB = malloc(sizeof(ColorsDB));
    DB->Path = COLOR_NAMES_DB_PATH;
    return(db_list_typeids(DB));
  }
  if ((strcmp(args.mode, "db") == 0)) {
    load_new_palette_type_id(DB_PALETTE);
    ColorsDB *DB = malloc(sizeof(ColorsDB));
    DB->Path = COLOR_NAMES_DB_PATH;
    return(db_list_ids(DB));
  }

  if ((strcmp(args.mode, "json") == 0)) {
    load_new_palette_type_id(JSON_PALETTE);
    args.input = DEFAULT_JSON_INPUT;
    parse_json_options *options = malloc(sizeof(parse_json_options));
    options->DB                 = malloc(sizeof(ColorsDB));
    options->DB->Path           = COLOR_NAMES_DB_PATH;
    options->input_file         = args.input;
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
    cleanup();
    return(r);
  }

  printf(AC_RESETALL AC_RED "No mode selected: %s\n" AC_RESETALL, args.mode);
  return(1);
} /* main */


int debug_args(){
  fprintf(stderr,
          acs(AC_BRIGHT_BLUE_BLACK AC_ITALIC  "Verbose: %d")
          ", "
          ansistr(AC_RESETALL AC_REVERSE AC_BRIGHT_RED_BLACK "Input: %s")
          ", "
          ansistr(AC_RESETALL AC_BRIGHT_GREEN_BLACK "Output: %s")
          ", "
          ansistr(AC_RESETALL AC_BRIGHT_GREEN_BLACK "Mode: %s")
          ", "
          ansistr(AC_RESETALL AC_UNDERLINE "Count: %d")
          "\n"
          ansistr(AC_RESETALL AC_UNDERLINE "Pretty Mode: %d")
          "\n"
          ansistr(AC_RESETALL AC_UNDERLINE "Color Mode: %d")
          "\n",
          args.verbose, args.input, args.output, args.mode, args.count, args.pretty, args.color
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
    case 'i':
      value      = cag_option_get_value(&context);
      args.input = value;
      break;
    case 'm':
      value     = cag_option_get_value(&context);
      args.mode = value;
      break;
    case 'o':
      value       = cag_option_get_value(&context);
      args.output = value;
      break;
    case 'p':
      args.pretty = true;
      break;
    case 'x':
      args.color = true;
      break;
    case 'v':
      args.verbose = true;
      break;
    case 'c':
      value      = cag_option_get_value(&context);
      args.count = atoi(value);
      break;
    case 'h':
      fprintf(stderr, AC_RESETALL AC_YELLOW AC_BOLD "Usage: parse-colors [OPTION]\n" AC_RESETALL);
      cag_option_print(options, CAG_ARRAY_SIZE(options), stdout);
      exit(EXIT_SUCCESS);
    }
  }
  return(EXIT_SUCCESS);
} /* parse_args */

