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

#ifndef PHP_BINWIN_H
#define PHP_BINWIN_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "bw_function.h"

extern zend_module_entry binwin_module_entry;
#define phpext_binwin_ptr &binwin_module_entry

#define PHP_BINWIN_VERSION "0.1.0" /* Replace with version number for your extension */

#ifdef PHP_WIN32
#	define PHP_BINWIN_API __declspec(dllexport)
#elif defined(__GNUC__) && __GNUC__ >= 4
#	define PHP_BINWIN_API __attribute__ ((visibility("default")))
#else
#	define PHP_BINWIN_API
#endif

#ifdef ZTS
#include "TSRM.h"
#endif

PHP_MINIT_FUNCTION(binwin);
PHP_MSHUTDOWN_FUNCTION(binwin);
PHP_RINIT_FUNCTION(binwin);
PHP_RSHUTDOWN_FUNCTION(binwin);
PHP_MINFO_FUNCTION(binwin);

PHP_FUNCTION(confirm_binwin_compiled);	/* For testing, remove later. */


PHP_METHOD(binwin_fw, __construct);
PHP_METHOD(binwin_fw, __destruct);
PHP_METHOD(binwin_fw, getName);
PHP_METHOD(binwin_fw, setName);



/* 
  	Declare any global variables you may need between the BEGIN
	and END macros here:     
*/
ZEND_BEGIN_MODULE_GLOBALS(binwin)
	char *root;
  zval *config;
  int running;  
ZEND_END_MODULE_GLOBALS(binwin)


/* In every utility function you add that needs to use variables 
   in php_binwin_globals, call TSRMLS_FETCH(); after declaring other 
   variables used by that function, or better yet, pass in TSRMLS_CC
   after the last function argument and declare your utility function
   with TSRMLS_DC after the last declared argument.  Always refer to
   the globals in your function as BINWIN_G(variable).  You are 
   encouraged to rename these macros something shorter, see
   examples in any other php module directory.
*/

#ifdef ZTS
#define BINWIN_G(v) TSRMG(binwin_globals_id, zend_binwin_globals *, v)
#else
#define BINWIN_G(v) (binwin_globals.v)
#endif
#define BW_STARTUP(module)         ZEND_MODULE_STARTUP_N(bw_##module)(INIT_FUNC_ARGS_PASSTHRU)
extern ZEND_DECLARE_MODULE_GLOBALS(binwin);

#endif	/* PHP_BINWIN_H */


/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
