//#define DEBUG_MEMORY_ENABLED
#ifdef DEBUG_MEMORY_ENABLED
//#include "debug-memory/debug_memory.h"
#endif
#include "hex-png-pixel-utils.h"
#include "stb_image_write.h"
#include "dbg.h/dbg.h"
//////////////////////////////////////////////////////////
static const char *str_substring(char *str, int start, int size);
//////////////////////////////////////////////////////////
EncodedPngResult hex_to_png_encoded_bytes(char *COLOR){
  EncodedPngResult _ENC_RES = { .EncodedLength = 0, .DecodedLength = 0, .EncodedContent = NULL, .DecodedContent = NULL };
  if(COLOR[0] == '#'){
      char *NC = malloc(HEX_LEN+1);
      NC = str_substring(COLOR,1,HEX_LEN);
      NC[HEX_LEN] = '\0';
      COLOR = NC;
  }
  if(!COLOR || strlen(COLOR) != HEX_LEN) return(_ENC_RES);
  char *td = gettempdir();
  size_t wrote_bytes; 
  assert(td);
  char *write_file = malloc(MAX_PATH_LEN);
  sprintf(write_file, "%s/.tmp.png", td);
  char *upper_color = malloc(HEX_LEN); 
  unsigned char *data = malloc(3); 
  upper_color = stringfn_to_uppercase(COLOR); 
  data[0] = strtol(str_substring(upper_color, 0, 2), NULL, 16); 
  data[1] = strtol(str_substring(upper_color, 2, 2), NULL, 16); 
  data[2] = strtol(str_substring(upper_color, 4, 2), NULL, 16); 
  stbi_write_png(write_file, 1, 1, 3, data, 0); 
  wrote_bytes = fsio_file_size(write_file); 
  unsigned char* PNG_DATA = (unsigned char*)fsio_read_binary_file(write_file);
  free(data); 
  free(upper_color); 
  free(td);
  unlink(write_file);
  free(write_file);
  if(strlen(PNG_DATA) < 1) return(_ENC_RES);
  if(wrote_bytes < 1) return(_ENC_RES);
  char *ENCODED_PNG_DATA = b64_encode(PNG_DATA, wrote_bytes);
  EncodedPngResult ENC_RES = {
    .DecodedContent = PNG_DATA,
    .DecodedLength = wrote_bytes,
    .EncodedLength = strlen(ENCODED_PNG_DATA),
    .EncodedContent = ENCODED_PNG_DATA,
  };

#ifdef DEBUG_MEMORY_ENABLED
    //print_allocated_memory();
#endif    

  return(ENC_RES);
}

int write_hex_png_to_path(HexPngWriterConfig *CONFIG){
  if(strlen(CONFIG->PATH) < 1)
      return(9);
  if(strlen(CONFIG->COLOR) != HEX_LEN) 
      return(10);
  if(!fsio_path_exists(CONFIG->PATH))
      if(!fsio_mkdirs(CONFIG->PATH, DIR_MODE)) 
          return(11);

  EncodedPngResult ENC = hex_to_png_encoded_bytes(CONFIG->COLOR);
  char *write_file = malloc(MAX_PATH_LEN), *cwd = malloc(MAX_PATH_LEN - 128);
  char *upper_color = malloc(HEX_LEN);
  upper_color = stringfn_to_uppercase(CONFIG->COLOR);
  getcwd(cwd, MAX_PATH_LEN);
  sprintf(write_file, "%s/%s/%s.png", cwd, CONFIG->PATH, upper_color);
    if(!fsio_write_binary_file(write_file, ENC.DecodedContent, ENC.DecodedLength))
        return(15);
  size_t wrote_bytes = fsio_file_size(write_file);
  if(wrote_bytes < 1) return(1);

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


