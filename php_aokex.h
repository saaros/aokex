/*
 * AOChat -- library for talking with the Anarchy Online chat servers
 *
 * Copyright (c) 2002-2005, Oskari Saarenmaa <oskari@saarenmaa.fi>
 *
 * This file is under the 2-clause BSD license.
 * See the file `LICENSE` for details.
 *
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
