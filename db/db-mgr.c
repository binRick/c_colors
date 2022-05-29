#include "db-mgr.h"

#define SELECT_QUERY          "SELECT data FROM blobs WHERE id = ?"
#define ONE_QUERY             "SELECT id, data FROM blobs WHERE type = ? LIMIT 1"
#define INSERT_QUERY          "INSERT INTO blobs(type, data) VALUES (?, ?)"
#define DELETE_QUERY          "DELETE FROM blobs WHERE id = ?"
#define CREATE_TABLE_QUERY    "CREATE TABLE IF NOT EXISTS blobs (id INTEGER PRIMARY KEY, type INTEGER, data BLOB)"


colordb_id colordb_add(colordb            db,
                       const colordb_type type,
                       void               *blob,
                       size_t             size){
  sqlite3_int64 id = -1;

  if (size > INT_MAX) {
    return(id);
  }

  if (sqlite3_bind_int64(db->insert, 1, type) != SQLITE_OK) {
    return(id);
  }

  if (sqlite3_bind_blob(db->insert,
                        2,
                        blob,
                        (int)size,
                        SQLITE_STATIC) != SQLITE_OK) {
    goto reset;
  }

  if (sqlite3_step(db->insert) != SQLITE_DONE) {
    goto reset;
  }

  id = sqlite3_last_insert_rowid(db->db);

reset:
  sqlite3_clear_bindings(db->insert);
  sqlite3_reset(db->insert);

  return(id);
}


void *colordb_get(colordb db, colordb_id id, size_t *size){
  const void    *p;
  unsigned char *copy = NULL;

  if (sqlite3_bind_int64(db->select, 1, id) != SQLITE_OK) {
    return(copy);
  }

  if (sqlite3_step(db->select) != SQLITE_ROW) {
    goto reset;
  }

  p     = sqlite3_column_blob(db->select, 0);
  *size = (size_t)sqlite3_column_bytes(db->select, 0);

  copy = malloc(*size + 1);
  if (!copy) {
    goto reset;
  }

  memcpy(copy, p, *size);
  copy[*size] = '\0';

reset:
  sqlite3_clear_bindings(db->select);
  sqlite3_reset(db->select);

  return(copy);
}


void *colordb_one(colordb            db,
                  const colordb_type type,
                  colordb_id         *id,
                  size_t             *size){
  const void    *p;
  unsigned char *copy = NULL;

  if (sqlite3_bind_int(db->one, 1, type) != SQLITE_OK) {
    return(copy);
  }

  if (sqlite3_step(db->one) != SQLITE_ROW) {
    goto reset;
  }

  *id = sqlite3_column_int64(db->one, 0);

  p     = sqlite3_column_blob(db->one, 1);
  *size = (size_t)sqlite3_column_bytes(db->one, 1);

  copy = malloc(*size + 1);
  if (!copy) {
    goto reset;
  }

  memcpy(copy, p, *size);
  copy[*size] = '\0';

reset:
  sqlite3_clear_bindings(db->one);
  sqlite3_reset(db->one);

  return(copy);
}


void colordb_delete(colordb db, colordb_id id){
  if (sqlite3_bind_int64(db->delete, 1, id) != SQLITE_OK) {
    return;
  }

  sqlite3_step(db->delete);

  sqlite3_clear_bindings(db->delete);
  sqlite3_reset(db->delete);
}


static int try_again(void *arg, int times){
  return(1);
}


colordb colordb_open(const char *path){
  colordb db;

  db = malloc(sizeof(*db));
  if (!db) {
    return(NULL);
  }

  if (sqlite3_open_v2(path,
                      &db->db,
                      SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE,
                      NULL) != SQLITE_OK) {
    free(db);
    return(NULL);
  }

  if (sqlite3_busy_handler(db->db, try_again, NULL) != SQLITE_OK) {
    sqlite3_close(db->db);
    free(db);
  }

  if (sqlite3_exec(db->db,
                   CREATE_TABLE_QUERY,
                   NULL,
                   NULL,
                   NULL) != SQLITE_OK) {
    sqlite3_close(db->db);
    free(db);
    return(NULL);
  }

  if (sqlite3_prepare(db->db,
                      INSERT_QUERY,
                      -1,
                      &db->insert,
                      NULL) != SQLITE_OK) {
    sqlite3_close(db->db);
    free(db);
    return(NULL);
  }

  if (sqlite3_prepare(db->db,
                      SELECT_QUERY,
                      -1,
                      &db->select,
                      NULL) != SQLITE_OK) {
    sqlite3_finalize(db->insert);
    sqlite3_close(db->db);
    free(db);
    return(NULL);
  }

  if (sqlite3_prepare(db->db,
                      ONE_QUERY,
                      -1,
                      &db->one,
                      NULL) != SQLITE_OK) {
    sqlite3_finalize(db->select);
    sqlite3_finalize(db->insert);
    sqlite3_close(db->db);
    free(db);
    return(NULL);
  }

  if (sqlite3_prepare(db->db,
                      DELETE_QUERY,
                      -1,
                      &db->delete,
                      NULL) != SQLITE_OK) {
    sqlite3_finalize(db->one);
    sqlite3_finalize(db->select);
    sqlite3_finalize(db->insert);
    sqlite3_close(db->db);
    free(db);
    return(NULL);
  }

  return(db);
} /* colordb_open */


void colordb_close(colordb db){
  if (!db) {
    return;
  }

  sqlite3_finalize(db->delete);
  sqlite3_finalize(db->one);
  sqlite3_finalize(db->select);
  sqlite3_finalize(db->insert);

  sqlite3_close(db->db);

  free(db);
}
