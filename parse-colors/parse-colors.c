#define VERBOSE_DEBUG_HANDLER false
#include "../db/db.h"
#include "parse-colors.h"
args_t args = {
  DEFAULT_CSV_INPUT,
  DEFAULT_OUTPUT,
  DEFAULT_MODE,
  DEFAULT_VERBOSE,
  DEFAULT_COUNT,
  DEFAULT_COLOR,
};

void cleanup();


void print_color_name_handler(ParsedColor *PARSED_COLOR_ITEM){
  if (VERBOSE_DEBUG_HANDLER) {
    fprintf(stderr, "\nhandler, name: %s!\n", PARSED_COLOR_ITEM->Name);
  }
}


int main(int argc, char **argv) {
  parse_args(argc, argv);
  if ((argc >= 2) && (strcmp(argv[1], "--test") == 0)) {
    printf("Test OK\n"); return(0);
  }


  if ((strcmp(args.mode, "debug_args") == 0)) {
    return(debug_args());
  }

  if ((strcmp(args.mode, "csv") == 0)) {
    parse_csv_options *options = malloc(sizeof(parse_csv_options));
    options->input_file       = args.input;
    options->verbose_mode     = args.verbose;
    options->pretty_json_mode = args.pretty;
    options->output_file      = args.output;
    options->parse_qty        = args.count;
    return(parse_colors_csv(options));
  }
  if ((strcmp(args.mode, "db") == 0)) {
    ColorsDB *DB = malloc(sizeof(ColorsDB));
    DB->Path = COLOR_NAMES_DB_PATH;
    return(db_list_ids(DB));
  }

  if ((strcmp(args.mode, "json") == 0)) {
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


void cleanup(){
  if (args.verbose) {
#ifdef DEBUG_MEMORY
    print_allocated_memory();
#endif
  }
}

