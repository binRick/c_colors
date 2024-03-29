#pragma once
//#define DEBUG_MEMORY_ENABLED
#define PROGRESS_BAR_WIDTH      40
#define BG_PROGRESS_BAR_CHAR    "."
#define PROGRESS_BAR_CHAR       "="
#include "colors-csv-parser/colors-csv-parser.h"
#include "hsluv-c/src/hsluv.h"
#include "slug.c/src/slug.h"

static void csv_progress_start(progress_data_t *data) {
  assert(data);
  fprintf(stdout,
          AC_HIDE_CURSOR
          AC_RESETALL AC_GREEN AC_ITALIC "Processing" AC_RESETALL
          AC_RESETALL " "
          AC_RESETALL AC_BLUE AC_REVERSED AC_BOLD "%d"
          AC_RESETALL " "
          AC_RESETALL AC_GREEN AC_ITALIC "CSV Lines" AC_RESETALL
          "\n",
          data->holder->total
          );
  progress_write(data->holder);
}

static void csv_progress(progress_data_t *data) {
  progress_write(data->holder);
}

static void csv_progress_end(progress_data_t __attribute__((unused)) *data) {
  fprintf(stdout,
          AC_SHOW_CURSOR
          AC_RESETALL "\n"
          AC_GREEN AC_REVERSED AC_BOLD "Complete" AC_RESETALL
          "\n"
          );
}

