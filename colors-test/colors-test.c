#include <stdbool.h>
#define DEBUG_MEMORY_ENABLED      true
#define DEBUG_DB_PATH             false
#define DEBUG_LOAD_COLORS         false
#define DEBUG_COLOR_STRINGS       true
#define TEST_COLOR_STRINGS_QTY    5
#include "colors-test.h"
#include "submodules/dbg.h/dbg.h"
#include "submodules/log.h/log.h"
#ifdef DEBUG_MEMORY_ENABLED
#include "submodules/debug-memory/debug_memory.h"
#endif
//////////////////////////////////////////////
extern ColorsDB *DB;
//////////////////////////////////////////////


TEST t_colors_free(){
  free(DB->Path);
  free(DB);
  PASS();
}


TEST t_colors_init(){
  int  res = -1;
  char cwd[1024];

  getcwd(cwd, 256);
  DB       = malloc(sizeof(ColorsDB));
  DB->Path = malloc(strlen(COLOR_NAMES_DB_PATH) + 1024);
  sprintf(DB->Path, "%s", COLOR_NAMES_DB_PATH);
  long fsize = 0;

  res = fsio_file_exists(DB->Path);
  if (res != true) {
    sprintf(DB->Path, "%s/../%s", cwd, COLOR_NAMES_DB_PATH);
    res = fsio_file_exists(DB->Path);
    if (res != true) {
      sprintf(DB->Path, "%s/../../%s", cwd, COLOR_NAMES_DB_PATH);
      res = fsio_file_exists(DB->Path);
    }
  }
  if (DEBUG_DB_PATH) {
    dbg(DB->Path, %s);
  }
  fsize = fsio_file_size(DB->Path);
  if (DEBUG_DB_PATH) {
    dbg(fsize, %lu);
  }
  ASSERT_EQ(1, res);
  ASSERT_GTE(fsize, 1024 * 10);
  ASSERT_LTE(fsize, 1024 * 1024 * 10);

  res = init_colors_db(DB);
  if (DEBUG_DB_PATH) {
    dbg(res, %d);
  }
  ASSERT_EQ(0, res);

  PASS();
}

#define DEBUG_COLOR()    { do {                                            \
                             dbg(color_name, %s);                         \
                             dbg(color_hex, %s);                          \
                             dbg(color_hex[0], %c);                       \
                             dbg(color_row, %s);                          \
                             dbg(color_row[0], %c);                       \
                             dbg(color_row[strlen(color_row) - 1], %c);   \
                             dbg(color_ansicode, %lu);                    \
                             dbg(color_rgb.red, %d);                      \
                             dbg(color_rgb.green, %d);                    \
                             dbg(color_rgb.blue, %d);                     \
                             dbg(color_rgb_bg.red, %d);                   \
                             dbg(color_rgb_bg.green, %d);                 \
                             dbg(color_rgb_bg.blue, %d);                  \
                             dbg(color_decoded_png_length, %lu);          \
                             dbg((char *)color_encoded_png_content, %s);  \
                             dbg(strlen((char *)color_seq_ansi_fg), %lu); \
                             dbg(strlen((char *)color_seq_ansi_bg), %lu); \
                             dbg((char *)color_seq_ansi_fg, %s);          \
                             dbg((char *)color_seq_ansi_bg, %s);          \
                             dbg((char *)color_seq_truecolor_fg, %s);     \
                             dbg((char *)color_seq_truecolor_bg, %s);     \
                           } while (0); }

#define ASSERT_COLOR()                                  \
  { do {                                                \
      ASSERT_EQ(strlen(color_hex), 7);                  \
      ASSERT_EQ(color_hex[0], '#');                     \
      ASSERT_GTE(strlen(color_name), 3);                \
      ASSERT_GTE(strlen(color_row), 128);               \
      ASSERT_EQ(color_row[0], '{');                     \
      ASSERT_EQ(color_row[strlen(color_row) - 1], '}'); \
      ASSERT_GTE(color_rgb.red, 0);                     \
      ASSERT_LTE(color_rgb.red, 255);                   \
      ASSERT_GTE(color_rgb.green, 0);                   \
      ASSERT_LTE(color_rgb.green, 255);                 \
      ASSERT_GTE(color_rgb.blue, 0);                    \
      ASSERT_LTE(color_rgb.blue, 255);                  \
      ASSERT_GTE(color_rgb_bg.red, 0);                  \
      ASSERT_LTE(color_rgb_bg.red, 255);                \
      ASSERT_GTE(color_rgb_bg.green, 0);                \
      ASSERT_LTE(color_rgb_bg.green, 255);              \
      ASSERT_GTE(color_rgb_bg.blue, 0);                 \
      ASSERT_LTE(color_rgb_bg.blue, 255);               \
      ASSERT_GTE(color_ansicode, 0);                    \
      ASSERT_LTE(color_ansicode, 255);                  \
      ASSERT_GTE(color_decoded_png_length, 8);          \
      ASSERT_LTE(color_decoded_png_length, 1024 * 32);  \
    } while (0); }

