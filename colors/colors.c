#include "colors.h"
#include "submodules/log.h/log.h"
#define MAX_COLORS      2000
#define DEBUG_COLORS    false
//////////////////////////////////////////////////////////////////////////
ColorsDB                      *DB;
static struct djbhash         COLORS_HASH = { 0 }, COLOR_NAME_HASH = { 0 }, COLOR_HEX_HASH = { 0 };
static struct StringFNStrings COLOR_NAME_STRINGS, COLOR_HEX_STRINGS;
static int load_color_names();
static void iterate_color_hex_strings();
static void iterate_color_name_strings();
static int load_colors_hash(ColorsDB *DB);
static void iterate_colors_hash();

//////////////////////////////////////////////////////////////////////////


static void iterate_color_hex_strings(){
  for (size_t i = 0; i < COLOR_HEX_STRINGS.count; i++) {
    char *color_name = get_color_hex_name(COLOR_HEX_STRINGS.strings[i]);
    char *color_row  = get_color_name_row(color_name);
    if (DEBUG_COLORS) {
      fprintf(stderr, "\t  - " AC_RESETALL AC_CYAN AC_ITALIC "#%lu> %s|%s- %s" AC_RESETALL "\n",
              i,
              COLOR_HEX_STRINGS.strings[i],
              color_name,
              color_row
              );
    }
  }
}


static void iterate_color_name_strings(){
  for (size_t i = 0; i < COLOR_NAME_STRINGS.count; i++) {
    char *color_hex = get_color_name_hex(COLOR_NAME_STRINGS.strings[i]);
    char *color_row = get_color_name_row(COLOR_NAME_STRINGS.strings[i]);
    if (DEBUG_COLORS) {
      fprintf(stderr, "\t  - " AC_RESETALL AC_CYAN AC_ITALIC "#%lu> %s|%s- %s" AC_RESETALL "\n",
              i,
              COLOR_NAME_STRINGS.strings[i],
              color_hex,
              color_row
              );
    }
  }
}


static void iterate_colors_hash(){
  for (size_t i = 0; i < COLOR_HEX_STRINGS.count; i++) {
    fprintf(stderr, "\t  - " AC_RESETALL AC_CYAN AC_ITALIC "#%lu> %s" AC_RESETALL "\n", i, COLOR_HEX_STRINGS.strings[i]);
  }
}


static int load_colors_hash(){
  struct djbhash_node *HASH_ITEM;

  djbhash_init(&COLORS_HASH);
  djbhash_init(&COLOR_NAME_HASH);
  djbhash_init(&COLOR_HEX_HASH);
  int    qty = 0;
  size_t total_ids = 0, unique_typeids_qty = 0, typeid_qty = 0, type_ids_size = 0, type_ids_qty = 0, unique_typeids_size = 0;
  char   *unique_typeids = (char *)colordb_get_distinct_typeids(DB->db, &unique_typeids_size, &unique_typeids_qty);

  fprintf(stderr, "read %lu bytes from %lu items\n", unique_typeids_size, unique_typeids_qty);
  size_t      row_type_ids_size = 0, row_type_ids_qty = 0, row_data_size = 0;
  JSON_Value  *ColorLine;
  JSON_Object *ColorObject;

  for (size_t processed_items = 0; (processed_items < unique_typeids_qty) && (qty < MAX_COLORS); ) {
    if ((unique_typeids != NULL) && strlen(unique_typeids) > 0) {
      colordb_type row_typeid = atoi(unique_typeids);
      row_data_size = 0;
      char         *id_type_ids = (char *)colordb_get_typeid_ids(DB->db, row_typeid, &row_type_ids_size, &row_type_ids_qty);
      for (size_t _processed_items = 0; (_processed_items < row_type_ids_qty) && (_processed_items < MAX_COLORS); ) {
        if (strlen(id_type_ids) > 0) {
          int row_id = atoi(id_type_ids);
          if (row_id >= 0) {
            size_t row_data_size = 0;
            char   *row_data     = colordb_get(DB->db, row_id, &row_data_size);
            ColorLine   = json_parse_string(row_data);
            ColorObject = json_value_get_object(ColorLine);
            const char *color_name = json_object_get_string(ColorObject, "name");
            const char *color_hex  = json_object_get_string(ColorObject, "hex");
            if ((HASH_ITEM = djbhash_find(&COLORS_HASH, (char *)color_name)) == NULL) {
              djbhash_set(&COLORS_HASH, (char *)color_name, (void *)row_data, DJBHASH_OTHER);
              djbhash_set(&COLOR_NAME_HASH, (char *)color_name, (char *)color_hex, DJBHASH_STRING);
              djbhash_set(&COLOR_HEX_HASH, (char *)color_hex, (char *)color_name, DJBHASH_STRING);
              qty++;
            }
          }
          id_type_ids += strlen(id_type_ids);
          _processed_items++;
        }else{
          id_type_ids++;
        }
      }
      unique_typeids += strlen(unique_typeids);
      processed_items++;
    }else{
      unique_typeids++;
    }
  }
  djbhash_reset_iterator(&COLORS_HASH);
  djbhash_reset_iterator(&COLOR_NAME_HASH);
  djbhash_reset_iterator(&COLOR_NAME_HASH);
  if (ColorLine) {
    json_value_free(ColorLine);
  }
  return(qty);
} /* load_colors_hash */


