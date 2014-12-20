/*
 * AOChat -- library for talking with the Anarchy Online chat servers
 *
 * Copyright (c) 2002-2005, Oskari Saarenmaa <oskari@saarenmaa.fi>
 *
 * This file is under the 2-clause BSD license.
 * See the file `LICENSE` for details.
 *
 */


#ifndef _AOKEX_H
#define _AOKEX_H

#include "php_aokex.h"

typedef unsigned int AoUInt32;
typedef struct aokex_math_s aokex_math_t;

char *aokex_login_key(char *serverseed, char *username, char *password);

aokex_math_t *aokex_math_init();
void aokex_math_uninit(aokex_math_t *ctx);
AoUInt32 aokex_math_random32(aokex_math_t *ctx);
void aokex_math_dh_init(aokex_math_t *ctx, const char *g, const char *p);
char *aokex_math_dh_x(aokex_math_t *ctx, AoUInt32 exponent);
char *aokex_math_dh_k(aokex_math_t *ctx, const char *y);

#endif /* _AOKEX_H */
