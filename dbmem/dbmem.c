#include "dbmem.h"
//#define DBMEM_DEBUG
#define DBMEM_MIN(x, y) ((x) < (y)?(x):(y))
#define DBMEM_MAX(x, y) ((x) > (y)?(x):(y))


static void dbmemDebug(const char *format, ...){
#if defined (DBMEM_DEBUG)
  char    logTemp[1024] = { 0 };

  va_list vaList;
  va_start(vaList, format);
  vsnprintf(logTemp, sizeof(logTemp), format, vaList);
  va_end(vaList);

  if (strchr(logTemp, '\n')) {
    printf("%s", logTemp);
  } else {
    printf("%s\n", logTemp);
  }
#endif
}

//===========================================================================

typedef struct spmemfile_t {
  sqlite3_file  base;
  char          *path;
  int           flags;
  spmembuffer_t *mem;
} spmemfile_t;

static int spmemfileClose(sqlite3_file *file);
static int spmemfileRead(sqlite3_file *file, void *buffer, int len, sqlite3_int64 offset);
static int spmemfileWrite(sqlite3_file *file, const void *buffer, int len, sqlite3_int64 offset);
static int spmemfileTruncate(sqlite3_file *file, sqlite3_int64 size);
static int spmemfileSync(sqlite3_file *file, int flags);
static int spmemfileFileSize(sqlite3_file *file, sqlite3_int64 *size);
static int spmemfileLock(sqlite3_file *file, int type);
static int spmemfileUnlock(sqlite3_file *file, int type);
static int spmemfileCheckReservedLock(sqlite3_file *file, int *result);
static int spmemfileFileControl(sqlite3_file *file, int op, void *arg);
static int spmemfileSectorSize(sqlite3_file *file);
static int spmemfileDeviceCharacteristics(sqlite3_file *file);

static sqlite3_io_methods g_spmemfile_io_memthods = {
  1,                                        /* iVersion */
  spmemfileClose,                           /* xClose */
  spmemfileRead,                            /* xRead */
  spmemfileWrite,                           /* xWrite */
  spmemfileTruncate,                        /* xTruncate */
  spmemfileSync,                            /* xSync */
  spmemfileFileSize,                        /* xFileSize */
  spmemfileLock,                            /* xLock */
  spmemfileUnlock,                          /* xUnlock */
  spmemfileCheckReservedLock,               /* xCheckReservedLock */
  spmemfileFileControl,                     /* xFileControl */
  spmemfileSectorSize,                      /* xSectorSize */
  spmemfileDeviceCharacteristics            /* xDeviceCharacteristics */
};


int spmemfileClose(sqlite3_file *file){
  spmemfile_t *memfile = (spmemfile_t *)file;

  dbmemDebug("call %s( %p )", __func__, memfile);

  if (SQLITE_OPEN_MAIN_DB & memfile->flags) {
    // noop
  } else {
    if (NULL != memfile->mem) {
      if (memfile->mem->data) {
        free(memfile->mem->data);
      }
      free(memfile->mem);
    }
  }

  free(memfile->path);

  return(SQLITE_OK);
}


int spmemfileRead(sqlite3_file *file, void *buffer, int len, sqlite3_int64 offset){
  spmemfile_t *memfile = (spmemfile_t *)file;

  dbmemDebug("call %s( %p, ..., %d, %lld ), len %d",
             __func__, memfile, len, offset, memfile->mem->used);

  if ((offset + len) > memfile->mem->used) {
    return(SQLITE_IOERR_SHORT_READ);
  }

  memcpy(buffer, memfile->mem->data + offset, len);

  return(SQLITE_OK);
}


