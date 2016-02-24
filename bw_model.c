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

zend_class_entry bw_model_ce;
zend_class_entry *bw_model_class_entry_ptr;

PHP_METHOD(bw_model, mysql);



const zend_function_entry bw_model_methods[] =
{
    PHP_ME(bw_model, mysql, NULL, ZEND_ACC_PUBLIC)
    PHP_FE_END
};




void bw_model_init(int module_number TSRMLS_DC)
{
    INIT_CLASS_ENTRY(bw_model_ce, "bw_model", bw_model_methods);
    bw_model_class_entry_ptr = zend_register_internal_class(&bw_model_ce);
}



PHP_METHOD(bw_model, mysql)
{
    zval *obj,*method_construct,*method_action,*name,*age;
    MAKE_STD_ZVAL(obj);
    MAKE_STD_ZVAL(method_construct);
    MAKE_STD_ZVAL(method_action);
    ZVAL_STRINGL(method_construct,"__construct",strlen("__construct"),1);

    zend_class_entry ** ce;
    char * obj_name = "bw_mysql",*msg;
    if (zend_lookup_class(obj_name, strlen(obj_name), &ce TSRMLS_CC) != SUCCESS) {
      spprintf(&msg,0,"Can not find class bw_mysql");
      php_error_docref(NULL TSRMLS_CC, E_WARNING, msg);
      return;
    }

    zval *retval;
    MAKE_STD_ZVAL(retval);
    object_init_ex(obj, *ce);  
    

    zval** val,*config,**data,**db_host,**db_user,**db_pwd,**db_dbname;
    //BINWIN_G(config) = arr;
    if (zend_hash_find((HashTable *)Z_ARRVAL_P(BINWIN_G(config)), 
    "MYSQL", strlen("MYSQL") + 1, (void**)&val) != FAILURE) //查找散列表中所需的参数          
    {
      if(zend_hash_index_find((HashTable *)Z_ARRVAL_P(*val), 0, (void **)&data) == FAILURE) {
          return;
      }
      if (zend_hash_find((HashTable *)Z_ARRVAL_P(*data), 
      "host", strlen("host") + 1, (void**)&db_host) == FAILURE) //查找散列表中所需的参数          
      {
          return;
      }
      if (zend_hash_find((HashTable *)Z_ARRVAL_P(*data), 
      "user", strlen("user") + 1, (void**)&db_user) == FAILURE) //查找散列表中所需的参数          
      {
          return;
      }
      if (zend_hash_find((HashTable *)Z_ARRVAL_P(*data), 
      "pwd", strlen("pwd") + 1, (void**)&db_pwd) == FAILURE) //查找散列表中所需的参数          
      {
          return;
      }
      if (zend_hash_find((HashTable *)Z_ARRVAL_P(*data), 
      "dbname", strlen("dbname") + 1, (void**)&db_dbname) == FAILURE) //查找散列表中所需的参数          
      {
          return;
      }
    }
    zval *param[] = {*db_host, *db_user, *db_pwd, *db_dbname};
    if(call_user_function(NULL, &obj, method_construct, retval,4,param TSRMLS_CC) != SUCCESS){
      php_error_docref(NULL TSRMLS_CC, E_WARNING, "Unable to call _construct()");
      RETURN_FALSE;
    }
    RETURN_ZVAL(obj,1,1);
}


