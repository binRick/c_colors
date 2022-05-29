#include "color-db.h"
#define TYPE1    1
#define TYPE2    2

args_t  args = {
  DEFAULT_DB_FILE,
  DEFAULT_MODE,
  DEFAULT_VERBOSE,
  DEFAULT_COUNT,
};

colordb db;


int init_db(char *DB){
  db = colordb_open(DB);
  if (!db) {
    return(EXIT_FAILURE);
  }
  return(EXIT_SUCCESS);
}


int read_db(){
  return(EXIT_SUCCESS);
}


int list_db(char *DB){
  char *str = NULL;
  char *fmt = "this is a %s";
  int  size = asprintf(&str, fmt, "string");

  printf("%s\n", str);
  printf("%d\n", size);


  printf("listing db.........\n");
  init_db(DB);
  char       *msg = "mydat123xxxxxxxx";
  colordb_id id   = colordb_add(db, TYPE1, (void *)msg, strlen(msg));

  printf("added %llu\n", id);

  size_t     len0;
  colordb_id found_id0;
  void       *item0 = colordb_get(db, id, &len0);

  printf("got>  size:%lu\n", len0);
  printf("got>  '%s'\n", (char *)item0);

  size_t     len;
  colordb_id found_id;
  void       *item = colordb_one(db, TYPE1, &found_id, &len);

  printf("got>  size:%lu\n", len);
  printf("got>  '%s'\n", (char *)item);

  size_t     len1;
  colordb_id found_id1;
  void       *item1;

  colordb_delete(db, found_id1);

  item1 = colordb_one(db, TYPE2, &found_id1, &len1);
  printf("(after del) 2: got>  size:%lu\n", len1);
  printf("(after del) 2: got>  #%llu> '%s'\n", found_id1, (char *)item1);

  colordb_close(db);

  return(EXIT_SUCCESS);
} /* list_db */


int debug_args(){
  fprintf(stderr,
          acs(AC_BRIGHT_BLUE_BLACK AC_ITALIC  "Verbose: %d")
          ", "
          ansistr(AC_RESETALL AC_REVERSE AC_BRIGHT_RED_BLACK "DB File: %s")
          ", "
          ansistr(AC_RESETALL AC_BRIGHT_GREEN_BLACK "Mode: %s")
          ", "
          ansistr(AC_RESETALL AC_UNDERLINE "Count: %d")
          "\n",
          args.verbose, args.db_file, args.mode, args.count
          );

  return(EXIT_SUCCESS);
}


int parse_args(int argc, char *argv[]){
  char               identifier;
  const char         *value;
  cag_option_context context;

  cag_option_prepare(&context, options, CAG_ARRAY_SIZE(options), argc, argv);
  while (cag_option_fetch(&context)) {
    identifier = cag_option_get(&context);

    switch (identifier) {
    case 'd':
      value        = cag_option_get_value(&context);
      args.db_file = value;
      break;
    case 'm':
      value     = cag_option_get_value(&context);
      args.mode = value;
      break;
    case 'v':
      args.verbose = true;
      break;
    case 'c':
      value      = cag_option_get_value(&context);
      args.count = atoi(value);
      break;
    case 'h':
      fprintf(stderr, AC_RESETALL AC_YELLOW AC_BOLD "Usage: color-db [OPTION]\n" AC_RESETALL);
      cag_option_print(options, CAG_ARRAY_SIZE(options), stdout);
      exit(EXIT_SUCCESS);
    }
  }
  return(EXIT_SUCCESS);
}


int main(int argc, char **argv) {
  parse_args(argc, argv);
  if ((argc >= 2) && (strcmp(argv[1], "--test") == 0)) {
    printf("Test OK\n"); return(0);
  }


  if ((strcmp(args.mode, "debug_args") == 0)) {
    return(debug_args());
  }

  if ((strcmp(args.mode, "list") == 0)) {
    return(list_db(args.db_file));
  }

  printf(AC_RESETALL AC_RED "No mode selected: %s\n" AC_RESETALL, args.mode);
  return(1);
}

