/*
 * $Id: aokex.h,v 1.1 2005/01/05 14:06:02 pickett Exp $
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


#ifndef _AOKEX_H
#define _AOKEX_H

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
