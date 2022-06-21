#define COLOR_NAMES_DB_PATH    "etc/colornames.bestof.sqlite"
#include <assert.h>
#include <fnmatch.h>
#include <libproc.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/sysctl.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>


#include "db/db.h"
#include "submodules/meson_deps/submodules/c_ansi/ansi-codes/ansi-codes.h"
#include "submodules/meson_deps/submodules/c_ansi/ansi-utils/ansi-utils.h"
#include "submodules/meson_deps/submodules/c_string_buffer/include/stringbuffer.h"
#include "submodules/meson_deps/submodules/c_stringfn/include/stringfn.h"
#include "submodules/meson_deps/submodules/djbhash/src/djbhash.h"
#include "submodules/meson_deps/submodules/parson/parson.h"

typedef struct {
  int red, green, blue;
} color_rgb_t;

int load_colors(ColorsDB *DB);
struct StringFNStrings get_color_name_strings();
struct StringFNStrings  get_color_hex_strings();
char * get_color_name_hex(const char *COLOR_NAME);
char * get_color_hex_name(const char *COLOR_HEX);
char * get_color_name_row(const char *COLOR_NAME);
void *get_color_name_row_property(const char *COLOR_NAME, const char *ROW_PROPERTY);
color_rgb_t get_color_name_rgb(const char *COLOR_NAME);
color_rgb_t get_color_name_rgb_background(const char *COLOR_NAME);
