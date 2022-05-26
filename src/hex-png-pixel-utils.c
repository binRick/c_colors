#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
/////////////////////////////////////////////////////////
#define STB_NO_LINEAR
#define STB_NO_HDR
#define STB_IMAGE_WRITE_IMPLEMENTATION
/////////////////////////////////////////////////////////
#include "../vendor/stb_image_write.h"


/////////////////////////////////////////////////////////
char *str_substring(char *str, int start, int size){
  char *out = malloc(size);
  int  i    = 0;

  for (char *c = str + start; i < size; c++) {
    out[i] = *c;
    i++;
  }
  return(out);
}


int hex_png_pixel(char *COLOR){
  char *color    = malloc(6);
  char *filename = malloc(11);
  int  i         = 0;

  for (char *c = COLOR + 1; *c; c++) {
    color[i]    = *c;
    filename[i] = *c;
    i++;
  }
  strcat(filename, ".png");

  unsigned char *data = malloc(3);

  data[0] = strtol(str_substring(color, 0, 2), NULL, 16);
  data[1] = strtol(str_substring(color, 2, 2), NULL, 16);
  data[2] = strtol(str_substring(color, 4, 2), NULL, 16);

  stbi_write_png(filename, 1, 1, 3, data, 0);

  char cwd[1024];

  getcwd(cwd, sizeof(cwd));

  fprintf(stderr, "Color %s successfully saved to %s/%s\n", COLOR, cwd, filename);

  free(color);
  free(filename);
  free(data);

  return(0);
}
