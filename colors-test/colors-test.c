#define DEBUG_MEMORY_ENABLED    true
#define DEBUG_DB_PATH           false
#define DEBUG_LOAD_COLORS       false
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


TEST t_colors_load(){
  int qty = -1;

  qty = load_colors(DB);

  if (DEBUG_LOAD_COLORS) {
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

