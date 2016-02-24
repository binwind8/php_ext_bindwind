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
  | Author:                                                              |
  +----------------------------------------------------------------------+
*/

/* $Id$ */

#ifndef BW_FUNCTION_H
#define BW_FUNCTION_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"


#if ((PHP_MAJOR_VERSION == 5) && (PHP_MINOR_VERSION > 2)) || (PHP_MAJOR_VERSION > 5)
#define BW_STORE_EG_ENVIRON() \
  { \
    zval ** __old_return_value_pp   = EG(return_value_ptr_ptr); \
    zend_op ** __old_opline_ptr   = EG(opline_ptr); \
    zend_op_array * __old_op_array  = EG(active_op_array);

#define BW_RESTORE_EG_ENVIRON() \
    EG(return_value_ptr_ptr) = __old_return_value_pp;\
    EG(opline_ptr)       = __old_opline_ptr; \
    EG(active_op_array)    = __old_op_array; \
  }

#else

#define BW_STORE_EG_ENVIRON() \
  { \
    zval ** __old_return_value_pp        = EG(return_value_ptr_ptr); \
    zend_op ** __old_opline_ptr        = EG(opline_ptr); \
    zend_op_array * __old_op_array       = EG(active_op_array); \
    zend_function_state * __old_func_state = EG(function_state_ptr);

#define BW_RESTORE_EG_ENVIRON() \
    EG(return_value_ptr_ptr) = __old_return_value_pp;\
    EG(opline_ptr)       = __old_opline_ptr; \
    EG(active_op_array)    = __old_op_array; \
    EG(function_state_ptr)   = __old_func_state; \
  }

#endif



int bw_loader_import(char *path, int len, int use_path TSRMLS_DC);
zval * bw_request_query(uint type, char * name,uint len TSRMLS_DC);



#endif	/* BW_FUNCTION_H */


/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
