/*
 * $Id: aokex-gmp.c,v 1.2 2005/01/05 14:19:16 pickett Exp $
 *
 * AOChat -- library for talking with the Anarchy Online chat servers
 * Copyright (C) 2002-2005  Oskari Saarenmaa <auno@auno.org>.
 * All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
 * USA
 *
 *
 * ***
 *
 * The license does not actually allow this program to be linked against
 * PHP, which uses a different license, but since I own the copyrights
 * to this code, I hereby explicitly allow linking this object to a PHP
 * module called 'aokex' which is also authored by me.
 *
 * Oskari Saarenmaa / auno@auno.org / 2005-Jan-05.
 *
 *
 */


#include <gmp.h>
#include <stdlib.h>
#include <sys/time.h>
#include "aokex.h"

struct aokex_math_s
{
  gmp_randstate_t rstate;
  mpz_t tmpnum;
  mpz_t dh_g, dh_p, dh_y, dh_x, dh_secret, dh_k;
};

aokex_math_t *
aokex_math_init()
{
  struct timeval tv;
  aokex_math_t *ctx = (aokex_math_t *)aokex_malloc(sizeof(aokex_math_t));
  mp_set_memory_functions(aokex_malloc, aokex_realloc, aokex_free);
  gmp_randinit_default(ctx->rstate);
  gettimeofday(&tv, NULL);
  gmp_randseed_ui(ctx->rstate, tv.tv_sec^tv.tv_usec);
  mpz_init(ctx->tmpnum);
  mpz_init(ctx->dh_y);
  mpz_init(ctx->dh_x);
  mpz_init(ctx->dh_secret);
  mpz_init(ctx->dh_k);
  mpz_init(ctx->dh_g);
  mpz_init(ctx->dh_p);
  return ctx;
}

void
aokex_math_uninit(aokex_math_t *ctx)
{
  gmp_randclear(ctx->rstate);
  mpz_clear(ctx->tmpnum);
  mpz_clear(ctx->dh_y);
  mpz_clear(ctx->dh_x);
  mpz_clear(ctx->dh_secret);
  mpz_clear(ctx->dh_k);
  mpz_clear(ctx->dh_g);
  mpz_clear(ctx->dh_p);
  aokex_free(ctx, sizeof(aokex_math_t));
}

AoUInt32
aokex_math_random32(aokex_math_t *ctx)
{
  mpz_urandomb(ctx->tmpnum, ctx->rstate, 32);
  return (AoUInt32)mpz_get_ui(ctx->tmpnum);
}

void
aokex_math_dh_init(aokex_math_t *ctx,
                   const char *g,
                   const char *p)
{
  mpz_set_str(ctx->dh_p, p, 16);
  mpz_set_str(ctx->dh_g, g, 16);
}

char *
aokex_math_dh_x(aokex_math_t *ctx,
                AoUInt32 exponent)
{
  char *ret;
  mpz_urandomb(ctx->dh_secret, ctx->rstate, exponent);
  mpz_powm(ctx->dh_x, ctx->dh_g, ctx->dh_secret, ctx->dh_p);
  gmp_asprintf(&ret, "%Zx", ctx->dh_x);
  return ret;
}

char *
aokex_math_dh_k(aokex_math_t *ctx,
                const char *y)
{
  char *ret;
  mpz_set_str(ctx->dh_y, y, 16);
  mpz_powm(ctx->dh_k, ctx->dh_y, ctx->dh_secret, ctx->dh_p);
  gmp_asprintf(&ret, "%Zx", ctx->dh_k);
  return ret;
}
