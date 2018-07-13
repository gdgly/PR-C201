/*
 *  RFC 1321 compliant MD5 implementation
 *
 *  Copyright (C) 2001-2003  Christophe Devine
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include <string.h>

#include "stdafx.h"

#include "md5module.h"

#define GET_UINT32(n,b,i)                       \
{                                               \
    (n) = ( (uint32) (b)[(i)    ]       )       \
        | ( (uint32) (b)[(i) + 1] <<  8 )       \
        | ( (uint32) (b)[(i) + 2] << 16 )       \
        | ( (uint32) (b)[(i) + 3] << 24 );      \
}

#define PUT_UINT32(n,b,i)                       \
{                                               \
    (b)[(i)    ] = (uint8) ( (n)       );       \
    (b)[(i) + 1] = (uint8) ( (n) >>  8 );       \
    (b)[(i) + 2] = (uint8) ( (n) >> 16 );       \
    (b)[(i) + 3] = (uint8) ( (n) >> 24 );       \
}

void md5_starts( md5_context *ctx )
{
    ctx->total[0] = 0;
    ctx->total[1] = 0;

    ctx->state[0] = 0x67452301;
    ctx->state[1] = 0xEFCDAB89;
    ctx->state[2] = 0x98BADCFE;
    ctx->state[3] = 0x10325476;
}

void md5_process( md5_context *ctx, uint8 data[64] )
{
    uint32 X[16], A, B, C, D;

    GET_UINT32( X[0],  data,  0 );
    GET_UINT32( X[1],  data,  4 );
    GET_UINT32( X[2],  data,  8 );
    GET_UINT32( X[3],  data, 12 );
    GET_UINT32( X[4],  data, 16 );
    GET_UINT32( X[5],  data, 20 );
    GET_UINT32( X[6],  data, 24 );
    GET_UINT32( X[7],  data, 28 );
    GET_UINT32( X[8],  data, 32 );
    GET_UINT32( X[9],  data, 36 );
    GET_UINT32( X[10], data, 40 );
    GET_UINT32( X[11], data, 44 );
    GET_UINT32( X[12], data, 48 );
    GET_UINT32( X[13], data, 52 );
    GET_UINT32( X[14], data, 56 );
    GET_UINT32( X[15], data, 60 );

#define S(x,n) ((x << n) | ((x & 0xFFFFFFFF) >> (32 - n)))

#define P(a,b,c,d,k,s,t)                                \
{                                                       \
    a += F(b,c,d) + X[k] + t; a = S(a,s) + b;           \
}

    A = ctx->state[0];
    B = ctx->state[1];
    C = ctx->state[2];
    D = ctx->state[3];

#define F(x,y,z) (z ^ (x & (y ^ z)))

    P( A, B, C, D,  0,  7, 0xD76AA478 );
    P( D, A, B, C,  1, 12, 0xE8C7B756 );
    P( C, D, A, B,  2, 17, 0x242070DB );
    P( B, C, D, A,  3, 22, 0xC1BDCEEE );
    P( A, B, C, D,  4,  7, 0xF57C0FAF );
    P( D, A, B, C,  5, 12, 0x4787C62A );
    P( C, D, A, B,  6, 17, 0xA8304613 );
    P( B, C, D, A,  7, 22, 0xFD469501 );
    P( A, B, C, D,  8,  7, 0x698098D8 );
    P( D, A, B, C,  9, 12, 0x8B44F7AF );
    P( C, D, A, B, 10, 17, 0xFFFF5BB1 );
    P( B, C, D, A, 11, 22, 0x895CD7BE );
    P( A, B, C, D, 12,  7, 0x6B901122 );
    P( D, A, B, C, 13, 12, 0xFD987193 );
    P( C, D, A, B, 14, 17, 0xA679438E );
    P( B, C, D, A, 15, 22, 0x49B40821 );

#undef F

#define F(x,y,z) (y ^ (z & (x ^ y)))

    P( A, B, C, D,  1,  5, 0xF61E2562 );
    P( D, A, B, C,  6,  9, 0xC040B340 );
    P( C, D, A, B, 11, 14, 0x265E5A51 );
    P( B, C, D, A,  0, 20, 0xE9B6C7AA );
    P( A, B, C, D,  5,  5, 0xD62F105D );
    P( D, A, B, C, 10,  9, 0x02441453 );
    P( C, D, A, B, 15, 14, 0xD8A1E681 );
    P( B, C, D, A,  4, 20, 0xE7D3FBC8 );
    P( A, B, C, D,  9,  5, 0x21E1CDE6 );
    P( D, A, B, C, 14,  9, 0xC33707D6 );
    P( C, D, A, B,  3, 14, 0xF4D50D87 );
    P( B, C, D, A,  8, 20, 0x455A14ED );
    P( A, B, C, D, 13,  5, 0xA9E3E905 );
    P( D, A, B, C,  2,  9, 0xFCEFA3F8 );
    P( C, D, A, B,  7, 14, 0x676F02D9 );
    P( B, C, D, A, 12, 20, 0x8D2A4C8A );

#undef F
    
#define F(x,y,z) (x ^ y ^ z)

    P( A, B, C, D,  5,  4, 0xFFFA3942 );
    P( D, A, B, C,  8, 11, 0x8771F681 );
    P( C, D, A, B, 11, 16, 0x6D9D6122 );
    P( B, C, D, A, 14, 23, 0xFDE5380C );
    P( A, B, C, D,  1,  4, 0xA4BEEA44 );
    P( D, A, B, C,  4, 11, 0x4BDECFA9 );
    P( C, D, A, B,  7, 16, 0xF6BB4B60 );
    P( B, C, D, A, 10, 23, 0xBEBFBC70 );
    P( A, B, C, D, 13,  4, 0x289B7EC6 );
    P( D, A, B, C,  0, 11, 0xEAA127FA );
    P( C, D, A, B,  3, 16, 0xD4EF3085 );
    P( B, C, D, A,  6, 23, 0x04881D05 );
    P( A, B, C, D,  9,  4, 0xD9D4D039 );
    P( D, A, B, C, 12, 11, 0xE6DB99E5 );
    P( C, D, A, B, 15, 16, 0x1FA27CF8 );
    P( B, C, D, A,  2, 23, 0xC4AC5665 );

#undef F

#define F(x,y,z) (y ^ (x | ~z))

    P( A, B, C, D,  0,  6, 0xF4292244 );
    P( D, A, B, C,  7, 10, 0x432AFF97 );
    P( C, D, A, B, 14, 15, 0xAB9423A7 );
    P( B, C, D, A,  5, 21, 0xFC93A039 );
    P( A, B, C, D, 12,  6, 0x655B59C3 );
    P( D, A, B, C,  3, 10, 0x8F0CCC92 );
    P( C, D, A, B, 10, 15, 0xFFEFF47D );
    P( B, C, D, A,  1, 21, 0x85845DD1 );
    P( A, B, C, D,  8,  6, 0x6FA87E4F );
    P( D, A, B, C, 15, 10, 0xFE2CE6E0 );
    P( C, D, A, B,  6, 15, 0xA3014314 );
    P( B, C, D, A, 13, 21, 0x4E0811A1 );
    P( A, B, C, D,  4,  6, 0xF7537E82 );
    P( D, A, B, C, 11, 10, 0xBD3AF235 );
    P( C, D, A, B,  2, 15, 0x2AD7D2BB );
    P( B, C, D, A,  9, 21, 0xEB86D391 );

#undef F

    ctx->state[0] += A;
    ctx->state[1] += B;
    ctx->state[2] += C;
    ctx->state[3] += D;
}

void md5_update( md5_context *ctx, uint8 *input, uint32 length )
{
    uint32 left, fill;

    if( ! length ) return;

    left = ctx->total[0] & 0x3F;
    fill = 64 - left;

    ctx->total[0] += length;
    ctx->total[0] &= 0xFFFFFFFF;

    if( ctx->total[0] < length )
        ctx->total[1]++;

    if( left && length >= fill )
    {
        memcpy( (void *) (ctx->buffer + left),
                (void *) input, fill );
        md5_process( ctx, ctx->buffer );
        length -= fill;
        input  += fill;
        left = 0;
    }

    while( length >= 64 )
    {
        md5_process( ctx, input );
        length -= 64;
        input  += 64;
    }

    if( length )
    {
        memcpy( (void *) (ctx->buffer + left),
                (void *) input, length );
    }
}

static uint8 md5_padding[64] =
{
 0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

void md5_finish( md5_context *ctx, uint8 digest[16] )
{
    uint32 last, padn;
    uint32 high, low;
    uint8 msglen[8];

    high = ( ctx->total[0] >> 29 )
         | ( ctx->total[1] <<  3 );
    low  = ( ctx->total[0] <<  3 );

    PUT_UINT32( low,  msglen, 0 );
    PUT_UINT32( high, msglen, 4 );

    last = ctx->total[0] & 0x3F;
    padn = ( last < 56 ) ? ( 56 - last ) : ( 120 - last );

    md5_update( ctx, md5_padding, padn );
    md5_update( ctx, msglen, 8 );

    PUT_UINT32( ctx->state[0], digest,  0 );
    PUT_UINT32( ctx->state[1], digest,  4 );
    PUT_UINT32( ctx->state[2], digest,  8 );
    PUT_UINT32( ctx->state[3], digest, 12 );
};

/* ============================================================
des()
Description: DES algorithm,do encrypt or descrypt.
============================================================ */
int des(unsigned char *source,unsigned char * dest,unsigned char * inkey, int flg)
{
	unsigned char bufout[64],
	kwork[56], worka[48], kn[48], buffer[64], key[64],
	nbrofshift, temp1, temp2;
	int valindex;
	register BYTE i, j, k, iter;
	/* INITIALIZE THE TABLES */
	/* Table - s1 */
	static unsigned char s1[4][16] = {
	14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7,
	0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8,
	4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0,
	15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13 };
	/* Table - s2 */
	static unsigned char s2[4][16] = {
	15, 1, 8, 14, 6, 11, 3, 4, 9, 7, 2, 13, 12, 0, 5, 10,
	3, 13, 4, 7, 15, 2, 8, 14, 12, 0, 1, 10, 6, 9, 11, 5,
	0, 14, 7, 11, 10, 4, 13, 1, 5, 8, 12, 6, 9, 3, 2, 15,
	13, 8, 10, 1, 3, 15, 4, 2, 11, 6, 7, 12, 0, 5, 14, 9 };
	/* Table - s3 */
	static unsigned char s3[4][16] = {
	10, 0, 9, 14, 6, 3, 15, 5, 1, 13, 12, 7, 11, 4, 2, 8,
	13, 7, 0, 9, 3, 4, 6, 10, 2, 8, 5, 14, 12, 11, 15, 1,
	13, 6, 4, 9, 8, 15, 3, 0, 11, 1, 2, 12, 5, 10, 14, 7,
	1, 10, 13, 0, 6, 9, 8, 7, 4, 15, 14, 3, 11, 5, 2, 12 };
	/* Table - s4 */
	static unsigned char s4[4][16] = {
	7, 13, 14, 3, 0, 6, 9, 10, 1, 2, 8, 5, 11, 12, 4, 15,
	13, 8, 11, 5, 6, 15, 0, 3, 4, 7, 2, 12, 1, 10, 14, 9,
	10, 6, 9, 0, 12, 11, 7, 13, 15, 1, 3, 14, 5, 2, 8, 4,
	3, 15, 0, 6, 10, 1, 13, 8, 9, 4, 5, 11, 12, 7, 2, 14 };
	/* Table - s5 */
	static unsigned char s5[4][16] = {
	2, 12, 4, 1, 7, 10, 11, 6, 8, 5, 3, 15, 13, 0, 14, 9,
	14, 11, 2, 12, 4, 7, 13, 1, 5, 0, 15, 10, 3, 9, 8, 6,
	4, 2, 1, 11, 10, 13, 7, 8, 15, 9, 12, 5, 6, 3, 0, 14,
	11, 8, 12, 7, 1, 14, 2, 13, 6, 15, 0, 9, 10, 4, 5, 3 };
	/* Table - s6 */
	static unsigned char s6[4][16] = {
	12, 1, 10, 15, 9, 2, 6, 8, 0, 13, 3, 4, 14, 7, 5, 11,
	10, 15, 4, 2, 7, 12, 9, 5, 6, 1, 13, 14, 0, 11, 3, 8,
	9, 14, 15, 5, 2, 8, 12, 3, 7, 0, 4, 10, 1, 13, 11, 6,
	4, 3, 2, 12, 9, 5, 15, 10, 11, 14, 1, 7, 6, 0, 8, 13 };
	/* Table - s7 */
	static unsigned char s7[4][16] = {
	4, 11, 2, 14, 15, 0, 8, 13, 3, 12, 9, 7, 5, 10, 6, 1,
	13, 0, 11, 7, 4, 9, 1, 10, 14, 3, 5, 12, 2, 15, 8, 6,
	1, 4, 11, 13, 12, 3, 7, 14, 10, 15, 6, 8, 0, 5, 9, 2,
	6, 11, 13, 8, 1, 4, 10, 7, 9, 5, 0, 15, 14, 2, 3, 12 };
	/* Table - s8 */
	static unsigned char s8[4][16] = {
	13, 2, 8, 4, 6, 15, 11, 1, 10, 9, 3, 14, 5, 0, 12, 7,
	1, 15, 13, 8, 10, 3, 7, 4, 12, 5, 6, 11, 0, 14, 9, 2,
	7, 11, 4, 1, 9, 12, 14, 2, 0, 6, 10, 13, 15, 3, 5, 8,
	2, 1, 14, 7, 4, 10, 8, 13, 15, 12, 9, 0, 3, 5, 6, 11 };

	/* Table - Shift */
	static unsigned char shift[16] = {
	1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1 };

	/* Table - Binary */
	static unsigned char binary[64] = {
	0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 1, 1,
	0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 1, 0, 0, 1, 1, 1,
	1, 0, 0, 0, 1, 0, 0, 1, 1, 0, 1, 0, 1, 0, 1, 1,
	1, 1, 0, 0, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1 };
	/* MAIN PROCESS */
	/* Convert from 64-bit key into 64-byte key */
	for (i = 0; i < 8; i++) {
	key[8*i] = ((j = *(inkey + i)) / 128) % 2;
	key[8*i+1] = (j / 64) % 2;
	key[8*i+2] = (j / 32) % 2;
	key[8*i+3] = (j / 16) % 2;
	key[8*i+4] = (j / 8) % 2;
	key[8*i+5] = (j / 4) % 2;
	key[8*i+6] = (j / 2) % 2;
	key[8*i+7] = j % 2;
	}
	/* Convert from 64-bit data into 64-byte data */
	for (i = 0; i < 8; i++) {
	buffer[8*i] = ((j = *(source + i)) / 128) % 2;
	buffer[8*i+1] = (j / 64) % 2;
	buffer[8*i+2] = (j / 32) % 2;
	buffer[8*i+3] = (j / 16) % 2;
	buffer[8*i+4] = (j / 8) % 2;
	buffer[8*i+5] = (j / 4) % 2;
	buffer[8*i+6] = (j / 2) % 2;
	buffer[8*i+7] = j % 2;
	}
	/* Initial Permutation of Data */
	bufout[ 0] = buffer[57];
	bufout[ 1] = buffer[49];
	bufout[ 2] = buffer[41];
	bufout[ 3] = buffer[33];
	bufout[ 4] = buffer[25];
	bufout[ 5] = buffer[17];
	bufout[ 6] = buffer[ 9];
	bufout[ 7] = buffer[ 1];
	bufout[ 8] = buffer[59];
	bufout[ 9] = buffer[51];
	bufout[10] = buffer[43];
	bufout[11] = buffer[35];
	bufout[12] = buffer[27];
	bufout[13] = buffer[19];
	bufout[14] = buffer[11];
	bufout[15] = buffer[ 3];
	bufout[16] = buffer[61];
	bufout[17] = buffer[53];
	bufout[18] = buffer[45];
	bufout[19] = buffer[37];
	bufout[20] = buffer[29];
	bufout[21] = buffer[21];
	bufout[22] = buffer[13];
	bufout[23] = buffer[ 5];
	bufout[24] = buffer[63];
	bufout[25] = buffer[55];
	bufout[26] = buffer[47];
	bufout[27] = buffer[39];
	bufout[28] = buffer[31];
	bufout[29] = buffer[23];
	bufout[30] = buffer[15];
	bufout[31] = buffer[ 7];
	bufout[32] = buffer[56];
	bufout[33] = buffer[48];
	bufout[34] = buffer[40];
	bufout[35] = buffer[32];
	bufout[36] = buffer[24];
	bufout[37] = buffer[16];
	bufout[38] = buffer[ 8];
	bufout[39] = buffer[ 0];
	bufout[40] = buffer[58];
	bufout[41] = buffer[50];
	bufout[42] = buffer[42];
	bufout[43] = buffer[34];
	bufout[44] = buffer[26];
	bufout[45] = buffer[18];
	bufout[46] = buffer[10];
	bufout[47] = buffer[ 2];
	bufout[48] = buffer[60];
	bufout[49] = buffer[52];
	bufout[50] = buffer[44];
	bufout[51] = buffer[36];
	bufout[52] = buffer[28];
	bufout[53] = buffer[20];
	bufout[54] = buffer[12];
	bufout[55] = buffer[ 4];
	bufout[56] = buffer[62];
	bufout[57] = buffer[54];
	bufout[58] = buffer[46];
	bufout[59] = buffer[38];
	bufout[60] = buffer[30];
	bufout[61] = buffer[22];
	bufout[62] = buffer[14];
	bufout[63] = buffer[ 6];
	/* Initial Permutation of Key */
	kwork[ 0] = key[56];
	kwork[ 1] = key[48];
	kwork[ 2] = key[40];
	kwork[ 3] = key[32];
	kwork[ 4] = key[24];
	kwork[ 5] = key[16];
	kwork[ 6] = key[ 8];
	kwork[ 7] = key[ 0];
	kwork[ 8] = key[57];
	kwork[ 9] = key[49];
	kwork[10] = key[41];
	kwork[11] = key[33];
	kwork[12] = key[25];
	kwork[13] = key[17];
	kwork[14] = key[ 9];
	kwork[15] = key[ 1];
	kwork[16] = key[58];
	kwork[17] = key[50];
	kwork[18] = key[42];
	kwork[19] = key[34];
	kwork[20] = key[26];
	kwork[21] = key[18];
	kwork[22] = key[10];
	kwork[23] = key[ 2];
	kwork[24] = key[59];
	kwork[25] = key[51];
	kwork[26] = key[43];
	kwork[27] = key[35];
	kwork[28] = key[62];
	kwork[29] = key[54];
	kwork[30] = key[46];
	kwork[31] = key[38];
	kwork[32] = key[30];
	kwork[33] = key[22];
	kwork[34] = key[14];
	kwork[35] = key[ 6];
	kwork[36] = key[61];
	kwork[37] = key[53];
	kwork[38] = key[45];
	kwork[39] = key[37];
	kwork[40] = key[29];
	kwork[41] = key[21];
	kwork[42] = key[13];
	kwork[43] = key[ 5];
	kwork[44] = key[60];
	kwork[45] = key[52];
	kwork[46] = key[44];
	kwork[47] = key[36];
	kwork[48] = key[28];
	kwork[49] = key[20];
	kwork[50] = key[12];
	kwork[51] = key[ 4];
	kwork[52] = key[27];
	kwork[53] = key[19];
	kwork[54] = key[11];
	kwork[55] = key[ 3];
	/* 16 Iterations */
	for (iter = 1; iter < 17; iter++) {
	for (i = 0; i < 32; i++)
	{
		buffer[i] = bufout[32+i];
	}
	/* Calculation of F(R, K) */
	/* Permute - E */
	worka[ 0] = buffer[31];
	worka[ 1] = buffer[ 0];
	worka[ 2] = buffer[ 1];
	worka[ 3] = buffer[ 2];
	worka[ 4] = buffer[ 3];
	worka[ 5] = buffer[ 4];
	worka[ 6] = buffer[ 3];
	worka[ 7] = buffer[ 4];
	worka[ 8] = buffer[ 5];
	worka[ 9] = buffer[ 6];
	worka[10] = buffer[ 7];
	worka[11] = buffer[ 8];
	worka[12] = buffer[ 7];
	worka[13] = buffer[ 8];
	worka[14] = buffer[ 9];
	worka[15] = buffer[10];
	worka[16] = buffer[11];
	worka[17] = buffer[12];
	worka[18] = buffer[11];
	worka[19] = buffer[12];
	worka[20] = buffer[13];
	worka[21] = buffer[14];
	worka[22] = buffer[15];
	worka[23] = buffer[16];
	worka[24] = buffer[15];
	worka[25] = buffer[16];
	worka[26] = buffer[17];
	worka[27] = buffer[18];
	worka[28] = buffer[19];
	worka[29] = buffer[20];
	worka[30] = buffer[19];
	worka[31] = buffer[20];
	worka[32] = buffer[21];
	worka[33] = buffer[22];
	worka[34] = buffer[23];
	worka[35] = buffer[24];
	worka[36] = buffer[23];
	worka[37] = buffer[24];
	worka[38] = buffer[25];
	worka[39] = buffer[26];
	worka[40] = buffer[27];
	worka[41] = buffer[28];
	worka[42] = buffer[27];
	worka[43] = buffer[28];
	worka[44] = buffer[29];
	worka[45] = buffer[30];
	worka[46] = buffer[31];
	worka[47] = buffer[ 0];
	/* KS Function Begin */
	if (flg) {
	nbrofshift = shift[iter-1];
	for (i = 0; i < (int) nbrofshift; i++) {
		temp1 = kwork[0];
		temp2 = kwork[28];
		for (j = 0; j < 27; j++) {
		 kwork[j] = kwork[j+1];
		 kwork[j+28] = kwork[j+29];
		}
		kwork[27] = temp1;
		kwork[55] = temp2;
	}
	} else if (iter > 1) {
	nbrofshift = shift[17-iter];
	for (i = 0; i < (int) nbrofshift; i++) {
		temp1 = kwork[27];
		temp2 = kwork[55];
		for (j = 27; j > 0; j--) {
		 kwork[j] = kwork[j-1];
		 kwork[j+28] = kwork[j+27];
		}
		kwork[0] = temp1;
		kwork[28] = temp2;
	}
	}
	/* Permute kwork - PC2 */
	kn[ 0] = kwork[13];
	kn[ 1] = kwork[16];
	kn[ 2] = kwork[10];
	kn[ 3] = kwork[23];
	kn[ 4] = kwork[ 0];
	kn[ 5] = kwork[ 4];
	kn[ 6] = kwork[ 2];
	kn[ 7] = kwork[27];
	kn[ 8] = kwork[14];
	kn[ 9] = kwork[ 5];
	kn[10] = kwork[20];
	kn[11] = kwork[ 9];
	kn[12] = kwork[22];
	kn[13] = kwork[18];
	kn[14] = kwork[11];
	kn[15] = kwork[ 3];
	kn[16] = kwork[25];
	kn[17] = kwork[ 7];
	kn[18] = kwork[15];
	kn[19] = kwork[ 6];
	kn[20] = kwork[26];
	kn[21] = kwork[19];
	kn[22] = kwork[12];
	kn[23] = kwork[ 1];
	kn[24] = kwork[40];
	kn[25] = kwork[51];
	kn[26] = kwork[30];
	kn[27] = kwork[36];
	kn[28] = kwork[46];
	kn[29] = kwork[54];
	kn[30] = kwork[29];
	kn[31] = kwork[39];
	kn[32] = kwork[50];
	kn[33] = kwork[44];
	kn[34] = kwork[32];
	kn[35] = kwork[47];
	kn[36] = kwork[43];
	kn[37] = kwork[48];
	kn[38] = kwork[38];
	kn[39] = kwork[55];
	kn[40] = kwork[33];
	kn[41] = kwork[52];
	kn[42] = kwork[45];
	kn[43] = kwork[41];
	kn[44] = kwork[49];
	kn[45] = kwork[35];
	kn[46] = kwork[28];
	kn[47] = kwork[31];
	/* KS Function End */
	/* worka XOR kn */
	for (i = 0; i < 48; i++)
	worka[i] = worka[i] ^ kn[i];
	/* 8 s-functions */
	valindex = s1[2*worka[ 0]+worka[ 5]]
	[2*(2*(2*worka[ 1]+worka[ 2])+
	worka[ 3])+worka[ 4]];
	valindex = valindex * 4;
	kn[ 0] = binary[0+valindex];
	kn[ 1] = binary[1+valindex];
	kn[ 2] = binary[2+valindex];
	kn[ 3] = binary[3+valindex];
	valindex = s2[2*worka[ 6]+worka[11]]
	[2*(2*(2*worka[ 7]+worka[ 8])+
	worka[ 9])+worka[10]];
	valindex = valindex * 4;
	kn[ 4] = binary[0+valindex];
	kn[ 5] = binary[1+valindex];
	kn[ 6] = binary[2+valindex];
	kn[ 7] = binary[3+valindex];
	valindex = s3[2*worka[12]+worka[17]]
	[2*(2*(2*worka[13]+worka[14])+
	worka[15])+worka[16]];
	valindex = valindex * 4;
	kn[ 8] = binary[0+valindex];
	kn[ 9] = binary[1+valindex];
	kn[10] = binary[2+valindex];
	kn[11] = binary[3+valindex];
	valindex = s4[2*worka[18]+worka[23]]
	[2*(2*(2*worka[19]+worka[20])+
	worka[21])+worka[22]];
	valindex = valindex * 4;
	kn[12] = binary[0+valindex];
	kn[13] = binary[1+valindex];
	kn[14] = binary[2+valindex];
	kn[15] = binary[3+valindex];
	valindex = s5[2*worka[24]+worka[29]]
	[2*(2*(2*worka[25]+worka[26])+
	worka[27])+worka[28]];
	valindex = valindex * 4;
	kn[16] = binary[0+valindex];
	kn[17] = binary[1+valindex];
	kn[18] = binary[2+valindex];
	kn[19] = binary[3+valindex];
	valindex = s6[2*worka[30]+worka[35]]
	[2*(2*(2*worka[31]+worka[32])+
	worka[33])+worka[34]];
	valindex = valindex * 4;
	kn[20] = binary[0+valindex];
	kn[21] = binary[1+valindex];
	kn[22] = binary[2+valindex];
	kn[23] = binary[3+valindex];
	valindex = s7[2*worka[36]+worka[41]]
	[2*(2*(2*worka[37]+worka[38])+
	worka[39])+worka[40]];
	valindex = valindex * 4;
	kn[24] = binary[0+valindex];
	kn[25] = binary[1+valindex];
	kn[26] = binary[2+valindex];
	kn[27] = binary[3+valindex];
	valindex = s8[2*worka[42]+worka[47]]
	[2*(2*(2*worka[43]+worka[44])+
	worka[45])+worka[46]];
	valindex = valindex * 4;
	kn[28] = binary[0+valindex];
	kn[29] = binary[1+valindex];
	kn[30] = binary[2+valindex];
	kn[31] = binary[3+valindex];
	/* Permute - P */
	worka[ 0] = kn[15];
	worka[ 1] = kn[ 6];
	worka[ 2] = kn[19];
	worka[ 3] = kn[20];
	worka[ 4] = kn[28];
	worka[ 5] = kn[11];
	worka[ 6] = kn[27];
	worka[ 7] = kn[16];
	worka[ 8] = kn[ 0];
	worka[ 9] = kn[14];
	worka[10] = kn[22];
	worka[11] = kn[25];
	worka[12] = kn[ 4];
	worka[13] = kn[17];
	worka[14] = kn[30];
	worka[15] = kn[ 9];
	worka[16] = kn[ 1];
	worka[17] = kn[ 7];
	worka[18] = kn[23];
	worka[19] = kn[13];
	worka[20] = kn[31];
	worka[21] = kn[26];
	worka[22] = kn[ 2];
	worka[23] = kn[ 8];
	worka[24] = kn[18];
	worka[25] = kn[12];
	worka[26] = kn[29];
	worka[27] = kn[ 5];
	worka[28] = kn[21];
	worka[29] = kn[10];
	worka[30] = kn[ 3];
	worka[31] = kn[24];
	/* bufout XOR worka */
	for (i = 0; i < 32; i++) {
		bufout[i+32] = bufout[i] ^ worka[i];
		bufout[i] = buffer[i];
	}
	} /* End of Iter */
	/* Prepare Output */
	for (i = 0; i < 32; i++) {
		j = bufout[i];
		bufout[i] = bufout[32+i];
		bufout[32+i] = j;
	}
	/* Inverse Initial Permutation */
	buffer[ 0] = bufout[39];
	buffer[ 1] = bufout[ 7];
	buffer[ 2] = bufout[47];
	buffer[ 3] = bufout[15];
	buffer[ 4] = bufout[55];
	buffer[ 5] = bufout[23];
	buffer[ 6] = bufout[63];
	buffer[ 7] = bufout[31];
	buffer[ 8] = bufout[38];
	buffer[ 9] = bufout[ 6];
	buffer[10] = bufout[46];
	buffer[11] = bufout[14];
	buffer[12] = bufout[54];
	buffer[13] = bufout[22];
	buffer[14] = bufout[62];
	buffer[15] = bufout[30];
	buffer[16] = bufout[37];
	buffer[17] = bufout[ 5];
	buffer[18] = bufout[45];
	buffer[19] = bufout[13];
	buffer[20] = bufout[53];
	buffer[21] = bufout[21];
	buffer[22] = bufout[61];
	buffer[23] = bufout[29];
	buffer[24] = bufout[36];
	buffer[25] = bufout[ 4];
	buffer[26] = bufout[44];
	buffer[27] = bufout[12];
	buffer[28] = bufout[52];
	buffer[29] = bufout[20];
	buffer[30] = bufout[60];
	buffer[31] = bufout[28];
	buffer[32] = bufout[35];
	buffer[33] = bufout[ 3];
	buffer[34] = bufout[43];
	buffer[35] = bufout[11];
	buffer[36] = bufout[51];
	buffer[37] = bufout[19];
	buffer[38] = bufout[59];
	buffer[39] = bufout[27];
	buffer[40] = bufout[34];
	buffer[41] = bufout[ 2];
	buffer[42] = bufout[42];
	buffer[43] = bufout[10];
	buffer[44] = bufout[50];
	buffer[45] = bufout[18];
	buffer[46] = bufout[58];
	buffer[47] = bufout[26];
	buffer[48] = bufout[33];
	buffer[49] = bufout[ 1];
	buffer[50] = bufout[41];
	buffer[51] = bufout[ 9];
	buffer[52] = bufout[49];
	buffer[53] = bufout[17];
	buffer[54] = bufout[57];
	buffer[55] = bufout[25];
	buffer[56] = bufout[32];
	buffer[57] = bufout[ 0];
	buffer[58] = bufout[40];
	buffer[59] = bufout[ 8];
	buffer[60] = bufout[48];
	buffer[61] = bufout[16];
	buffer[62] = bufout[56];
	buffer[63] = bufout[24];
	j = 0;
	for (i = 0; i < 8; i++) {
		*(dest + i) = 0x00;
		for (k = 0; k < 7; k++)
		*(dest + i) = ((*(dest + i)) + buffer[j+k]) * 2;
		*(dest + i) = *(dest + i) + buffer[j+7];
		j += 8;
	}
	return 0;
}