int spmemfileWrite(sqlite3_file *file, const void *buffer, int len, sqlite3_int64 offset){
  spmemfile_t   *memfile = (spmemfile_t *)file;
  spmembuffer_t *mem     = memfile->mem;

  dbmemDebug("call %s( %p, ..., %d, %lld ), len %d",
             __func__, memfile, len, offset, mem->used);

  if ((offset + len) > mem->total) {
    int  newTotal   = 2 * (offset + len + mem->total);
    char *newBuffer = (char *)realloc(mem->data, newTotal);
    if (NULL == newBuffer) {
      return(SQLITE_NOMEM);
    }

    mem->total = newTotal;
    mem->data  = newBuffer;
  }

  memcpy(mem->data + offset, buffer, len);

  mem->used = DBMEM_MAX(mem->used, offset + len);

  return(SQLITE_OK);
}


int spmemfileTruncate(sqlite3_file *file, sqlite3_int64 size){
  spmemfile_t *memfile = (spmemfile_t *)file;

  dbmemDebug("call %s( %p )", __func__, memfile);

  memfile->mem->used = DBMEM_MIN(memfile->mem->used, size);

  return(SQLITE_OK);
}


int spmemfileSync(sqlite3_file *file, int flags){
  dbmemDebug("call %s( %p )", __func__, file);

  return(SQLITE_OK);
}


int spmemfileFileSize(sqlite3_file *file, sqlite3_int64 *size){
  spmemfile_t *memfile = (spmemfile_t *)file;

  dbmemDebug("call %s( %p )", __func__, memfile);

  *size = memfile->mem->used;

  return(SQLITE_OK);
}


int spmemfileLock(sqlite3_file *file, int type){
  dbmemDebug("call %s( %p )", __func__, file);

  return(SQLITE_OK);
}


int spmemfileUnlock(sqlite3_file *file, int type){
  dbmemDebug("call %s( %p )", __func__, file);

  return(SQLITE_OK);
}


int spmemfileCheckReservedLock(sqlite3_file *file, int *result){
  dbmemDebug("call %s( %p )", __func__, file);

  *result = 0;

  return(SQLITE_OK);
}


int spmemfileFileControl(sqlite3_file *file, int op, void *arg){
  dbmemDebug("call %s( %p )", __func__, file);

  return(SQLITE_OK);
}


int spmemfileSectorSize(sqlite3_file *file){
  dbmemDebug("call %s( %p )", __func__, file);

  return(0);
}


int spmemfileDeviceCharacteristics(sqlite3_file *file){
  dbmemDebug("call %s( %p )", __func__, file);

  return(0);
}

//===========================================================================

typedef struct dbmem_cb_t {
  void *arg;
  spmembuffer_t * ( *load )(void *args, const char *path);
} dbmem_cb_t;

typedef struct dbmem_t {
  sqlite3_vfs base;
  dbmem_cb_t  cb;
  sqlite3_vfs *parent;
} dbmem_t;

static int dbmemOpen(sqlite3_vfs *vfs, const char *path, sqlite3_file *file, int flags, int *outflags);
static int dbmemDelete(sqlite3_vfs *vfs, const char *path, int syncDir);
static int dbmemAccess(sqlite3_vfs *vfs, const char *path, int flags, int *result);
static int dbmemFullPathname(sqlite3_vfs *vfs, const char *path, int len, char *fullpath);
static void * dbmemDlOpen(sqlite3_vfs *vfs, const char *path);
static void dbmemDlError(sqlite3_vfs *vfs, int len, char *errmsg);

static void(*dbmemDlSym(sqlite3_vfs * vfs, void *handle, const char *symbol)) (void);
static void dbmemDlClose(sqlite3_vfs *vfs, void *handle);
static int dbmemRandomness(sqlite3_vfs *vfs, int len, char *buffer);
static int dbmemSleep(sqlite3_vfs *vfs, int microseconds);
static int dbmemCurrentTime(sqlite3_vfs *vfs, double *result);

