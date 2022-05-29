#include "hex-png-pixel-creator.h"

int parse_args(int, char **);
int debug_args();
int write_png_images();

args_t args = {
  DEFAULT_INPUT,
  DEFAULT_OUTPUT_DIR,
  DEFAULT_MODE,
  DEFAULT_VERBOSE,
  DEFAULT_COUNT,
};


int main(int argc, char **argv) {
  parse_args(argc, argv);
  if ((strcmp(args.mode, "write") == 0)) {
    printf("writing images\n");
    return(0);
  }
  if ((strcmp(args.mode, "debug_args") == 0)) {
    return(debug_args());
  }
  if ((argc >= 2) && (strcmp(argv[1], "--test") == 0)) {
    hex_png_pixel("#ABFF55");
    hex_png_pixel("#5C22F5");
    hex_png_pixel("#AB8855");
    return(hex_png_pixel("#77FF55"));
  }

  printf(AC_RESETALL AC_RED "No mode selected: %s\n" AC_RESETALL, args.mode);
  return(1);
}


int debug_args(){
    fprintf(stderr,
    acs(AC_BRIGHT_BLUE_BLACK AC_ITALIC  "Verbose: %d")
    ", "
    ansistr(AC_RESETALL AC_REVERSE AC_BRIGHT_RED_BLACK "Input: %s")
    ", "
    ansistr(AC_RESETALL AC_BRIGHT_GREEN_BLACK "Output Directory: %s")
    ", "
    ansistr(AC_RESETALL AC_BRIGHT_GREEN_BLACK "Mode: %s")
    ", "
    ansistr(AC_RESETALL AC_BRIGHT_GREEN_BLACK "Count: %d")
    ", "
    "\n"
    , args.verbose, args.input, args.output_dir, args.mode, args.count
    );

  return(EXIT_SUCCESS);
}

int parse_args(int argc, char **argv){
  char               identifier;
  const char         *value;
  cag_option_context context;

  cag_option_prepare(&context, options, CAG_ARRAY_SIZE(options), argc, argv);
  while (cag_option_fetch(&context)) {
    identifier = cag_option_get(&context);

    switch (identifier) {
    case 'm':
      value      = cag_option_get_value(&context);
      args.mode = value;
      break;
    case 'i':
      value      = cag_option_get_value(&context);
      args.input = value;
      break;
    case 'o':
      value       = cag_option_get_value(&context);
      args.output_dir = value;
      break;
    case 'v':
      args.verbose = true;
      break;
    case 'c':
      value      = cag_option_get_value(&context);
      args.count = atoi(value);
      break;
    case 'h':
      fprintf(stderr, AC_RESETALL AC_YELLOW AC_BOLD "Usage: hex-png-pixel-creator [OPTION]\n" AC_RESETALL);
      cag_option_print(options, CAG_ARRAY_SIZE(options), stdout);
      exit(EXIT_SUCCESS);
    }
  }
  return(EXIT_SUCCESS);
} /* parse_args */
