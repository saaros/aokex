/*
 * $Id: aokex-openssl.c,v 1.1 2005/01/07 02:58:43 pickett Exp $
 *
 * AOChat -- library for talking with the Anarchy Online chat servers
 * Copyright (C) 2005  Oskari Saarenmaa <auno@auno.org>.
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
 * Oskari Saarenmaa / auno@auno.org / 2005-Jan-07.
 *
 *
 */


#include <openssl/dh.h>
#include <openssl/rand.h>
#include <stdlib.h>
#include "aokex.h"

struct aokex_math_s
{
  DH *dh;
};

aokex_math_t *
aokex_math_init()
{
  aokex_math_t *ctx = (aokex_math_t *)aokex_malloc(sizeof(aokex_math_t));
  ctx->dh = DH_new();
  return ctx;
}

void
aokex_math_uninit(aokex_math_t *ctx)
{
  DH_free(ctx->dh);
  aokex_free(ctx, sizeof(aokex_math_t));
}

AoUInt32
aokex_math_random32(aokex_math_t *ctx)
{
  AoUInt32 x;
  RAND_pseudo_bytes((unsigned char*)&x, 4);
  return x;
}

void
aokex_math_dh_init(aokex_math_t *ctx,
                   const char *g,
                   const char *p)
{
  ctx->dh->p = BN_new();
  BN_hex2bn(&ctx->dh->p, p);
  ctx->dh->g = BN_new();
  BN_hex2bn(&ctx->dh->g, g);
}

static inline char *
openssl_to_aokex_str(char *src)
{
  unsigned char *ret = aokex_malloc(strlen(src)+1);
  strcpy(ret, src);
  OPENSSL_free(src);
  return ret;
}

char *
aokex_math_dh_x(aokex_math_t *ctx,
                AoUInt32 exponent)
{
  DH_generate_key(ctx->dh);
  return openssl_to_aokex_str(BN_bn2hex(ctx->dh->pub_key));
}

char *
aokex_math_dh_k(aokex_math_t *ctx,
                const char *y)
{
  char *ret;
  size_t ksz = DH_size(ctx->dh);
  unsigned char *k = aokex_malloc(ksz);
  BIGNUM *by = BN_new();
  BN_hex2bn(&by, y);
  DH_compute_key(k, by, ctx->dh);
  BN_bin2bn(k, ksz, by);
  ret = BN_bn2hex(by);
  BN_clear_free(by);
  aokex_free(k, ksz);
  return openssl_to_aokex_str(ret);
}
