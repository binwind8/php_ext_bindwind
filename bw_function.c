/*
  +----------------------------------------------------------------------+
  | PHP Version 5                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2015 The PHP Group                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author:baboosh                                                       |
  +----------------------------------------------------------------------+
*/
#include "php_binwin.h"
/** {{{ zval * bw_request_query(uint type, char * name,uint len TSRMLS_DC)
*/
zval * bw_request_query(uint type, char * name,uint len TSRMLS_DC) { 

zval    **carrier = NULL, **ret;
#if (PHP_MAJOR_VERSION == 5) && (PHP_MINOR_VERSION < 4)
  zend_bool   jit_initialization = (PG(auto_globals_jit) && !PG(register_globals) && !PG(register_long_arrays));
#else
  zend_bool   jit_initialization = PG(auto_globals_jit);
#endif

  /* for phpunit test requirements */
#if PHP_BINWIN_DEBUG
  switch (type) {
    case TRACK_VARS_POST:
      (void)zend_hash_find(&EG(symbol_table), ZEND_STRS("_POST"), (void **)&carrier);
      break;
    case TRACK_VARS_GET:
      (void)zend_hash_find(&EG(symbol_table), ZEND_STRS("_GET"), (void **)&carrier);
      break;
    case TRACK_VARS_COOKIE:
      (void)zend_hash_find(&EG(symbol_table), ZEND_STRS("_COOKIE"), (void **)&carrier);
      break;
    case TRACK_VARS_SERVER:
      if (jit_initialization) {
        zend_is_auto_global(ZEND_STRL("_SERVER") TSRMLS_CC);
      }
      (void)zend_hash_find(&EG(symbol_table), ZEND_STRS("_SERVER"), (void **)&carrier);
      break;
    case TRACK_VARS_ENV:
      if (jit_initialization) {
        zend_is_auto_global(ZEND_STRL("_ENV") TSRMLS_CC);
      }
      carrier = &PG(http_globals)[TRACK_VARS_ENV];
      break;
    case TRACK_VARS_FILES:
      carrier = &PG(http_globals)[TRACK_VARS_FILES];
      break;
    case TRACK_VARS_REQUEST:
      if (jit_initialization) {
        zend_is_auto_global(ZEND_STRL("_REQUEST") TSRMLS_CC);
      }
      (void)zend_hash_find(&EG(symbol_table), ZEND_STRS("_REQUEST"), (void **)&carrier);
      break;
    default:
      break;
  }
#else
  switch (type) {
    case TRACK_VARS_POST:
    case TRACK_VARS_GET:
    case TRACK_VARS_FILES:
    case TRACK_VARS_COOKIE:
      carrier = &PG(http_globals)[type];
      break;
    case TRACK_VARS_ENV:
      if (jit_initialization) {
        zend_is_auto_global(ZEND_STRL("_ENV") TSRMLS_CC);
      }
      carrier = &PG(http_globals)[type];
      break;
    case TRACK_VARS_SERVER:
      if (jit_initialization) {
        zend_is_auto_global(ZEND_STRL("_SERVER") TSRMLS_CC);
      }
      carrier = &PG(http_globals)[type];
      break;
    case TRACK_VARS_REQUEST:
      if (jit_initialization) {
        zend_is_auto_global(ZEND_STRL("_REQUEST") TSRMLS_CC);
      }
      (void)zend_hash_find(&EG(symbol_table), ZEND_STRS("_REQUEST"), (void **)&carrier);
      break;
    default:
      break;
  }
#endif

  if (!carrier || !(*carrier)) {
    zval *empty;
    MAKE_STD_ZVAL(empty);
    ZVAL_NULL(empty);
    return empty;
  }

  if (!len) {
    Z_ADDREF_P(*carrier);
    return *carrier;
  }

  if (zend_hash_find(Z_ARRVAL_PP(carrier), name, len + 1, (void **)&ret) == FAILURE) {
    zval *empty;
    MAKE_STD_ZVAL(empty);
    ZVAL_NULL(empty);
    return empty;
  }

  Z_ADDREF_P(*ret);
  return *ret;
}  

/** {{{ int loader_import(char *path, int len, int use_path TSRMLS_DC)
*/
int bw_loader_import(char *path, int len, int use_path TSRMLS_DC) {
  zend_file_handle file_handle;
  zend_op_array   *op_array;
  char realpath[MAXPATHLEN];

  if (!VCWD_REALPATH(path, realpath)) {
    return 0;
  }

  file_handle.filename = path;
  file_handle.free_filename = 0;
  file_handle.type = ZEND_HANDLE_FILENAME;
  file_handle.opened_path = NULL;
  file_handle.handle.fp = NULL;

  op_array = zend_compile_file(&file_handle, ZEND_INCLUDE TSRMLS_CC);

  if (op_array && file_handle.handle.stream.handle) {
    int dummy = 1;

    if (!file_handle.opened_path) {
      file_handle.opened_path = path;
    }

    zend_hash_add(&EG(included_files), file_handle.opened_path, strlen(file_handle.opened_path)+1, (void *)&dummy, sizeof(int), NULL);
  }
  zend_destroy_file_handle(&file_handle TSRMLS_CC);

  if (op_array) {
    zval *result = NULL;

    BW_STORE_EG_ENVIRON();

    EG(return_value_ptr_ptr) = &result;
    EG(active_op_array)    = op_array;

#if ((PHP_MAJOR_VERSION == 5) && (PHP_MINOR_VERSION > 2)) || (PHP_MAJOR_VERSION > 5)
    if (!EG(active_symbol_table)) {
#if PHP_MINOR_VERSION < 5
      zval *orig_this = EG(This);
      EG(This) = NULL;
      zend_rebuild_symbol_table(TSRMLS_C);
      EG(This) = orig_this;
#else
      zend_rebuild_symbol_table(TSRMLS_C);
#endif

    }
#endif
    zend_execute(op_array TSRMLS_CC);

    destroy_op_array(op_array TSRMLS_CC);
    efree(op_array);
    if (!EG(exception)) {
      if (EG(return_value_ptr_ptr) && *EG(return_value_ptr_ptr)) {
        zval_ptr_dtor(EG(return_value_ptr_ptr));
      }
    }
    BW_RESTORE_EG_ENVIRON();
      return 1;
  }

  return 0;
}

