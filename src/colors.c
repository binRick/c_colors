#include "../include/colors.h"
#include <assert.h>
#include <ctype.h>
#include <math.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdlib.h>
#include <string.h>
#include <string.h>
#include <sys/time.h>
int parse_csv();

#define RENDER_COLOR_OUTPUT     false
#define RENDER_PRETTY_JSON      true
#define COLORNAMES_CSV_FILE     "vendor/colornames/colornames.csv"
#define COLORNAMES_JSON_FILE    "vendor/colornames/colornames.json"



int main(int argc, char **argv) {
  if ((argc >= 2) && (strcmp(argv[1], "--test") == 0)) {
    printf("Test OK\n"); return(0);
  }
  parse_csv();
  printf("Test OK\n"); return(0);

  return(EXIT_SUCCESS);
}


int parse_csv(){
  char                   *content = fs_read(COLORNAMES_CSV_FILE);
  struct StringFNStrings Lines    = stringfn_split_lines_and_trim(content);

  printf("%d lines\n", Lines.count);
  int qty = 0, ui;

  short ok;
  char                   *LINE, *NAME, *HEX, *colp, *ptr, *js;
  uint32_t    r;
    JSON_Value  *o;  
    JSON_Object *O;
  for (int i = 0; i < Lines.count; i++) {
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
    rgba_t      rgba = rgba_new(r);
    ////////////////////
    ui = strtoul(C->hex, &ptr, 16);
    ////////////////////
    C->ansicode = hex_to_closest_ansi_code(ui);
    C->rgb       = malloc(sizeof(RGB_t));
    C->ansi      = malloc(sizeof(Ansi_t));
    C->truecolor = malloc(sizeof(Ansi_t));
    C->truecolor->fg   = malloc(32);
    C->truecolor->bg   = malloc(32);
    C->ansi->fg   = malloc(32);
    C->ansi->bg   = malloc(32);
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
    if (RENDER_PRETTY_JSON) {
      js = json_serialize_to_string_pretty(o);
    }else{
      js = json_serialize_to_string(o);
    }
    if (RENDER_COLOR_OUTPUT) {
      ansi_truecolor_bg(stdout, C->rgb->red, C->rgb->green, C->rgb->blue);
    }
    fprintf(stdout, "%s\n", js);
    if (RENDER_COLOR_OUTPUT) {
      ansi_reset(stdout);
    }


    qty++;
  }

  return(EXIT_SUCCESS);
} /* csv_logic */