UINT atoh(CString sz)
{
	int len,i;
	UINT dt,tmp;
	char ch;
	
	len = sz.GetLength();
	sz.MakeLower();
	
	for (i=0,dt=0,tmp=0;i<len;i++)
	{
		ch = sz[i];
		if( ch>='0' && ch<='9')
			tmp = ch-'0';
		if( ch>='a' && ch<='f')
			tmp = ch-'a'+10;
		dt = dt*16+tmp;
	}
	
	return dt;
}
CString Des_EN(CString source, CString inkey)
{
	int len_source, len_inkey;
	int i,j,k;
	int x;
	UCHAR s[8],d[8],key[8];
	CString str, dest;

	source.Remove(' ');
	inkey.Remove(' ');

	len_source = source.GetLength()/2;
	source = source.Left(len_source*2);//避免source长度为奇数

	//密钥处理
	len_inkey = inkey.GetLength()/2;
	if (len_inkey>8) len_inkey = 8;//限制inkey最大值不超过8字节
	for (i=0;i<len_inkey;i++)	key[i] = (UCHAR)atoh(inkey.Mid(i*2,2));//转换key
	for(;i<8;i++)	key[i] = 0x00;//不足8字节的部分用0补足
	
	//数据源
	j = len_source/8;
	if ( len_source%8)//数据源分组后有剩余数据
	{
		for (k=8-len_source%8,i=0;i<k;i++)	source +="00";//填充至8个字节		
		j++;//分组长度增加
	}

	for (i=0,dest.Empty();i<j;i++)
	{
		for (x=0;x<8;x++)	s[x] = (UCHAR)atoh(source.Mid(i*16+x*2,2));//将字符串转换成16进制数
		des(s,d,key,1);//flag=1:加密
		for (x=0;x<8;x++)
		{
			str.Format(L"%0.2X",d[x]);
			dest += str;
		}		
	}
	
	return dest;
}

