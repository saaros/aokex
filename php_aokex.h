/*
   +----------------------------------------------------------------------+
   | PHP Version 4                                                        |
   +----------------------------------------------------------------------+
   | Copyright (c) 1997-2002 The PHP Group                                |
   +----------------------------------------------------------------------+
   | This source file is subject to version 2.02 of the PHP license,      |
   | that is bundled with this package in the file LICENSE, and is        |
   | available at through the world-wide-web at                           |
   | http://www.php.net/license/2_02.txt.                                 |
   | If you did not receive a copy of the PHP license and are unable to   |
   | obtain it through the world-wide-web, please send a note to          |
   | license@php.net so we can mail you a copy immediately.               |
   +----------------------------------------------------------------------+
   | Author: Oskari Saarenmaa <auno@auno.org>                             |
   +----------------------------------------------------------------------+
*/

#ifndef PHP_AOKEX_H
#define PHP_AOKEX_H

#if HAVE_AOKEX

extern zend_module_entry aokex_module_entry;
#define aokex_module_ptr &aokex_module_entry

PHP_MINIT_FUNCTION(aokex);
PHP_MINFO_FUNCTION(aokex);
PHP_RINIT_FUNCTION(aokex);

PHP_FUNCTION(aokex_login_key);

ZEND_BEGIN_MODULE_GLOBALS(aokex)
ZEND_END_MODULE_GLOBALS(aokex)

#else /* HAVE_AOKEX */
#define aokex_module_ptr NULL
#endif /* HAVE_AOKEX */

#define phpext_aokex_ptr aokex_module_ptr

void *aokex_malloc(size_t);
void *aokex_realloc(void *, size_t, size_t);
void aokex_free(void *, size_t);

#endif /* PHP_AOKEX_H */
