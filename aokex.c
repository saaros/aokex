/*
 * $Id: aokex.c,v 1.3 2005/01/07 02:42:08 pickett Exp $
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

/*
 * based on the PHP class I wrote some time ago.. this C-port is
 * quite unfinished, the only thing it can do at the moment is generate
 * the required login keys (onesided Diffie-Hellman key exchange with
 * TEA encrypted message body)
 *
 * DH:  http://www.rsasecurity.com/rsalabs/faq/3-6-1.html
 * TEA: http://www.ftp.cl.cam.ac.uk/ftp/papers/djw-rmn/djw-rmn-tea.html
 *
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/types.h>

#include "aokex.h"

static char * aokex_cipher(char *, char *, int);
static void   aokex_tea_encipher(AoUInt32[4], AoUInt32[4]);


char *
aokex_login_key(char *serverseed,
                char *username,
                char *password)
{
  char *dh_str_x, *dh_str_k, *challenge, *plaintext, *ciphertext, *ret;
  size_t clen, plen;
  AoUInt32 cookie[2];

  const char
    dh_str_y[] =
    "9c32cc23d559ca90fc31be72df817d0e124769e809f936bc"
    "14360ff4bed758f260a0d596584eacbbc2b88bdd41041616"
    "3e11dbf62173393fbc0c6fefb2d855f1a03dec8e9f105bba"
    "d91b3437d8eb73fe2f44159597aa4053cf788d2f9d7012fb"
    "8d7c4ce3876f7d6cd5d0c31754f4cd96166708641958de54"
    "a6def5657b9f2e92",
    dh_str_p[] =
    "eca2e8c85d863dcdc26a429a71a9815ad052f6139669dd65"
    "9f98ae159d313d13c6bf2838e10a69b6478b64a24bd054ba"
    "8248e8fa778703b418408249440b2c1edd28853e240d8a7e"
    "49540b76d120d3b1ad2878b1b99490eb4a2a5e84caa8a91c"
    "ecbdb1aa7c816e8be343246f80c637abc653b893fd91686c"
    "f8d32d6cfe5f2a6f",
    dh_str_g[] = "5";
  const int
    dh_exponent = 192;

  aokex_math_t *ctx = aokex_math_init();

  /* diffie-hellman */
  aokex_math_dh_init(ctx, dh_str_g, dh_str_p);
  dh_str_x = aokex_math_dh_x(ctx, dh_exponent);
  dh_str_k = aokex_math_dh_k(ctx, dh_str_y);

  /* session cookie */
  cookie[0] = aokex_math_random32(ctx);
  cookie[1] = aokex_math_random32(ctx);

  /* uninit math */
  aokex_math_uninit(ctx);

  /* challenge = username '|' serverseed '|' password */
  /* payload = cookie[8] challenge-length[4] challenge */
  clen = strlen(username) + 1 + strlen(serverseed) + 1 + strlen(password);
  plen = 8+4+clen;
  plen = ((plen+7)/8)*8; /* round up to 8 */

  plaintext = (char *)aokex_malloc(plen+1); /* make room for a null */

  /* insert cookie */
  memcpy(plaintext+0, &cookie[0], 4);
  memcpy(plaintext+4, &cookie[1], 4);

  /* pack the content-length in network byte order */
  plaintext[ 8] = (clen>>24) & 0xff;
  plaintext[ 9] = (clen>>16) & 0xff;
  plaintext[10] = (clen>> 8) & 0xff;
  plaintext[11] = (clen>> 0) & 0xff;

  /* insert the challenge in our plaintext string */
  snprintf(plaintext+12, plen-12, "%s|%s|%s",
    username, serverseed, password);
  memset(plaintext+12+clen, 0, plen-clen-12);

  /* 'ciphertext' is a newly allocated string */
  ciphertext = aokex_cipher(dh_str_k, plaintext, plen);

  /* the return value is dh_str_x '-' ciphertext */
  ret = (char *)aokex_malloc(strlen(dh_str_x)+1+strlen(ciphertext)+1);
  strcpy(ret, dh_str_x);
  strcat(ret, "-");
  strcat(ret, ciphertext);

  /* free any memory we allocated */
  aokex_free(dh_str_x, 0);
  aokex_free(dh_str_k, 0);
  aokex_free(plaintext, 0);
  aokex_free(ciphertext, 0);

  return ret;
}

static char *
aokex_cipher(char *key,
             char *str,
             int len)
{
  AoUInt32 i, cycle[4], akey[4];
  char *text=(char *)aokex_malloc(len*2+1), *p=text;

  /* Zero the cycle */
  memset(cycle, 0, sizeof cycle);

  /* Convert the hexadecimal key to binary */
  sscanf(key, "%08x%08x%08x%08x", &akey[0], &akey[1], &akey[2], &akey[3]);
  akey[0] = htonl(akey[0]);
  akey[1] = htonl(akey[1]);
  akey[2] = htonl(akey[2]);
  akey[3] = htonl(akey[3]);

  for(i=0; i<len/4;)
  {
    cycle[0] = ((str[4*i+3] << 24) | (str[4*i+2] << 16)  |
                (str[4*i+1] <<  8) | (str[4*i+0] <<  0)) ^ cycle[2];
    i++;
    cycle[1] = ((str[4*i+3] << 24) | (str[4*i+2] << 16)  |
                (str[4*i+1] <<  8) | (str[4*i+0] <<  0)) ^ cycle[3];
    i++;
    aokex_tea_encipher(cycle, akey);
    sprintf(p, "%08x%08x", htonl(cycle[2]), htonl(cycle[3]));
    p += 16;
  }

  return text;
}

static void
aokex_tea_encipher(AoUInt32 cycle[4],
                   AoUInt32 key[4])
{
  AoUInt32 a = cycle[0],
           b = cycle[1],
           sum = 0,
           delta = 0x9e3779b9,
           i = 32;

  while(i--)
  {
    sum += delta;
    a   += ((b << 4 & 0xfffffff0) + key[0]) ^ (b + sum) ^ ((b >> 5 & 0x7ffffff) + key[1]);
    b   += ((a << 4 & 0xfffffff0) + key[2]) ^ (a + sum) ^ ((a >> 5 & 0x7ffffff) + key[3]);
  }

  cycle[2] = a;
  cycle[3] = b;
}
