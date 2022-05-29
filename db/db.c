#include "db.h"

boydemdb_id db_get_typeid_id(ColorsDB *DB, boydemdb_type TYPEID){
    boydemdb_id id = 0;
    size_t len = 0;
    boydemdb_one(DB, TYPEID, &id, &len);
    if((len > 0) && (id > 0))
        return(id);
    return(-1);
}

bool db_typeid_exists(ColorsDB *DB, boydemdb_type TYPEID){
    return( db_get_typeid_id(DB,TYPEID) > 0);
}

boydemdb_id add_colors_db(ColorsDB *DB, boydemdb_type TYPEID, char *RECORD){
    boydemdb_id id = boydemdb_add(DB, TYPEID, (void*)RECORD, strlen(RECORD));
    return(id);
}

boydemdb_id add_colors_db_if_not_exist(ColorsDB *DB, boydemdb_type TYPEID, char *RECORD){
    boydemdb_id id = db_get_typeid_id(DB,TYPEID);
    if(id > 0){
        return(id);
    }
    return(add_colors_db(DB,TYPEID,RECORD));
}

int init_colors_db(ColorsDB *DB){
  DB->db = boydemdb_open(DB->Path);
  return((DB->db != NULL) ? 0 : 1);
}
