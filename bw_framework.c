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
#include <stdint.h>

zend_class_entry bf_frameword_ce;
zend_class_entry *bf_frameword_class_entry_ptr;

PHP_METHOD(bw_framework, on2);

ZEND_BEGIN_ARG_INFO_EX(arginfo_bw_framework_on2, 0, 0, 1)
    ZEND_ARG_INFO(0, event_name)
    //ZEND_ARG_INFO(0, callback)
ZEND_END_ARG_INFO()


const zend_function_entry bw_framework_methods[] =
{
    PHP_ME(bw_framework, on2, arginfo_bw_framework_on2, ZEND_ACC_PUBLIC)
    PHP_FE_END
};



void bw_framework_init(int module_number TSRMLS_DC)
{
    INIT_CLASS_ENTRY(bf_frameword_ce, "bw_framework", bw_framework_methods);
    bf_frameword_class_entry_ptr = zend_register_internal_class(&bf_frameword_ce);
}



int * makemem(){
  int *a = NULL;
  a = (int*)malloc(sizeof(int));
  *a =10;
  return a;
}
PHP_METHOD(bw_framework, on2)
{
    zval *config;
    char * msg;
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z", &config) == FAILURE)
    {
        return;
    }
    BINWIN_G(config) = config;
    char *key = "ROOT";
    zval** val;
    char* root;
    HashTable *arr = Z_ARRVAL_P(config);
    //BINWIN_G(config) = arr;
    if (zend_hash_find(arr, key, strlen(key) + 1, (void**)&val) != FAILURE) //查找散列表中所需的参数          
    {
      root =  Z_STRVAL_P(*val); //值
      BINWIN_G(root) = root;
      char * code;
      char *file;
      
      char *str_controller ,*str_action;
      zval* controller = bw_request_query(TRACK_VARS_GET, ZEND_STRL("c") TSRMLS_CC);
      //return ;
      if(Z_TYPE_P(controller) != IS_NULL){
        str_controller =  Z_STRVAL_P(controller); //值       
      }else{
        str_controller =  ZEND_STRL("index"); //值         
      }

      zval* action = bw_request_query(TRACK_VARS_GET, ZEND_STRL("a") TSRMLS_CC);
      if (Z_TYPE_P(action) != IS_NULL) {
        str_action =  Z_STRVAL_P(action); //值         
      }else{
        str_action =  ZEND_STRL("index"); //值  
      }
    
      int len = spprintf(&file,0,"%s/controller/%s.controller.php",root,str_controller);
      //php_error_docref(NULL TSRMLS_CC, E_WARNING, file);
      if(!bw_loader_import(file,len, 0 TSRMLS_CC)){
        spprintf(&msg,0,"can not import file:%s",file);
        php_error_docref(NULL TSRMLS_CC, E_WARNING, msg);
        return;
      }
//return;
      zval *obj,*method_construct,*method_action,*name,*age;
      MAKE_STD_ZVAL(obj);
      MAKE_STD_ZVAL(method_construct);
      MAKE_STD_ZVAL(method_action);
      ZVAL_STRINGL(method_construct,"__construct",strlen("__construct"),1);
      ZVAL_STRINGL(method_action,str_action,strlen(str_action),1);
      

      zend_class_entry ** ce;
      if (zend_lookup_class(str_controller, strlen(str_controller), &ce TSRMLS_CC) != SUCCESS) {
        spprintf(&msg,0,"Can not find controller %s",str_controller);
        php_error_docref(NULL TSRMLS_CC, E_WARNING, msg);
        return;
      }

      spprintf(&msg,0,"controller: %s,action:%s",str_controller,str_action);
      //php_error_docref(NULL TSRMLS_CC, E_ERROR, msg);
      //__construct中存在参数 
      
      zval *retval;
      MAKE_STD_ZVAL(retval);
      object_init_ex(obj, *ce);  
      /*
      if(call_user_function(NULL, &obj, method_construct, retval,0,NULL TSRMLS_CC) != SUCCESS){
        php_error_docref(NULL TSRMLS_CC, E_WARNING, "Unable to call _construct()");
      }*/
      if(call_user_function(NULL, &obj, method_action,retval,0,NULL TSRMLS_CC) != SUCCESS){
        php_error_docref(NULL TSRMLS_CC, E_WARNING, "Unable to call action");
      }
      //spprintf(&code,0,fmt,root);
      //zend_eval_string(code, NULL, (char *)"" TSRMLS_CC); //代码执行
      //return SUCCESS;      
    }

}




