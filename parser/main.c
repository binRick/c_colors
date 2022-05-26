#include "parser.h"


void ansi_256_fg(FILE *file, int color) {
    fprintf(file, C256_FG_CODE, color);
}

void ansi_256_bg(FILE *file, int color) {
    fprintf(file, C256_BG_CODE, color);
}

void ansi_truecolor_fg(FILE *file, int r, int g, int b) {
    fprintf(file, TRUECOLOR_FG_CODE, r, g, b);
}

void ansi_truecolor_bg(FILE *file, int r, int g, int b) {
    fprintf(file, TRUECOLOR_BG_CODE, r, g, b);
}

void ansi_reset(FILE *file) {
    fputs(RESET_CODE, file);
}
args_t args = {
    DEFAULT_INPUT,
    DEFAULT_OUTPUT,
    DEFAULT_MODE,
    DEFAULT_VERBOSE,
    DEFAULT_COUNT,
};


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
    , args.verbose, args.input, args.output, args.mode, args.count, args.pretty
    );

  return(EXIT_SUCCESS);
}

int main(int argc, char **argv) {
  if ((argc >= 2) && (strcmp(argv[1], "--test") == 0)) {
    printf("Test OK\n"); return(0);
  }
  parse_args(argc, argv);
  assert(debug_args() == 0);

  
  if ((strcmp(args.mode, "csv") == 0)) return(parse_csv());

  printf(AC_RESETALL AC_RED "No mode selected: %s\n" AC_RESETALL, args.mode); return(0);

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
      value    = cag_option_get_value(&context);
      args.input = value;
      break;
    case 'm':
      value    = cag_option_get_value(&context);
      args.mode = value;
      break;
    case 'o':
      value    = cag_option_get_value(&context);
      args.output = value;
      break;
    case 'p':
      args.pretty = true;
      break;
    case 'C':
      args.color = true;
      break;
    case 'v':
      args.verbose = true;
      break;
    case 'c':
      value    = cag_option_get_value(&context);
      args.count = atoi(value);
      break;
    case 'h':
      printf(AC_RESETALL AC_YELLOW AC_BOLD "Usage: cargsdemo [OPTION]...\n" AC_RESETALL);
      printf("Demonstrates the cargs library.\n\n");
      cag_option_print(options, CAG_ARRAY_SIZE(options), stdout);
      printf("\nNote that all formatting is done by cargs.\n");
      return(EXIT_SUCCESS);
    }
  }


  return(EXIT_SUCCESS);
}

int parse_csv(){
  char                   *content = fs_read(args.input);
  struct StringFNStrings Lines    = stringfn_split_lines_and_trim(content);
  if(args.verbose)
      fprintf(stderr, "%d lines\n", Lines.count);
  int         qty = 0, ui;

  short       ok;
  char        *LINE, *NAME, *HEX, *colp, *ptr, *js;
  uint32_t    r;
  JSON_Value  *o;
  JSON_Object *O;

  for (int i = 0; i < Lines.count && ((i+1) < args.count); i++) {
    LINE = stringfn_trim(Lines.strings[i]);
    struct StringFNStrings SPLIT = stringfn_split(LINE, ',');
    free(LINE);
    NAME = stringfn_trim(SPLIT.strings[0]);
    if (strlen(NAME) < 3) {
      continue;
    }
    HEX = stringfn_to_uppercase(stringfn_trim(SPLIT.strings[1]));
    if (!stringfn_starts_with(HEX, "#")) {
      char *ns = malloc(strlen(HEX) + 1);
      sprintf(ns, "#%s", HEX);
      HEX = strdup(ns);
      free(ns);
    }
    if (strlen(HEX) != 7) {
      continue;
    }
    ////////////////////
    ColorInfo *C = malloc(sizeof(ColorInfo));
    C->name = strdup(NAME);
    free(NAME);
    C->hex = strdup(HEX);
    free(HEX);
    r = rgba_from_string(C->hex, &ok);
    rgba_t rgba = rgba_new(r);
    ////////////////////
    ui = strtoul(C->hex, &ptr, 16);
    ////////////////////
    C->ansicode      = hex_to_closest_ansi_code(ui);
    C->rgb           = malloc(sizeof(RGB_t));
    C->ansi          = malloc(sizeof(Ansi_t));
    C->truecolor     = malloc(sizeof(Ansi_t));
    C->truecolor->fg = malloc(32);
    C->truecolor->bg = malloc(32);
    C->ansi->fg      = malloc(32);
    C->ansi->bg      = malloc(32);
    ////////////////////
    C->rgb->red   = (uint32_t)r >> 24 & 0xff;
    C->rgb->green = (uint32_t)r >> 16 & 0xff;
    C->rgb->blue  = (uint32_t)r >> 8 & 0xff;
    C->rgb->alpha = (uint32_t)r & 0xff;
    ////////////////////
    sprintf(C->truecolor->fg, TRUECOLOR_FG_CODE, C->rgb->red, C->rgb->green, C->rgb->blue);
    sprintf(C->truecolor->bg, TRUECOLOR_BG_CODE, C->rgb->red, C->rgb->green, C->rgb->blue);
    sprintf(C->ansi->fg, "\x1b[38;5;%dm", C->ansicode);
    sprintf(C->ansi->bg, "\x1b[48;5;%dm", C->ansicode);
    ////////////////////

    /////////////////////////////////////////////////////////////////////////
    o = json_value_init_object();
    O = json_value_get_object(o);
    json_object_set_string(O, "name", C->name);
    json_object_set_string(O, "hex", C->hex);
    json_object_dotset_number(O, "ansicode", C->ansicode);
    json_object_dotset_number(O, "rgb.red", C->rgb->red);
    json_object_dotset_number(O, "rgb.green", C->rgb->green);
    json_object_dotset_number(O, "rgb.blue", C->rgb->blue);
    json_object_dotset_string(O, "seq.ansi.fg", C->ansi->fg);
    json_object_dotset_string(O, "seq.ansi.bg", C->ansi->bg);
    json_object_dotset_string(O, "seq.truecolor.fg", C->truecolor->fg);
    json_object_dotset_string(O, "seq.truecolor.bg", C->truecolor->bg);
    /////////////////////////////////////////////////////////////////////////
    if (args.pretty) {
      js = json_serialize_to_string_pretty(o);
    }else{
      js = json_serialize_to_string(o);
    }
    if (args.color) {
      ansi_truecolor_bg(stdout, C->rgb->red, C->rgb->green, C->rgb->blue);
    }
    fprintf(stdout, "%s\n", js);
    if (args.color) {
      printf(AC_RESETALL);
    }


    qty++;
  }
  return(EXIT_SUCCESS);
}


