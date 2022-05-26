#include "../include/colors.h"
#include "../submodules/c_stringfn/include/stringfn.h"
#include "../submodules/fs.c/fs.h"
#include "../submodules/rgba/src/rgba.h"
#include "../subprojects/parson/parson.h"
#include "../vendor/ansicodes/ansicodes.h"
#include "../vendor/ansicodes/rgb2ansi.c"
#include "../vendor/ansicodes/rgbansi.c"
#include "../vendor/ansicodes/rgbansi.h"
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


void ansi_dev(){
  for (int g = 0; g < 6; g++) {
    if (g > 0) {
      printf("\n");
    }
    print_cube(stdout, g);
  }

  for (int i = 0; i < 256; i++) {
    show_color(stdout, i);
  }
}


int main(int argc, char **argv) {
  if ((argc >= 2) && (strcmp(argv[1], "--test") == 0)) {
    printf("Test OK\n"); return(0);
  }
  // dd();
  // exit(0);
  char                   *p = "vendor/colornames/colornames.json";
  char                   *content = fs_read(p);
  struct StringFNStrings Lines = stringfn_split_lines_and_trim(content);
  char                   *hex, *color_name;
  JSON_Value             *color;
  int                    qty = 0;

  for (int i = 0; i < Lines.count; i++) {
    if (strlen(Lines.strings[i]) < 3) {
      continue;
    }
    ColorInfo *C = malloc(sizeof(ColorInfo));
    C->rgb  = malloc(sizeof(RGB_t));
    C->ansi = malloc(sizeof(Ansi_t));
    color   = json_parse_string(Lines.strings[i]);
    C->name = json_object_get_string(json_object(color), "name");
    hex     = json_object_get_string(json_object(color), "hex");
    char *h = malloc(1024);
    sprintf(h, "%s%s", "#", (char *)hex);
    C->hex = strdup(h);

    short    ok;

    uint32_t r    = rgba_from_string(h, &ok);
    rgba_t   rgba = rgba_new(r);
    C->ansi->fg   = malloc(32);
    C->ansi->bg   = malloc(32);
    C->rgb->red   = (uint32_t)r >> 24 & 0xff;
    C->rgb->green = (uint32_t)r >> 16 & 0xff;
    C->rgb->blue  = (uint32_t)r >> 8 & 0xff;
    C->rgb->alpha = (uint32_t)r & 0xff;
    sprintf(C->ansi->fg, TRUECOLOR_FG_CODE, C->rgb->red, C->rgb->green, C->rgb->blue);
    sprintf(C->ansi->bg, TRUECOLOR_BG_CODE, C->rgb->red, C->rgb->green, C->rgb->blue);
    JSON_Value  *o = json_value_init_object();
    JSON_Object *O = json_value_get_object(o);
    char        *S = NULL;
    json_object_set_string(O, "name", C->name);
    json_object_set_string(O, "hex", C->hex);
    char *colp, *ptr;
    int  ui      = strtoul(hex, &ptr, 16);
    int  __clos  = hex_to_closest_ansi_code(ui);
    char *ansifg = malloc(1024);
    char *ansibg = malloc(1024);
    sprintf(ansifg, "\x1b[38;5;%dm", __clos);
    sprintf(ansibg, "\x1b[48;5;%dm", __clos);


    json_object_dotset_number(O, "rgb.red", C->rgb->red);
    json_object_dotset_number(O, "rgb.green", C->rgb->green);
    json_object_dotset_number(O, "rgb.blue", C->rgb->blue);

    json_object_dotset_number(O, "ansi.code", __clos);
    json_object_dotset_string(O, "ansi.seq.256.fg", ansifg);
    json_object_dotset_string(O, "ansi.seq.256.bg", ansibg);
    json_object_dotset_string(O, "ansi.seq.truecolor.fg", C->ansi->fg);
    json_object_dotset_string(O, "ansi.seq.truecolor.bg", C->ansi->bg);

    char *js  = json_serialize_to_string(o);
    char *pjs = json_serialize_to_string_pretty(o);
    if (i < 100 || (i > (Lines.count - 100))) {
      ansi_truecolor_bg(stdout, C->rgb->red, C->rgb->green, C->rgb->blue);
      printf("#%d> %s\n", qty, pjs);
      ansi_reset(stdout);
    }
    json_free_serialized_string(S);

    json_value_free(O);
    qty++;
  }
  json_value_free(color);
  printf("Test OK\n"); return(0);

  return(EXIT_SUCCESS);
} /* main */