int parse_colors_csv(parse_csv_options *OPTIONS){
  bool                WRITE_TO_FILE = false;
  struct StringBuffer *sb           = stringbuffer_new_with_options(1024, true);

  if (OPTIONS->output_file != NULL && strlen(OPTIONS->output_file) > 1)
    WRITE_TO_FILE = true;
  char                   *content = fs_read(OPTIONS->input_file);
  struct StringFNStrings Lines    = stringfn_split_lines_and_trim(content);

  if (OPTIONS->verbose_mode)
    fprintf(stderr, "%d lines\n", Lines.count);
  size_t      qty = 0;
  short       ok;
  char        *LINE, *NAME, *HEX, *js;
  uint32_t    r;
  char        *sl = "none";
  JSON_Value  *o;
  JSON_Object *O;
  progress_t  *progress = progress_new(Lines.count, PROGRESS_BAR_WIDTH);

  progress->fmt      = "    Progress (:percent) => {:bar} [:elapsed]";    progress->bg_bar_char = BG_PROGRESS_BAR_CHAR;
  progress->bar_char = PROGRESS_BAR_CHAR;    progress_on(progress, PROGRESS_EVENT_START, csv_progress_start);
  progress_on(progress, PROGRESS_EVENT_PROGRESS, csv_progress);    progress_on(progress, PROGRESS_EVENT_END, csv_progress_end);

  for (size_t i = 0; i < (size_t)Lines.count && ((size_t)qty < (size_t)OPTIONS->parse_qty); i++) {
    LINE = stringfn_trim(Lines.strings[i]);
    struct StringFNStrings SPLIT = stringfn_split(LINE, ',');
    free(LINE);
    NAME = stringfn_trim(SPLIT.strings[0]);
    if (strlen(NAME) < 3)
      continue;
    HEX = stringfn_to_uppercase(stringfn_trim(SPLIT.strings[1]));
    if (!stringfn_starts_with(HEX, "#")) {
      char *ns = malloc(strlen(HEX) + 1);
      sprintf(ns, "#%s", HEX);
      HEX = strdup(ns);
      free(ns);
    }
    if (strlen(HEX) != 7)
      continue;
    ////////////////////
    ColorInfo *C = malloc(sizeof(ColorInfo));
    C->name = strdup(NAME);
    free(NAME);
    C->hex = strdup(HEX);
    free(HEX);
    r = rgba_from_string(C->hex, &ok);
    ////////////////////
    C->ansicode      = hex_to_256_color_ansicode(C->hex);
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
    double hsluv[3];
    rgb2hsluv(C->rgb->red, C->rgb->green, C->rgb->blue, &hsluv[0], &hsluv[1], &hsluv[2]);
    /////////////////////////////////////////////////////////////////////////
    o = json_value_init_object();
    O = json_value_get_object(o);
    EncodedPngResult PNG_RESULT = hex_to_png_encoded_bytes(C->hex);
    json_object_set_string(O, "name", C->name);
    if (C->name && stringfn_is_ascii(C->name)) {
      sl = stringfn_trim(stringfn_to_lowercase(C->name));
      stringfn_mut_replace(sl, ' ', '-');
      stringfn_mut_replace(sl, '\'', '-');
      stringfn_mut_replace(sl, '"', '-');
    }
    json_object_set_string(O, "slug", sl);
    json_object_set_string(O, "hex", C->hex);
    json_object_dotset_number(O, "hsl.hue", (int)hsluv[0]);
    json_object_dotset_number(O, "hsl.saturation", (int)hsluv[1]);
    json_object_dotset_number(O, "hsl.brightness", (int)(hsluv[2] / 100));
    json_object_dotset_boolean(O, "hsl.bright", (hsluv[2] / 100 > 50)?true:false);
    json_object_dotset_boolean(O, "hsl.very_bright", (hsluv[2] / 100 > 90)?true:false);
    json_object_dotset_boolean(O, "hsl.dark", (hsluv[2] / 100 < 30)?true:false);
    json_object_dotset_boolean(O, "hsl.very_dark", (hsluv[2] / 100 < 10)?true:false);
    json_object_set_string(O, "encoded_png_content", PNG_RESULT.EncodedContent);
    json_object_set_number(O, "decoded_png_length", PNG_RESULT.DecodedLength);
    json_object_dotset_number(O, "ansicode", C->ansicode);
    json_object_dotset_number(O, "rgb.red", C->rgb->red);
    json_object_dotset_number(O, "rgb.green", C->rgb->green);
    json_object_dotset_number(O, "rgb.blue", C->rgb->blue);
    json_object_dotset_string(O, "seq.ansi.fg", C->ansi->fg);
    json_object_dotset_string(O, "seq.ansi.bg", C->ansi->bg);
    json_object_dotset_string(O, "seq.truecolor.fg", C->truecolor->fg);
    json_object_dotset_string(O, "seq.truecolor.bg", C->truecolor->bg);
    /////////////////////////////////////////////////////////////////////////
    if (OPTIONS->pretty_json_mode)
      js = json_serialize_to_string_pretty(o);
    else
      js = json_serialize_to_string(o);
    qty++;
    if (!WRITE_TO_FILE) {
      if (OPTIONS->output_color_mode)
        ansi_truecolor_bg(stdout, C->rgb->red, C->rgb->green, C->rgb->blue);
      fprintf(stdout, "%s\n", js);
      if (OPTIONS->output_color_mode)
        printf(AC_RESETALL);
    }else{
      stringbuffer_append_string(sb, js);
      stringbuffer_append_string(sb, "\n");
    }
    {
      free(C->truecolor->fg);
      free(C->truecolor->bg);
      free(C->truecolor);
      free(C->ansi->fg);
      free(C->ansi->bg);
      free(C->rgb);
      free(C->ansi);
      free(C);
    }
    progress_value(progress, i + 1);
  }
  stringbuffer_append_string(sb, "\n");
  progress_value(progress, Lines.count);
  progress_free(progress);
  if (WRITE_TO_FILE) {
    int wrote_bytes = fs_write(OPTIONS->output_file, stringbuffer_to_string(sb));
    fprintf(stderr,
            "Wrote %s to file %s."
            "\n",
            bytes_to_string(wrote_bytes),
            OPTIONS->output_file
            );
  }
  stringbuffer_release(sb);
#ifdef DEBUG_MEMORY_ENABLED
  //print_allocated_memory();
#endif
  return(EXIT_SUCCESS);
} /* parse_colors_csv */

void ansi_truecolor_fg(FILE *file, int r, int g, int b) {
  fprintf(file, TRUECOLOR_FG_CODE, r, g, b);
}

void ansi_truecolor_bg(FILE *file, int r, int g, int b) {
  fprintf(file, TRUECOLOR_BG_CODE, r, g, b);
}

void ansi_reset(FILE *file) {
  fputs(RESET_CODE, file);
}

void ansi_truecolor_fg(FILE *file, int r, int g, int b);
void ansi_reset(FILE *file);
void ansi_truecolor_bg(FILE *file, int r, int g, int b);
