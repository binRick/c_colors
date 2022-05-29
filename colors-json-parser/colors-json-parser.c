#include "colors-json-parser.h"

#define COLORDB_MAX_HASH_BUCKETS    65536 * 128

unsigned long colordb_hash(char *key, int length){
  unsigned long i;
  unsigned long hash;

  hash = 5381;
  for ( i = 0; i < length; key++, i++ ) {
    hash = ((hash << 5) + hash) + (*key);
  }
  return(hash % COLORDB_MAX_HASH_BUCKETS);
}

#define ADD_ITEM_TO_HEX_DB()     { do { \
    unsigned long key = (unsigned long)colordb_hash(c->Hex, strlen(c->Hex));                       \
    if (OPTIONS->verbose_mode)                                                                     \
    printf("Hex>  adding '%s' (%lu bytes) to db with key '%lu' \n", c->Hex, strlen(c->JSON), key); \
    unsigned long inserted_id = add_colors_db_if_not_exist(OPTIONS->DB->db, key, (void *)c->JSON); \
    if (OPTIONS->verbose_mode)                                                                     \
    printf("Hex>  added ID #%lu\n", inserted_id);                                                  \
} while (0); }

#define ADD_ITEM_TO_NAME_DB()    { do {   \
    unsigned long key = (unsigned long)colordb_hash(c->Name, strlen(c->Name));                      \
    if (OPTIONS->verbose_mode)                                                                      \
    printf("Name> adding '%s' (%lu bytes) to db with key '%lu' \n", c->Name, strlen(c->JSON), key); \
    unsigned long inserted_id = add_colors_db_if_not_exist(OPTIONS->DB->db, key, (void *)c->JSON);  \
    if (OPTIONS->verbose_mode)                                                                      \
    printf("Name> added ID #%lu\n", inserted_id);                                                   \
} while (0); }

#define ADD_ITEM_TO_DBS()        { do {                     \
    ADD_ITEM_TO_NAME_DB(); \
    ADD_ITEM_TO_HEX_DB();  \
} while (0); }


#define ALLOC_ITEM()             { do {                                                      \
                                     c                 = malloc(sizeof(ParsedColor));        \
                                     c->RGB            = malloc(sizeof(ParsedRGB));          \
                                     c->Seq            = malloc(sizeof(ParsedSeq));          \
                                     c->Seq->Ansi      = malloc(sizeof(ParsedAnsiSeq));      \
                                     c->Seq->Truecolor = malloc(sizeof(ParsedTruecolorSeq)); \
                                   } while (0); }

#define ASSIGN_ITEM()            { do {                                                                                  \
                                     c->JSON               = Lines.strings[i];                                           \
                                     c->Props              = json_object_get_count(ColorObject);                         \
                                     c->Name               = json_object_get_string(ColorObject, "name");                \
                                     c->Ansicode           = json_object_get_number(ColorObject, "ansicode");            \
                                     c->Hex                = json_object_get_string(ColorObject, "hex");                 \
                                     c->RGB->red           = json_object_dotget_number(ColorObject, "rgb.red");          \
                                     c->RGB->green         = json_object_dotget_number(ColorObject, "rgb.green");        \
                                     c->RGB->blue          = json_object_dotget_number(ColorObject, "rgb.blue");         \
                                     c->Seq->Ansi->fg      = json_object_dotget_string(ColorObject, "seq.ansi.fg");      \
                                     c->Seq->Ansi->bg      = json_object_dotget_string(ColorObject, "seq.ansi.bg");      \
                                     c->Seq->Truecolor->fg = json_object_dotget_string(ColorObject, "seq.truecolor.fg"); \
                                     c->Seq->Truecolor->bg = json_object_dotget_string(ColorObject, "seq.truecolor.bg"); \
                                   } while (0); }

#define FREE_ITEM()              { do {                                              \
                                     if (c->Seq->Truecolor) free(c->Seq->Truecolor); \
                                     if (c->Seq->Ansi) free(c->Seq->Ansi);           \
                                     if (c->Seq) free(c->Seq);                       \
                                     if (c->RGB) free(c->RGB);                       \
                                     if (c) free(c);                                 \
                                   } while (0); }

#define DEBUG_ITEM()             { do {                                                                       \
                                     printf("line is object with %lu props\n", c->Props);                     \
                                     printf("\tname:%s|ansicode:%d|hex:%s|\n", c->Name, c->Ansicode, c->Hex); \
                                     printf("\trgb:%dx%dx%d\n",                                               \
                                            c->RGB->red                                                       \
                                            , c->RGB->green                                                   \
                                            , c->RGB->blue                                                    \
                                            );                                                                \
                                     printf("\ttruecolor seq fg: %lu bytes\n",                                \
                                            strlen(c->Seq->Truecolor->fg)                                     \
                                            );                                                                \
                                     printf("\tansi seq fg: %lu bytes\n",                                     \
                                            strlen(c->Seq->Ansi->fg)                                          \
                                            );                                                                \
                                   } while (0); }


int parse_colors_json(parse_json_options *OPTIONS){
  ct_start(NULL);
  djbhash_init(&(OPTIONS->ResultsHash));
  char                   *json_data = fs_read(OPTIONS->input_file);
  struct StringFNStrings Lines      = stringfn_split_lines_and_trim(json_data);

  fprintf(stderr, "JSON> parsing %lu colors from %s of %lu bytes and %d lines\n",
          OPTIONS->parse_qty, OPTIONS->input_file, strlen(json_data),
          Lines.count
          );
  JSON_Value  *ColorLine;
  JSON_Object *ColorObject;
  ParsedColor *c;

  for (int i = 0; i < Lines.count && (i < OPTIONS->parse_qty); i++) {
    ColorLine = json_parse_string(Lines.strings[i]);
    if (ColorLine == NULL) {
      continue;
    }
    ColorObject = json_value_get_object(ColorLine);
    ALLOC_ITEM();
    ASSIGN_ITEM();
    if (OPTIONS->verbose_mode) {
      DEBUG_ITEM();
    }
    ADD_ITEM_TO_DBS();
    FREE_ITEM();
  }
  OPTIONS->duration = ct_stop("");
  if (OPTIONS->verbose_mode) {
    fprintf(stderr, "Parsed %d items in %s\n",
            OPTIONS->ResultsHash.active_count, OPTIONS->duration
            );
  }
  return(EXIT_SUCCESS);
} /* parse_colors_json */


int free_parsed_results(parse_json_options *OPTIONS){
  djbhash_reset_iterator(&OPTIONS->ResultsHash);
  struct djbhash_node *item = djbhash_iterate(&OPTIONS->ResultsHash);

  while (item) {
    ParsedColor *c = (ParsedColor *)((item)->value);
    if (c) {
      FREE_ITEM();
    }
    item = djbhash_iterate(&OPTIONS->ResultsHash);
  }
  djbhash_destroy(&OPTIONS->ResultsHash);
  return(0);
}


int iterate_parsed_results(parse_json_options *OPTIONS){
  djbhash_reset_iterator(&OPTIONS->ResultsHash);
  struct djbhash_node *item = djbhash_iterate(&OPTIONS->ResultsHash);

  while (item) {
    ParsedColor *c = (ParsedColor *)((item)->value);
    if (c) {
      printf("   #item> Name:%s\n",
             c->Name
             );
    }
    item = djbhash_iterate(&OPTIONS->ResultsHash);
  }
  return(0);
}

