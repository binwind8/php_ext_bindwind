dnl $Id$
dnl config.m4 for extension binwin

dnl Comments in this file start with the string 'dnl'.
dnl Remove where necessary. This file will not work
dnl without editing.

dnl If your extension references something external, use with:

PHP_ARG_WITH(binwin, for binwin support,
Make sure that the comment is aligned:
[  --with-binwin             Include binwin support])

dnl Otherwise use enable:

PHP_ARG_ENABLE(binwin, whether to enable binwin support,
Make sure that the comment is aligned:
[  --enable-binwin           Enable binwin support])

if test "$PHP_BINWIN" != "no"; then
  dnl Write more examples of tests here...

  dnl # --with-binwin -> check with-path
  dnl SEARCH_PATH="/usr/local /usr"     # you might want to change this
  dnl SEARCH_FOR="/include/binwin.h"  # you most likely want to change this
  dnl if test -r $PHP_BINWIN/$SEARCH_FOR; then # path given as parameter
  dnl   BINWIN_DIR=$PHP_BINWIN
  dnl else # search default path list
  dnl   AC_MSG_CHECKING([for binwin files in default path])
  dnl   for i in $SEARCH_PATH ; do
  dnl     if test -r $i/$SEARCH_FOR; then
  dnl       BINWIN_DIR=$i
  dnl       AC_MSG_RESULT(found in $i)
  dnl     fi
  dnl   done
  dnl fi
  dnl
  dnl if test -z "$BINWIN_DIR"; then
  dnl   AC_MSG_RESULT([not found])
  dnl   AC_MSG_ERROR([Please reinstall the binwin distribution])
  dnl fi

  dnl # --with-binwin -> add include path
  dnl PHP_ADD_INCLUDE($BINWIN_DIR/include)

  dnl # --with-binwin -> check for lib and symbol presence
  dnl LIBNAME=binwin # you may want to change this
  dnl LIBSYMBOL=binwin # you most likely want to change this 

  dnl PHP_CHECK_LIBRARY($LIBNAME,$LIBSYMBOL,
  dnl [
  dnl   PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $BINWIN_DIR/$PHP_LIBDIR, BINWIN_SHARED_LIBADD)
  dnl   AC_DEFINE(HAVE_BINWINLIB,1,[ ])
  dnl ],[
  dnl   AC_MSG_ERROR([wrong binwin lib version or lib not found])
  dnl ],[
  dnl   -L$BINWIN_DIR/$PHP_LIBDIR -lm
  dnl ])
  dnl
  dnl PHP_SUBST(BINWIN_SHARED_LIBADD)

    binwin_source_file="binwin.c \
        bw_model.c\
        bw_mysql.c \   
        bw_function.c\
        bw_framework.c\
        bw_controller.c"


  PHP_NEW_EXTENSION(binwin, $binwin_source_file, $ext_shared)


fi