static int load_color_names(){
  struct StringBuffer *NAME_STRINGS = stringbuffer_new();
  struct StringBuffer *HEX_STRINGS  = stringbuffer_new();
  struct djbhash_node *item         = djbhash_iterate(&COLORS_HASH);
  JSON_Value          *ColorLine;
  JSON_Object         *ColorObject;
  size_t              qty = 0;

  while (item) {
    char *row_data = (char *)((item)->value);
    if (strlen(row_data) > 0) {
      if (DEBUG_COLORS) {
        fprintf(stderr, "\t  - " AC_RESETALL AC_GREEN AC_ITALIC "#%lu> %s" AC_RESETALL "\n", qty, row_data);
      }
      ColorLine   = json_parse_string(row_data);
      ColorObject = json_value_get_object(ColorLine);
      const char *color_name = json_object_get_string(ColorObject, "name");
      const char *hex_string = json_object_get_string(ColorObject, "hex");
      stringbuffer_append_string(NAME_STRINGS, stringfn_trim(color_name));
      stringbuffer_append_string(NAME_STRINGS, "\n");
      stringbuffer_append_string(HEX_STRINGS, stringfn_trim(hex_string));
      stringbuffer_append_string(HEX_STRINGS, "\n");
      item = djbhash_iterate(&COLORS_HASH);
      qty++;
    }
  }
  COLOR_NAME_STRINGS = stringfn_split_lines(stringfn_trim(stringbuffer_to_string(NAME_STRINGS)));
  COLOR_HEX_STRINGS  = stringfn_split_lines(stringfn_trim(stringbuffer_to_string(HEX_STRINGS)));
  stringbuffer_release(NAME_STRINGS);
  stringbuffer_release(HEX_STRINGS);
  if (ColorLine) {
    json_value_free(ColorLine);
  }
  return(qty);
}


struct StringFNStrings get_color_name_strings(){
  return(COLOR_NAME_STRINGS);
}

struct StringFNStrings  get_color_hex_strings(){
  return(COLOR_HEX_STRINGS);
}


int load_colors(ColorsDB *DB){
  int qty = 0, names_qty = 0;

  qty = load_colors_hash(DB);

  if (DEBUG_COLORS) {
    printf("loaded %d colors into hash\n", qty);
  }

  assert(qty > 10);

  names_qty = load_color_names();
  if (DEBUG_COLORS) {
    printf("loaded %d color names\n", names_qty);
  }
  assert(qty == names_qty);

  printf("loaded %d color names\n", COLOR_NAME_STRINGS.count);
  iterate_color_name_strings();
  assert(qty == COLOR_NAME_STRINGS.count);

  printf("loaded %d color hexes\n", COLOR_HEX_STRINGS.count);
  iterate_color_hex_strings();
  assert(qty == COLOR_HEX_STRINGS.count);

  return(qty);
}


