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

zend_class_entry bw_controller_ce;
zend_class_entry *bw_controller_class_entry_ptr;

PHP_METHOD(bw_controller, display);



ZEND_BEGIN_ARG_INFO_EX(arginfo_bw_controller_display, 0, 0, 1)
    ZEND_ARG_INFO(0, event_name)
    ZEND_ARG_ARRAY_INFO(0, config, 1)
ZEND_END_ARG_INFO()


const zend_function_entry bw_controller_methods[] =
{
    PHP_ME(bw_controller, display, NULL, ZEND_ACC_PUBLIC)
    PHP_FE_END
};




void bw_controller_init(int module_number TSRMLS_DC)
{
    INIT_CLASS_ENTRY(bw_controller_ce, "bw_controller", bw_controller_methods);
    bw_controller_class_entry_ptr = zend_register_internal_class(&bw_controller_ce);
}



PHP_METHOD(bw_controller, display)
{
  zval *t_key,*data;
  if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "zz", &t_key, &data) == FAILURE)
  {
      return;
  }
  if(Z_TYPE_P(data) != IS_ARRAY){
      php_error_docref(NULL TSRMLS_CC, E_WARNING, "param 2 must be a array!"); 
  }else{
      zval function  = {{0}, 0},*param[] = {data};
      ZVAL_STRING(&function, "extract", 0);
      if (call_user_function(EG(function_table), NULL, &function, return_value, 1, param TSRMLS_CC) == FAILURE) {
        if (return_value) {
          zval_dtor(return_value);
        }
        php_error_docref(NULL TSRMLS_CC, E_WARNING, "display:call function 'extract' error!");
        RETURN_FALSE;
      }
      //char *code;
      //spprintf(&code,0,"var_dump($title);");
      //zend_eval_string(code, NULL, (char *)"" TSRMLS_CC); //代码执行
  }

  char *tpl_file;
  int len = spprintf(&tpl_file,0,"%s/view/%s.php",BINWIN_G(root), Z_STRVAL_P(t_key));

  bw_loader_import(tpl_file,len, 0 TSRMLS_CC);
}


