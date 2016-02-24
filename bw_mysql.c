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

#define MYSQL_ASSOC   1<<0
#define MYSQL_NUM   1<<1
#define MYSQL_BOTH    (MYSQL_ASSOC|MYSQL_NUM)

zend_class_entry bw_mysql_ce;
zend_class_entry *bw_mysql_class_entry_ptr;

zval *host,*user,*password,*database,* persist,* conn,* result,
* fields,* check_fields,* tbname,* addNewFlag,* isnew,* opened; //标志已经连接过一次

PHP_METHOD(bw_mysql, __construct);
PHP_METHOD(bw_mysql, getOne);
PHP_METHOD(bw_mysql, getAll);

const zend_function_entry bw_mysql_methods[] =
{
    PHP_ME(bw_mysql, __construct, NULL, ZEND_ACC_PUBLIC)  
    PHP_ME(bw_mysql, getOne, NULL, ZEND_ACC_PUBLIC)
    PHP_ME(bw_mysql, getAll, NULL, ZEND_ACC_PUBLIC)       
    PHP_FE_END
};


void bw_mysql_init(int module_number TSRMLS_DC)
{
    INIT_CLASS_ENTRY(bw_mysql_ce, "bw_mysql", bw_mysql_methods);
    bw_mysql_class_entry_ptr = zend_register_internal_class(&bw_mysql_ce);
}

PHP_METHOD(bw_mysql, __construct)
{
  zval *dbname;
  if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "zzzz", &host, &user,&password,&dbname) == FAILURE)
  {
      return;
  }
  php_error_docref(NULL TSRMLS_CC, E_WARNING, "mysql_connect:host:%s user:%s pwd:%s db:%s",Z_STRVAL_P(host), Z_STRVAL_P(user), Z_STRVAL_P(password),Z_STRVAL_P(dbname));
  //MAKE_STD_ZVAL(isnew);
  //ZVAL_BOOL(isnew,1);

  zval function  = {{0}, 0},*param[] = {host, user, password, dbname};
  ZVAL_STRING(&function, "mysqli_connect", 0);
  MAKE_STD_ZVAL(conn);
  MAKE_STD_ZVAL(result);  
  if (call_user_function(EG(function_table), NULL, &function, conn, 4, param TSRMLS_CC) == FAILURE) {
    if (conn) {
      zval_dtor(conn);
    }
    php_error_docref(NULL TSRMLS_CC, E_WARNING, "display:call function 'mysqli_connect' error!");
    return;
  }
/*
  zval *param2[] = {dbname,conn};
  ZVAL_STRING(&function, "mysqli_select_db", 0);
  if (call_user_function(EG(function_table), NULL, &function, return_value, 2, param2 TSRMLS_CC) == FAILURE) {
    if (return_value) {
      zval_dtor(return_value);
    }
    php_error_docref(NULL TSRMLS_CC, E_WARNING, "display:call function 'mysql_select_db' error!");
  }*/
}


void show_db_error(){
  zval function  = {{0}, 0};
  zval *param[] = {conn},*ret;
    MAKE_STD_ZVAL(ret);
  ZVAL_STRING(&function, "mysqli_error", 0);
  if (call_user_function(EG(function_table), NULL, &function, ret, 1, param TSRMLS_CC) == FAILURE) {
    if (ret) {
      zval_dtor(ret);
    }
    php_error_docref(NULL TSRMLS_CC, E_WARNING, "display:call function 'mysqli_error' error!");
    return ;
  }
  if (ret) {
    php_error_docref(NULL TSRMLS_CC, E_WARNING, "db error:%s",Z_STRVAL_P(ret));     
    zval_dtor(ret);
  }
}

PHP_METHOD(bw_mysql, getOne)
{
  zval *sql;
  if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z", &sql) == FAILURE)
  {
      return;
  }

  zval function  = {{0}, 0},*param[] = {conn,sql};
  ZVAL_STRING(&function, "mysqli_query", 0);
  if (call_user_function(EG(function_table), NULL, &function, result, 2, param TSRMLS_CC) == FAILURE) {
    if (result) {
      zval_dtor(result);
    }
    php_error_docref(NULL TSRMLS_CC, E_WARNING, "display:call function 'mysql_query' error!");
    return;
  }

  zval *array;
  zval *param2[] = {result};
  ZVAL_STRING(&function, "mysqli_fetch_assoc", 0);
  MAKE_STD_ZVAL(array);  
  if (call_user_function(EG(function_table), NULL, &function, array, 1, param2 TSRMLS_CC) == FAILURE) {
    if (array) {
      zval_dtor(array);
    }
    php_error_docref(NULL TSRMLS_CC, E_WARNING, "display:call function 'mysql_select_db' error!");
    return;
  }
  if(Z_TYPE_P(array) != IS_ARRAY){
    show_db_error();
  }
  RETURN_ZVAL(array, 1, 1);
}


PHP_METHOD(bw_mysql, getAll)
{
  
}

