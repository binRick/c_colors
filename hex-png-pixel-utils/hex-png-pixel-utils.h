#pragma once
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
/////////////////////////////////////////////////////////
#define STB_NO_LINEAR
#define STB_NO_HDR
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define DIR_MODE 0700
#define HEX_LEN 6
#define MAX_PATH_LEN 1024
/////////////////////////////////////////////////////////
#include "../submodules/tempdir.c/tempdir.h"
#include "../submodules/b64.c/b64.h"
#include "../submodules/c_fsio/include/fsio.h"
#include "../submodules/c_ansi/ansi-codes/ansi-codes.h"
#include "../submodules/bytes/bytes.h"
#include "../submodules/c_stringfn/include/stringfn.h"
//////////////////////////////////////////////////////////
typedef struct EncodedPngResult EncodedPngResult;
typedef struct HexPngWriterConfig HexPngWriterConfig;
////////////////////////////////////////////////////////////
struct EncodedPngResult { 
    char *EncodedContent;
    unsigned char *DecodedContent;
    size_t EncodedLength;
    size_t DecodedLength;
};
struct HexPngWriterConfig {
      char *COLOR;
      char *PATH;
      bool DEBUG;
};


int write_hex_png_to_path(HexPngWriterConfig *CONFIG);
EncodedPngResult hex_to_png_encoded_bytes(char *COLOR);


