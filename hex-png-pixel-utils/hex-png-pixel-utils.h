#pragma once
#include <stdbool.h>

typedef struct HexPngWriterConfig HexPngWriterConfig;
struct HexPngWriterConfig {
      char *COLOR;
      char *PATH;
      bool DEBUG;
};


int write_hex_png_to_path(HexPngWriterConfig *CONFIG);


