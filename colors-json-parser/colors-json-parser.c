#pragma once
#define PROGRESS_BAR_WIDTH      40
#define BG_PROGRESS_BAR_CHAR    "."
#define PROGRESS_BAR_CHAR       "="
#define DEBUG_MEMORY_ENABLED
#ifdef DEBUG_MEMORY_ENABLED
#include "debug_memory.h"
#endif
#include "colors-json-parser/colors-json-parser.h"
#include "progress.h"
unsigned long colordb_hash(char *key, int length);
static void db_progress_start(progress_data_t *data);
static void db_progress(progress_data_t *data);
static void db_progress_end(progress_data_t *data);

////////////////////////////////////////////////////////////////////
#define OPTIMIZE_SQL_SYNC_USING_HASH_TABLE    true
#define VERBOSE_DEBUG_ITEMS                   true
#define VERBOSE_DEBUG_PROGRESS                false
#define DB_HEX_MODE_ENABLED                   false
#define DB_NAME_MODE_ENABLED                  true
////////////////////////////////////////////////////////////////////


static void db_progress_start(progress_data_t *data) {
  assert(data);
  fprintf(stdout,
          AC_HIDE_CURSOR
          AC_RESETALL AC_GREEN AC_ITALIC "Processing" AC_RESETALL
          AC_RESETALL " "
          AC_RESETALL AC_BLUE AC_REVERSED AC_BOLD "%d"
          AC_RESETALL " "
          AC_RESETALL AC_GREEN AC_ITALIC "JSON Lines" AC_RESETALL
          "\n",
          data->holder->total
          );
  progress_write(data->holder);
}


static void db_progress(progress_data_t *data) {
  progress_write(data->holder);
}


static void db_progress_end(progress_data_t *data) {
  fprintf(stdout,
          AC_SHOW_CURSOR
          AC_RESETALL "\n"
          AC_GREEN AC_REVERSED AC_BOLD "Complete" AC_RESETALL
          "\n"
          );
}


#define ADD_ITEM_TO_HEX_DB()     { do {                                                                                                           \
                                     unsigned long key      = (unsigned long)colordb_hash(c->Hex, strlen(c->Hex));                                \
                                     char          *key_str = malloc(1024);                                                                       \
                                     sprintf(key_str, "%lu", key);                                                                                \
                                     bool          exists_in_typeids_hash = ((TYPEIDS_HASH_ITEM = djbhash_find(&TYPEIDS_HASH, key_str)) != NULL); \
                                     fprintf(stderr, "Hex>  adding \n");                                                                          \
                                     free(key_str);                                                                                               \
                                     if (OPTIONS->verbose_mode) {                                                                                 \
                                       fprintf(stderr, "Hex>  adding '%s' (%lu bytes) to db with key '%lu'"                                       \
                                               "\n"                                                                                               \
                                               "\t| exists_in_typeids_hash: %s |"                                                                 \
                                               "\n"                                                                                               \
                                               , c->Hex, strlen(c->JSON), key                                                                     \
                                               , exists_in_typeids_hash ?"Yes":"No"                                                               \
                                               );                                                                                                 \
                                     }                                                                                                            \
                                     if (!OPTIMIZE_SQL_SYNC_USING_HASH_TABLE || !exists_in_typeids_hash) {                                        \
                                       unsigned long inserted_id = add_colors_db_if_not_exist(OPTIONS->DB->db                                     \
                                                                                              , key                                               \
                                                                                              , (void *)c->JSON                                   \
                                                                                              , (char *)c->Hex                                    \
                                                                                              , (char *)c->Name                                   \
                                                                                              );                                                  \
                                       if (OPTIONS->verbose_mode) {                                                                               \
                                         fprintf(stderr, "Hex>  added ID #%lu\n", inserted_id);                                                   \
                                       }                                                                                                          \
                                     }                                                                                                            \
                                   } while (0); }

#define ADD_ITEM_TO_NAME_DB()    { do {                                                                                                                              \
                                     unsigned long key = (unsigned long)colordb_hash(c->Name, strlen(c->Name));                                                      \
                                     if (OPTIONS->verbose_mode)                                                                                                      \
                                     fprintf(stderr, "Name> adding '%s' (%lu bytes) to db with key '%lu' \n", c->Name, strlen(c->JSON), key);                        \
                                     unsigned long inserted_id = add_colors_db_if_not_exist(OPTIONS->DB->db, key, (void *)c->JSON, (char *)c->Hex, (char *)c->Name); \
                                     if (OPTIONS->verbose_mode)                                                                                                      \
                                     fprintf(stderr, "Name> added ID #%lu\n", inserted_id);                                                                          \
                                   } while (0); }
#define FREE_ITEM()              { do {                                              \
                                     if (c->Seq->Truecolor) free(c->Seq->Truecolor); \
                                     if (c->Seq->Ansi) free(c->Seq->Ansi);           \
                                     if (c->Seq) free(c->Seq);                       \
                                     if (c->RGB) free(c->RGB);                       \
                                     if (c) free(c);                                 \
                                   } while (0); }


