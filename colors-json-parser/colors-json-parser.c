#include "colors-json-parser.h"

#define ALLOC_ITEM(){ do { \
    c                 = malloc(sizeof(ParsedColor));        \
    c->RGB            = malloc(sizeof(ParsedRGB));          \
    c->Seq            = malloc(sizeof(ParsedSeq));          \
    c->Seq->Ansi      = malloc(sizeof(ParsedAnsiSeq));      \
    c->Seq->Truecolor = malloc(sizeof(ParsedTruecolorSeq));  \
} while(0); }

#define ASSIGN_ITEM(){ do { \
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
    c->Seq->Truecolor->bg = json_object_dotget_string(ColorObject, "seq.truecolor.bg");  \
} while(0); }

#define FREE_ITEM(){ do { \
    if(c->Seq->Truecolor) free(c->Seq->Truecolor); \
    if(c->Seq->Ansi)  free(c->Seq->Ansi);      \
    if(c->Seq)    free(c->Seq);            \
    if(c->RGB)  free(c->RGB);            \
    if(c) free(c);                 \
} while(0); }

#define DEBUG_ITEM(){ do { \
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
);                                                                 \
} while(0); }

int free_parsed_results(parse_json_options *OPTIONS){
  djbhash_reset_iterator(&OPTIONS->ResultsHash);
  struct djbhash_node *item = djbhash_iterate(&OPTIONS->ResultsHash);
  while (item) {
    ParsedColor *c = (ParsedColor *)((item)->value);
    if(c) FREE_ITEM();
    item = djbhash_iterate(&OPTIONS->ResultsHash);
  }
  return(0);
}

int iterate_parsed_results(parse_json_options *OPTIONS){
  djbhash_reset_iterator(&OPTIONS->ResultsHash);
  struct djbhash_node *item = djbhash_iterate(&OPTIONS->ResultsHash);
  while (item) {
    ParsedColor *c = (ParsedColor *)((item)->value);
    if(c){
        printf("   #item> Name:%s\n",
           c->Name
        );
    }
    item = djbhash_iterate(&OPTIONS->ResultsHash);
  }
  return(0);
}


int parse_colors_json(parse_json_options *OPTIONS){
  ct_start(NULL);
  djbhash_init(&(OPTIONS->ResultsHash));
  char        *json_data = fs_read(OPTIONS->input_file);
  struct StringFNStrings Lines = stringfn_split_lines_and_trim(json_data);

  fprintf(stderr, "JSON> parsing %lu colors from %s of %lu bytes and %d lines\n",
          OPTIONS->parse_qty, OPTIONS->input_file, strlen(json_data),
          Lines.count
          );
  JSON_Value  *ColorLine;
  JSON_Object *ColorObject;
  ParsedColor *c;
  for (int i = 0; i < Lines.count && (i < OPTIONS->parse_qty); i++) {
    ColorLine = json_parse_string(Lines.strings[i]);
    if (ColorLine == NULL) continue;
    ColorObject = json_value_get_object(ColorLine);
    ALLOC_ITEM();
    ASSIGN_ITEM();
    if (OPTIONS->verbose_mode) { DEBUG_ITEM(); }
    char *name = strdup(c->Name);
    djbhash_set(&(OPTIONS->ResultsHash), name, c, DJBHASH_OTHER);
    free(name);
  }
  OPTIONS->duration = ct_stop("");
  if (OPTIONS->verbose_mode) {
    fprintf(stderr, "Parsed %d items in %s\n",
            OPTIONS->ResultsHash.active_count, OPTIONS->duration
            );
  }
  return(EXIT_SUCCESS);
} /* parse_colors_json */

