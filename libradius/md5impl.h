/*
 * Copyright (c) 2013 Tamas Tevesz <ice@extreme.hu>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#ifdef WITH_SSL

#include <openssl/hmac.h>
#include <openssl/md5.h>

#define __md5_digest_length			MD5_DIGEST_LENGTH
#define __md5_ctx				MD5_CTX

#define __md5_init(ctx)				MD5_Init(ctx)
#define __md5_update(ctx, data, length)		MD5_Update(ctx, data, length)
#define __md5_finish(ctx, result)		MD5_Final(result, ctx)

#define __has_md5_hmac__			1

#define __md5_hmac_ctx				HMAC_CTX

#define __md5_hmac_init(ctx, key, length)	do {							\
							HMAC_CTX_init(ctx);				\
							HMAC_Init(ctx, key, length, EVP_md5());		\
						} while(0)
#define __md5_hmac_update(ctx, data, length)	HMAC_Update(ctx, data, length)
#define __md5_hmac_finish(ctx, result, length)	do {							\
							HMAC_Final(ctx, result, length);		\
							HMAC_CTX_cleanup(ctx);				\
							HMAC_cleanup(ctx);				\
						} while(0)

#elif defined(WITH_POLARSSL)

#include <polarssl/md5.h>

#define __md5_digest_length			16
#define __md5_ctx				md5_context

#define __md5_init(ctx)				md5_starts(ctx)
#define __md5_update(ctx, data, length)		md5_update(ctx, data, length)
#define __md5_finish(ctx, result)		md5_finish(ctx, result)

#define __has_md5_hmac__			1

#define __md5_hmac_ctx				md5_context

#define __md5_hmac_init(ctx, key, length)	md5_hmac_starts(ctx, key, length)
#define __md5_hmac_update(ctx, data, length)	md5_hmac_update(ctx, data, length)
#define __md5_hmac_finish(ctx, result, length)	do {							\
							*(u_int *)(length) = __md5_digest_length;	\
							md5_hmac_finish(ctx, result);			\
							md5_hmac_reset(ctx);				\
						} while(0)

#elif defined(WITH_MBEDTLS)

#include <mbedtls/md.h>
#include <mbedtls/md5.h>

#define __md5_digest_length			16
#define __md5_ctx				mbedtls_md5_context

#define __md5_init(ctx)				mbedtls_md5_starts(ctx)
#define __md5_update(ctx, data, length)		mbedtls_md5_update(ctx, data, length)
#define __md5_finish(ctx, result)		mbedtls_md5_finish(ctx, result)

#define __has_md5_hmac__			1

#define __md5_hmac_ctx				mbedtls_md5_context

#define __md5_hmac_init(ctx, key, length)	mbedtls_md_hmac_starts(ctx, key, length)
#define __md5_hmac_update(ctx, data, length)	mbedtls_md_hmac_update(ctx, data, length)
#define __md5_hmac_finish(ctx, result, length)	do {							\
							*(u_int *)(length) = __md5_digest_length;	\
							mbedtls_md_hmac_finish(ctx, result);		\
							mbedtls_md_hmac_reset(ctx);			\
						} while(0)

#else /* libmd */

#include <md5.h>

#define __md5_digest_length			16

#define __md5_ctx				MD5_CTX

#define __md5_init(ctx)				MD5Init(ctx)
#define __md5_update(ctx, data, length)		MD5Update(ctx, data, length)
#define __md5_finish(ctx, result)		MD5Final(result, ctx)

#ifdef __has_md5_hmac__
#undef __has_md5_hmac__
#endif

#endif