char * get_color_hex_name(const char *COLOR_HEX){
  struct djbhash_node *HASH_ITEM;

  HASH_ITEM = djbhash_find(&COLOR_HEX_HASH, (char *)COLOR_HEX);
  if (HASH_ITEM == NULL) {
    return(NULL);
  }
  return((char *)((HASH_ITEM)->value));
}


char * get_color_name_hex(const char *COLOR_NAME){
  struct djbhash_node *HASH_ITEM;

  HASH_ITEM = djbhash_find(&COLOR_NAME_HASH, (char *)COLOR_NAME);
  if (HASH_ITEM == NULL) {
    return(NULL);
  }
  return((char *)((HASH_ITEM)->value));
}


void * get_color_name_row_property(const char *COLOR_NAME, const char *ROW_PROPERTY){
  void       *res       = NULL;
  char       *COLOR_ROW = get_color_name_row(COLOR_NAME);
  JSON_Value *V;

  if (DEBUG_COLORS) {
    log_debug("%s", COLOR_NAME);
    log_debug("%s", ROW_PROPERTY);
    log_debug("%s", COLOR_ROW);
  }
  assert(COLOR_ROW != NULL);
  assert(strlen(COLOR_ROW) > 5);

  JSON_Value *ROW = json_parse_string(COLOR_ROW);

  assert(ROW != NULL);
  assert(json_value_get_type(ROW) == JSONObject);

  JSON_Object *O = json_value_get_object(ROW);

  assert(O != NULL);

  V = json_object_dotget_value(O, ROW_PROPERTY);
  assert(V != NULL);
  log_debug("prop:%s", ROW_PROPERTY);
  log_debug("val type:%d", json_value_get_type(V));

  assert(json_value_get_type(V) != JSONError);
  assert(json_value_get_type(V) != JSONNull);

  switch (json_value_get_type(V)) {
  case JSONString:
    res = (void *)strdup_escaped(((char *)json_value_get_string(V)));
    break;
  case JSONBoolean:
    res = (void *)((size_t)json_value_get_boolean(V));
    break;
  case JSONNumber:
    res = (void *)((size_t)json_value_get_number(V));
    break;
  }
  if (ROW) {
    json_value_free(ROW);
  }
  return(res);
} /* get_color_name_row_property */


char * get_color_name_row(const char *COLOR_NAME){
  struct djbhash_node *HASH_ITEM;

  HASH_ITEM = djbhash_find(&COLORS_HASH, (char *)COLOR_NAME);
  if (HASH_ITEM == NULL) {
    return(NULL);
  }
  return((char *)((HASH_ITEM)->value));
}


color_rgb_t get_color_name_rgb_background(const char *COLOR_NAME){
  color_rgb_t color_rgb = { 0, 0, 0 };

  color_rgb = get_color_name_rgb(COLOR_NAME);
  return(color_rgb);
}


color_rgb_t get_color_name_rgb(const char *COLOR_NAME){
  struct djbhash_node *HASH_ITEM;
  color_rgb_t         color_rgb  = { 0, 0, 0 };
  char                *color_row = get_color_name_row(COLOR_NAME);

  if (color_row == NULL) {
    return(color_rgb);
  }
  JSON_Value  *ColorLine;
  JSON_Object *ColorObject;

  ColorLine       = json_parse_string(color_row);
  ColorObject     = json_value_get_object(ColorLine);
  color_rgb.red   = json_object_dotget_number(ColorObject, "rgb.red");
  color_rgb.green = json_object_dotget_number(ColorObject, "rgb.green");
  color_rgb.blue  = json_object_dotget_number(ColorObject, "rgb.blue");
  if (ColorLine) {
    json_value_free(ColorLine);
  }
  return(color_rgb);
}

