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

