/*
 * $Id: php_aokex.c,v 1.2 2005/01/05 14:19:16 pickett Exp $
 *
 * +----------------------------------------------------------------------+
 * | PHP Version 4                                                        |
 * +----------------------------------------------------------------------+
 * | Copyright (c) 1997-2002 The PHP Group                                |
 * +----------------------------------------------------------------------+
 * | This source file is subject to version 2.02 of the PHP license,      |
 * | that is bundled with this package in the file LICENSE, and is        |
 * | available at through the world-wide-web at                           |
 * | http://www.php.net/license/2_02.txt.                                 |
 * | If you did not receive a copy of the PHP license and are unable to   |
 * | obtain it through the world-wide-web, please send a note to          |
 * | license@php.net so we can mail you a copy immediately.               |
 * +----------------------------------------------------------------------+
 * | Author: Oskari Saarenmaa <auno@auno.org>                             |
 * +----------------------------------------------------------------------+
 *
 */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#include "php.h"

#if HAVE_AOKEX
#include <stdio.h>
#include "ext/standard/info.h"
#include "aokex.h"
#include "php_aokex.h"

ZEND_DECLARE_MODULE_GLOBALS(aokex)

function_entry aokex_functions[] =
{
  PHP_FE(aokex_login_key, NULL)
  {NULL,NULL,NULL}
};

zend_module_entry aokex_module_entry =
{
  STANDARD_MODULE_HEADER,
  "aokex",
  aokex_functions,
  PHP_MINIT(aokex),
  NULL,
  PHP_RINIT(aokex),
  NULL,
  PHP_MINFO(aokex),
  NO_VERSION_YET,
  STANDARD_MODULE_PROPERTIES
};

#ifdef COMPILE_DL_AOKEX
ZEND_GET_MODULE(aokex)
#endif

PHP_MINFO_FUNCTION(aokex)
{
  php_info_print_table_start();
  php_info_print_table_row(2, "aokex support", "enabled");
  php_info_print_table_end();
}

PHP_MINIT_FUNCTION(aokex)
{
  return SUCCESS;
}

PHP_RINIT_FUNCTION(aokex)
{
  return SUCCESS;
}

void *
aokex_malloc(size_t n)
{
  return emalloc(n);
}

void *
aokex_realloc(void *p, size_t on, size_t nn)
{
  return erealloc(p, nn);
}

void
aokex_free(void *p, size_t n)
{
  efree(p);
}

PHP_FUNCTION(aokex_login_key)
{
  zval **username_arg, **password_arg, **serverseed_arg;
  char *result;

  if (ZEND_NUM_ARGS() != 3 ||
      zend_get_parameters_ex(3, &serverseed_arg, &username_arg, &password_arg) == FAILURE)
  {
    WRONG_PARAM_COUNT;
  }

  result = aokex_login_key(Z_STRVAL_PP(serverseed_arg),
                           Z_STRVAL_PP(username_arg),
                           Z_STRVAL_PP(password_arg));

  RETURN_STRINGL(result, strlen(result), 1);
}

#endif /* HAVE_AOKEX */
