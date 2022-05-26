#include "include/hex-png-pixel.h"
#include <stdio.h>
#include <string.h>


int main(int argc, char **argv) {
  if ((argc >= 2) && (strcmp(argv[1], "--test") == 0)) {
    hex_png_pixel("#ABFF55");
    hex_png_pixel("#5C22F5");
    hex_png_pixel("#AB8855");
    return(hex_png_pixel("#77FF55"));
  }
  hex_png_pixel("#ABFF55");
  hex_png_pixel("#5C22F5");
  hex_png_pixel("#AB8855");
  return(hex_png_pixel("#77FF55"));
}