static dbmem_t g_dbmem = {
  {
    1,                                                    /* iVersion */
    0,                                                    /* szOsFile */
    0,                                                    /* mxPathname */
    0,                                                    /* pNext */
    DBMEM_NAME,                                           /* zName */
    0,                                                    /* pAppData */
    dbmemOpen,                                            /* xOpen */
    dbmemDelete,                                          /* xDelete */
    dbmemAccess,                                          /* xAccess */
    dbmemFullPathname,                                    /* xFullPathname */
    dbmemDlOpen,                                          /* xDlOpen */
    dbmemDlError,                                         /* xDlError */
    dbmemDlSym,                                           /* xDlSym */
    dbmemDlClose,                                         /* xDlClose */
    dbmemRandomness,                                      /* xRandomness */
    dbmemSleep,                                           /* xSleep */
    dbmemCurrentTime                                      /* xCurrentTime */
  },
  { 0 },
  0                                                      /* pParent */
};


int dbmemOpen(sqlite3_vfs *vfs, const char *path, sqlite3_file *file, int flags, int *outflags){
  dbmem_t     *memvfs  = (dbmem_t *)vfs;
  spmemfile_t *memfile = (spmemfile_t *)file;

  dbmemDebug("call %s( %p(%p), %s, %p, %x, %p )\n",
             __func__, vfs, &g_dbmem, path, file, flags, outflags);

  memset(memfile, 0, sizeof(spmemfile_t));
  memfile->base.pMethods = &g_spmemfile_io_memthods;
  memfile->flags         = flags;

  memfile->path = strdup(path);

  if (SQLITE_OPEN_MAIN_DB & memfile->flags) {
    memfile->mem = memvfs->cb.load(memvfs->cb.arg, path);
  } else {
    memfile->mem = (spmembuffer_t *)calloc(sizeof(spmembuffer_t), 1);
  }

  return(memfile->mem ? SQLITE_OK : SQLITE_ERROR);
}


int dbmemDelete(sqlite3_vfs *vfs, const char *path, int syncDir){
  dbmemDebug("call %s( %p(%p), %s, %d )\n",
             __func__, vfs, &g_dbmem, path, syncDir);

  return(SQLITE_OK);
}


int dbmemAccess(sqlite3_vfs *vfs, const char *path, int flags, int *result){
  *result = 0;
  return(SQLITE_OK);
}


int dbmemFullPathname(sqlite3_vfs *vfs, const char *path, int len, char *fullpath){
  strncpy(fullpath, path, len);
  fullpath[len - 1] = '\0';

  return(SQLITE_OK);
}


void * dbmemDlOpen(sqlite3_vfs *vfs, const char *path){
  return(NULL);
}


void dbmemDlError(sqlite3_vfs *vfs, int len, char *errmsg){
  // noop
}

void(*dbmemDlSym(sqlite3_vfs * vfs, void *handle, const char *symbol)) (void){
  return(NULL);
}


void dbmemDlClose(sqlite3_vfs *vfs, void *handle){
  // noop
}


int dbmemRandomness(sqlite3_vfs *vfs, int len, char *buffer){
  return(SQLITE_OK);
}


int dbmemSleep(sqlite3_vfs *vfs, int microseconds){
  return(SQLITE_OK);
}


int dbmemCurrentTime(sqlite3_vfs *vfs, double *result){
  return(SQLITE_OK);
}


//===========================================================================


int dbmem_init(dbmem_cb_t *cb){
  sqlite3_vfs *parent = NULL;

  if (g_dbmem.parent) {
    return(SQLITE_OK);
  }

  parent = sqlite3_vfs_find(0);

  g_dbmem.parent = parent;

  g_dbmem.base.mxPathname = parent->mxPathname;
  g_dbmem.base.szOsFile   = sizeof(spmemfile_t);

  g_dbmem.cb = *cb;

  return(sqlite3_vfs_register((sqlite3_vfs *)&g_dbmem, 0));
}

//===========================================================================

typedef struct spmembuffer_link_t {
  char                      *path;
  spmembuffer_t             *mem;
  struct spmembuffer_link_t *next;
} spmembuffer_link_t;