CString Des_DE(CString source, CString inkey)
{
	int len_source, len_inkey;
	int i,j,k;
	int x;
	UCHAR s[8],d[8],key[8];
	CString str, dest;

	source.Remove(' ');
	inkey.Remove(' ');	

	len_source = source.GetLength()/2;
	source = source.Left(len_source*2);//避免source长度为奇数
	
	//密钥处理
	len_inkey = inkey.GetLength()/2;
	if (len_inkey>8) len_inkey = 8;//限制inkey最大值不超过8字节
	for (i=0;i<len_inkey;i++)	key[i] = (UCHAR)atoh(inkey.Mid(i*2,2));//转换key
	for(;i<8;i++)	key[i] = 0x00;//不足8字节的部分用0补足
	
	//数据源
	j = len_source/8;
	if ( len_source%8)//数据源分组后有剩余数据
	{
		for (k=8-len_source%8,i=0;i<k;i++)	source +="00";//填充至8个字节		
		j++;//分组长度增加
	}
	
	for (i=0,dest.Empty();i<j;i++)
	{
		for (x=0;x<8;x++)	s[x] = (UCHAR)atoh(source.Mid(i*16+x*2,2));//将字符串转换成16进制数
		des(s,d,key,0);//flag=0:解密
		for (x=0;x<8;x++)
		{
			str.Format(L"%0.2X",d[x]);
			dest += str;
		}		
	}
	
	return dest;
}

CString TriDes_EN(CString source, CString inkey)
{
	CString dest;

	source.Remove(' ');
	inkey.Remove(' ');

	inkey = inkey.Left((inkey.GetLength()/2)*2);//避免inkey长度为奇数
	for (;inkey.GetLength()<32;)	inkey+="00";//增加inkey的长度到16字节
	inkey = inkey.Left(32);//防止inkey长度超过16字节

	dest = Des_EN(source,inkey.Left(16));
	dest = Des_DE(dest,inkey.Right(16));
	dest = Des_EN(dest,inkey.Left(16));

	return dest;
}

CString TriDes_DE(CString source, CString inkey)
{
	CString dest;
	
	source.Remove(' ');
	inkey.Remove(' ');

	inkey = inkey.Left((inkey.GetLength()/2)*2);//避免inkey长度为奇数
	for (;inkey.GetLength()<32;)	inkey+="00";//增加inkey的长度到16字节
	inkey = inkey.Left(32);//防止inkey长度超过16字节
	
	dest = Des_DE(source,inkey.Left(16));
	dest = Des_EN(dest,inkey.Right(16));
	dest = Des_DE(dest,inkey.Left(16));
	
	return dest;
}
