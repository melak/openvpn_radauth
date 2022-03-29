#include <stdio.h>
#include <string.h>
#include "../libradius/picohash.h"

int test(int testno, char *data, int datalen, char *exp)
{
	picohash_ctx_t ctx;
	char digest[PICOHASH_MD5_DIGEST_LENGTH];
	char hexdigest[2 * PICOHASH_MD5_DIGEST_LENGTH + 1];

	picohash_init_md5(&ctx);
	picohash_update(&ctx, data, datalen);
	picohash_final(&ctx, digest);

	memset(hexdigest, 0, sizeof(hexdigest));
	char *p = hexdigest;
	for(int i = 0; i < PICOHASH_MD5_DIGEST_LENGTH; i++)
	{
		p += sprintf(p, "%02x", digest[i] & 0xff);
	}

	if(strcmp(hexdigest, exp) == 0)
	{
		return 0;
	}
	else
	{
		printf("Mismatch: %d: expected: %s, got: %s\n", testno, exp, hexdigest);
	}

	return 1;
}

int main(int argc, char **argv)
{
	(void)argc;
	(void)argv;
	int rc;

	rc = 0;
	rc |= test(1, "", 0, "d41d8cd98f00b204e9800998ecf8427e");
	rc |= test(2, "The quick brown fox jumps over the lazy dog", 43, "9e107d9d372bb6826bd81d3542a419d6");
	rc |= test(3, "Test vector from febooti.com", 28, "500ab6613c6db7fbd30c62f5ff573d0f");
	rc |= test(4, "\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd", 10, "22c0abf4637191d92a57ec19e2b0fc01");
	rc |= test(5, "\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0a"
                      "\x0b\x0c\x0d\x0e\x0f\x10\x11\x12\x13\x14"
                      "\x15\x16\x17\x18\x19",
		   25, "8a5690c97e2b1e44de8a68372e83768e");
	rc |= test(6, "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
		      "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
		      "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
		      "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
		      "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
		      "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
		      "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
		      "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x0a",
		   80, "c4cc5a3a924c8c983d6156de8dcea9e4");

	return rc;
}