int parse_colors_json(parse_json_options *OPTIONS){
  ct_start(NULL);
  struct djbhash      TYPEIDS_HASH = db_get_typeids_hash(OPTIONS->DB);
  struct djbhash_node *TYPEIDS_HASH_ITEM;

  djbhash_reset_iterator(&TYPEIDS_HASH);
  fprintf(stdout,
          AC_BRIGHT_BLUE_BLACK AC_ITALIC  "TypeIDs Hash qty:"
          AC_RESETALL " "
          AC_RESETALL AC_GREEN AC_BOLD AC_REVERSED "%d" AC_RESETALL
          "\n",
          TYPEIDS_HASH.active_count
          );


  djbhash_init(&(OPTIONS->ResultsHash));
  char                   *json_data = fs_read(OPTIONS->input_file);
  struct StringFNStrings Lines      = stringfn_split_lines_and_trim(json_data);

  if (OPTIONS->verbose_mode) {
    fprintf(stderr, "JSON> parsing %lu colors from %s of %lu bytes and %d lines\n",
            OPTIONS->parse_qty, OPTIONS->input_file, strlen(json_data),
            Lines.count
            );
  }
  JSON_Value  *ColorLine;
  JSON_Object *ColorObject;
  ParsedColor *c;
  int         items_qty = (Lines.count < OPTIONS->parse_qty) ? Lines.count : OPTIONS->parse_qty;
  progress_t  *progress = progress_new(items_qty, PROGRESS_BAR_WIDTH);
  {
    progress->fmt         = "    Progress (:percent) => {:bar} [:elapsed]";
    progress->bg_bar_char = BG_PROGRESS_BAR_CHAR;
    progress->bar_char    = PROGRESS_BAR_CHAR;
    progress_on(progress, PROGRESS_EVENT_START, db_progress_start);
    progress_on(progress, PROGRESS_EVENT_PROGRESS, db_progress);
    progress_on(progress, PROGRESS_EVENT_END, db_progress_end);
  }
  for (int i = 0; i < Lines.count && (i < OPTIONS->parse_qty); i++) {
    {
      ColorLine = json_parse_string(Lines.strings[i]);
      if (ColorLine == NULL) {
        continue;
      }
      ColorObject = json_value_get_object(ColorLine);
    }
    {
      c                 = malloc(sizeof(ParsedColor));
      c->RGB            = malloc(sizeof(ParsedRGB));
      c->Seq            = malloc(sizeof(ParsedSeq));
      c->Seq->Ansi      = malloc(sizeof(ParsedAnsiSeq));
      c->Seq->Truecolor = malloc(sizeof(ParsedTruecolorSeq));
    }
    {
      c->JSON               = Lines.strings[i];
      c->Props              = json_object_get_count(ColorObject);
      c->Name               = json_object_get_string(ColorObject, "name");
      c->Ansicode           = json_object_get_number(ColorObject, "ansicode");
      c->Hex                = json_object_get_string(ColorObject, "hex");
      c->PngEncodedContent  = json_object_get_string(ColorObject, "encoded_png_content");
      c->PngDecodedLength   = json_object_get_number(ColorObject, "decoded_png_length");
      c->RGB->red           = json_object_dotget_number(ColorObject, "rgb.red");
      c->RGB->green         = json_object_dotget_number(ColorObject, "rgb.green");
      c->RGB->blue          = json_object_dotget_number(ColorObject, "rgb.blue");
      c->Seq->Ansi->fg      = json_object_dotget_string(ColorObject, "seq.ansi.fg");
      c->Seq->Ansi->bg      = json_object_dotget_string(ColorObject, "seq.ansi.bg");
      c->Seq->Truecolor->fg = json_object_dotget_string(ColorObject, "seq.truecolor.fg");
      c->Seq->Truecolor->bg = json_object_dotget_string(ColorObject, "seq.truecolor.bg");
    }
    if (OPTIONS->verbose_mode) {
      fprintf(stderr, "line is object with %lu props\n", c->Props);
      fprintf(stderr, "\tname:%s|ansicode:%d|hex:%s|\n", c->Name, c->Ansicode, c->Hex);
      fprintf(stderr, "\trgb:%dx%dx%d\n",
              c->RGB->red,
              c->RGB->green,
              c->RGB->blue
              );
      fprintf(stderr, "\ttruecolor seq fg: %lu bytes\n",
              strlen(c->Seq->Truecolor->fg)
              );
      fprintf(stderr, "\tansi seq fg: %lu bytes\n",
              strlen(c->Seq->Ansi->fg)
              );
      fprintf(stderr, "\tencoded png (%lu bytes decoded): %s\n",
              c->PngDecodedLength, c->PngEncodedContent
              );
    }
    {
      if (DB_NAME_MODE_ENABLED) {
        ADD_ITEM_TO_NAME_DB();
      }
      if (DB_HEX_MODE_ENABLED) {
        ADD_ITEM_TO_HEX_DB();
      }
    }
    {
      if (OPTIONS->ParsedColorHandler != NULL && c != NULL) {
        OPTIONS->ParsedColorHandler(c);
      }
    }
    {
      FREE_ITEM();
      progress_value(progress, i + 1);
    }
  }
  OPTIONS->duration = ct_stop("");
  progress_value(progress, items_qty);
  progress_free(progress);
  if (OPTIONS->verbose_mode) {
    fprintf(stderr, "Parsed %d items in %s\n", OPTIONS->ResultsHash.active_count, OPTIONS->duration);
  }
#ifdef DEBUG_MEMORY_ENABLED
  print_allocated_memory();
#endif
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
      fprintf(stderr, "   #item> Name:%s\n",
              c->Name
              );
    }
    item = djbhash_iterate(&OPTIONS->ResultsHash);
  }
  return(0);
}

