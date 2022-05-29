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
#include "hex-png-pixel-utils.h"
#include "stb_image_write.h"
//#include "../submodules/dbg.h/dbg.h"
#include "../submodules/c_fsio/include/fsio.h"
#include "../submodules/c_ansi/ansi-codes/ansi-codes.h"
#include "../submodules/bytes/bytes.h"
#include "../submodules/c_stringfn/include/stringfn.h"
//////////////////////////////////////////////////////////
static const char *str_substring(char *str, int start, int size);
//////////////////////////////////////////////////////////

int write_hex_png_to_path(HexPngWriterConfig *CONFIG){
  if(strlen(CONFIG->PATH) < 1)
      return(9);
  if(strlen(CONFIG->COLOR) != HEX_LEN) 
      return(10);
  if(!fsio_path_exists(CONFIG->PATH))
      if(!fsio_mkdirs(CONFIG->PATH, DIR_MODE)) 
          return(11);

  char *write_file = malloc(MAX_PATH_LEN), *cwd = malloc(MAX_PATH_LEN - 128);
  char *upper_color = malloc(HEX_LEN);
  unsigned char *data = malloc(3);
  upper_color = stringfn_to_uppercase(CONFIG->COLOR);

  getcwd(cwd, MAX_PATH_LEN);
  sprintf(write_file, "%s/%s/%s.png", cwd, CONFIG->PATH, upper_color);
  data[0] = strtol(str_substring(upper_color, 0, 2), NULL, 16);
  data[1] = strtol(str_substring(upper_color, 2, 2), NULL, 16);
  data[2] = strtol(str_substring(upper_color, 4, 2), NULL, 16);

  stbi_write_png(write_file, 1, 1, 3, data, 0);
  size_t wrote_bytes = fsio_file_size(write_file);

  fprintf(stdout, "Color" " " AC_BOLD AC_REVERSED AC_BLUE_BLACK "%s" AC_RESETALL " " "successfully saved to " AC_YELLOW AC_ITALIC "%s" AC_RESETALL 
         " "
         "("
         AC_GREEN AC_REVERSED AC_BOLD "%s" AC_RESETALL
         ")"
         "\n"
          , upper_color, write_file, bytes_to_string(wrote_bytes)
          );


  free(upper_color);
  free(write_file);
  free(cwd);
  free(data);
  return(0);
}


static const char *str_substring(char *str, int start, int size){
  char *out = malloc(size);
  int  i    = 0;
  for (char *c = str + start; i < size; c++) {
    out[i] = *c;
    i++;
  }
  return(out);
}