spmembuffer_link_t * spmembuffer_link_remove(spmembuffer_link_t **head, const char *path){
  spmembuffer_link_t *ret = NULL;

  spmembuffer_link_t **iter = head;

  for ( ; NULL != *iter; ) {
    spmembuffer_link_t *curr = *iter;

    if (0 == strcmp(path, curr->path)) {
      ret   = curr;
      *iter = curr->next;
      break;
    } else {
      iter = &(curr->next);
    }
  }

  return(ret);
}


void spmembuffer_link_free(spmembuffer_link_t *iter){
  free(iter->path);
  free(iter->mem->data);
  free(iter->mem);
  free(iter);
}

//===========================================================================

typedef struct dbmem_env_t {
  spmembuffer_link_t *head;
  sqlite3_mutex      *mutex;
} dbmem_env_t;

static dbmem_env_t *g_dbmem_env = NULL;


static spmembuffer_t * load_cb(void *arg, const char *path){
  spmembuffer_t *ret = NULL;

  dbmem_env_t   *env = (dbmem_env_t *)arg;

  sqlite3_mutex_enter(env->mutex);
  {
    spmembuffer_link_t *toFind = spmembuffer_link_remove(&(env->head), path);

    if (NULL != toFind) {
      ret = toFind->mem;
      free(toFind->path);
      free(toFind);
    }
  }
  sqlite3_mutex_leave(env->mutex);

  return(ret);
}


int dbmem_env_init(){
  int ret = 0;

  if (NULL == g_dbmem_env) {
    dbmem_cb_t cb;

    g_dbmem_env        = (dbmem_env_t *)calloc(sizeof(dbmem_env_t), 1);
    g_dbmem_env->mutex = sqlite3_mutex_alloc(SQLITE_MUTEX_FAST);

    cb.arg  = g_dbmem_env;
    cb.load = load_cb;

    ret = dbmem_init(&cb);
  }

  return(ret);
}


void dbmem_env_fini(){
  if (NULL != g_dbmem_env) {
    spmembuffer_link_t *iter = NULL;

    sqlite3_vfs_unregister((sqlite3_vfs *)&g_dbmem);
    g_dbmem.parent = NULL;

    sqlite3_mutex_free(g_dbmem_env->mutex);

    iter = g_dbmem_env->head;
    for ( ; NULL != iter; ) {
      spmembuffer_link_t *next = iter->next;

      spmembuffer_link_free(iter);

      iter = next;
    }

    free(g_dbmem_env);
    g_dbmem_env = NULL;
  }
}


int dbmem_open_db(dbmem_db_t *db, const char *path, spmembuffer_t *mem){
  int                ret = 0;

  spmembuffer_link_t *iter = NULL;

  memset(db, 0, sizeof(dbmem_db_t));

  iter       = (spmembuffer_link_t *)calloc(sizeof(spmembuffer_link_t), 1);
  iter->path = strdup(path);
  iter->mem  = mem;

  sqlite3_mutex_enter(g_dbmem_env->mutex);
  {
    iter->next        = g_dbmem_env->head;
    g_dbmem_env->head = iter;
  }
  sqlite3_mutex_leave(g_dbmem_env->mutex);

  ret = sqlite3_open_v2(path, &(db->handle),
                        SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, DBMEM_NAME);

  if (0 == ret) {
    db->mem = mem;
  } else {
    sqlite3_mutex_enter(g_dbmem_env->mutex);
    {
      iter = spmembuffer_link_remove(&(g_dbmem_env->head), path);
      if (NULL != iter) {
        spmembuffer_link_free(iter);
      }
    }
    sqlite3_mutex_leave(g_dbmem_env->mutex);
  }

  return(ret);
}


int dbmem_close_db(dbmem_db_t *db){
  int ret = 0;

  if (NULL == db) {
    return(0);
  }

  if (NULL != db->handle) {
    ret        = sqlite3_close(db->handle);
    db->handle = NULL;
  }

  if (NULL != db->mem) {
    if (NULL != db->mem->data) {
      free(db->mem->data);
    }
    free(db->mem);
    db->mem = NULL;
  }

  return(ret);
}

