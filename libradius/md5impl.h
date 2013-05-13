
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

#define __md5_hmac_init(ctx, key, length)	do {						\
							HMAC_CTX_init(ctx);			\
							HMAC_Init(ctx, key, length, EVP_md5());	\
						} while(0)
#define __md5_hmac_update(ctx, data, length)	HMAC_Update(ctx, data, length)
#define __md5_hmac_finish(ctx, result, length)	do {						\
							HMAC_Final(ctx, result, length);	\
							HMAC_CTX_cleanup(ctx);			\
							HMAC_cleanup(ctx);			\
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
#define __md5_hmac_finish(ctx, result, length)	do {						\
							(void *)length;				\
							md5_hmac_finish(ctx, result);		\
							md5_hmac_reset(ctx);			\
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

