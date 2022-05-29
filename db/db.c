#include "db.h"

int db_list_ids(ColorsDB *DB){
    if(init_colors_db(DB) != 0)
        return(1);

    size_t total_ids = 0, unique_typeids = 0, typeid_qty = 0, type_ids_size = 0, type_ids_qty = 0;
    colordb_type TYPEID = 12345;

    colordb_count_typeid(DB->db, TYPEID, &typeid_qty);
    printf("type %llu has %lu items\n", TYPEID, typeid_qty);

    colordb_count_typeids(DB->db, &unique_typeids);
    printf("there are  %lu unique typeids\n", unique_typeids);

    colordb_count_ids(DB->db, &total_ids);
    printf("there are  %lu total ids\n", total_ids);

    char *type_ids = (char*)colordb_get_typeid_ids(DB->db, TYPEID, &type_ids_size, &type_ids_qty);
    printf("found %lu rows, %lu bytes of data for %llu ids list query\n", 
            type_ids_qty, 
            type_ids_size, 
            TYPEID
            );
    char *tmp = type_ids;
    for(size_t processed_items = 0;  processed_items < type_ids_qty; ){
        if(strlen(tmp)>0){
                printf("  item #%lu> %s (%lu bytes)\n", processed_items, tmp, strlen(tmp));
                tmp += strlen(tmp);
        }else{
            processed_items++;
            tmp++;
        }
    }
    


    return(0);
}

colordb_id db_get_typeid_id(ColorsDB *DB, colordb_type TYPEID){
    colordb_id id = 0;
    size_t len = 0;
    colordb_one(DB, TYPEID, &id, &len);
    if((len > 0) && (id > 0))
        return(id);
    return(-1);
}

bool db_typeid_exists(ColorsDB *DB, colordb_type TYPEID){
    return( db_get_typeid_id(DB,TYPEID) > 0);
}

colordb_id add_colors_db(ColorsDB *DB, colordb_type TYPEID, char *RECORD){
    colordb_id id = colordb_add(DB, TYPEID, (void*)RECORD, strlen(RECORD));
    return(id);
}

colordb_id add_colors_db_if_not_exist(ColorsDB *DB, colordb_type TYPEID, char *RECORD){
    colordb_id id = db_get_typeid_id(DB,TYPEID);
    if(id > 0){
        return(id);
    }
    return(add_colors_db(DB,TYPEID,RECORD));
}

int init_colors_db(ColorsDB *DB){
  DB->db = colordb_open(DB->Path);
  return((DB->db != NULL) ? 0 : 1);
}