#define GET_COLOR()                                                                                       \
  { do{                                                                                                   \
      color_row                 = get_color_name_row(color_name);                                         \
      color_hex                 = get_color_name_hex(color_name);                                         \
      color_rgb                 = get_color_name_rgb(color_name);                                         \
      color_rgb_bg              = get_color_name_rgb_background(color_name);                              \
      color_ansicode            = (size_t)get_color_name_row_property(color_name, "ansicode");            \
      color_decoded_png_length  = (size_t)get_color_name_row_property(color_name, "decoded_png_length");  \
      color_encoded_png_content = (char *)get_color_name_row_property(color_name, "encoded_png_content"); \
      color_seq_truecolor_fg    = (char *)get_color_name_row_property(color_name, "seq.truecolor.fg");    \
      color_seq_truecolor_bg    = (char *)get_color_name_row_property(color_name, "seq.truecolor.bg");    \
      color_seq_ansi_fg         = (char *)get_color_name_row_property(color_name, "seq.ansi.fg");         \
      color_seq_ansi_bg         = (char *)get_color_name_row_property(color_name, "seq.ansi.bg");         \
    } while (0); }

#define INIT_COLOR                                                                                     \
  char        *color_row, *color_hex, *color_seq_ansi_fg, *color_seq_ansi_bg, *color_seq_truecolor_fg, \
              *color_seq_truecolor_bg, *color_encoded_png_content;                                     \
  color_rgb_t color_rgb, color_rgb_bg;                                                                 \
  size_t      color_ansicode, color_decoded_png_length;


TEST t_color_hex_strings(){
  for (size_t i = 0; (i < TEST_COLOR_STRINGS_QTY) && (i < get_color_hex_strings().count); i++) {
    char *_color_hex = get_color_hex_strings().strings[i];
    char *color_name = get_color_hex_name(_color_hex);
    INIT_COLOR
    GET_COLOR();

    DEBUG_COLOR();
    ASSERT_STR_EQ(color_hex, _color_hex);
    ASSERT_COLOR();
  }
  PASS();
}


TEST t_color_name_strings(){
  for (size_t i = 0; (i < TEST_COLOR_STRINGS_QTY) && (i < get_color_name_strings().count); i++) {
    char *color_name = get_color_name_strings().strings[i];
    INIT_COLOR
    GET_COLOR();

    DEBUG_COLOR();
    ASSERT_COLOR();
  }
  PASS();
}


TEST t_colors_load(){
  int qty = load_colors(DB);

  dbg(DB->Path, %s);
  if (DEBUG_LOAD_COLORS) {
    printf("loaded db path %s\n", DB->Path);
    printf("loaded %d colors to hash\n", qty);
    printf("loaded %d color names\n", get_color_name_strings().count);
    printf("loaded %d color hexes\n", get_color_hex_strings().count);
  }

  ASSERT_GTE(qty, 10);
  ASSERT_EQ(get_color_name_strings().count, qty);
  ASSERT_EQ(get_color_hex_strings().count, qty);

  PASS();
}

SUITE(s_colors){
  RUN_TESTp(t_colors_init);
  RUN_TESTp(t_colors_load);
  RUN_TESTp(t_color_name_strings);
  RUN_TESTp(t_color_hex_strings);
  RUN_TESTp(t_colors_free);
  PASS();
}

GREATEST_MAIN_DEFS();


int main(int argc, char **argv) {
  GREATEST_MAIN_BEGIN();
  (void)argc; (void)argv;
  RUN_SUITE(s_colors);
#ifdef DEBUG_MEMORY_ENABLED
  print_allocated_memory();
#endif
  GREATEST_MAIN_END();
  return(0);
}

