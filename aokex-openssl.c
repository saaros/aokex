/*
 * AOChat -- library for talking with the Anarchy Online chat servers
 *
 * Copyright (c) 2002-2005, Oskari Saarenmaa <oskari@saarenmaa.fi>
 *
 * This file is under the 2-clause BSD license.
 * See the file `LICENSE` for details.
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
