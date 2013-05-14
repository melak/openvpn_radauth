/*
 * Copyright (c) 2007, 2009-2013 Tamas Tevesz <ice@extreme.hu>
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <syslog.h>

#include "credentials.h"

#ifndef HAVE_STRL
#include "compat/strl.h"
#endif

int getcreds( int argc, char **argv, char *username, char *password, size_t buflen ) {

	int res;
	char *uenv, *penv;

	res = -1;
	uenv = penv = NULL;

	memset( username, 0, buflen );
	memset( password, 0, buflen );

	uenv = getenv( "username" );
	penv = getenv( "password" );

	if( uenv != NULL && penv != NULL ) {

		if( strlcpy( username, uenv, buflen ) >= buflen ||
		    strlcpy( password, penv, buflen ) >= buflen ) {
			syslog( LOG_ERR, "Error: username/password buffer too small" );
			goto out;
		}

		if( *username == '\0' || *password == '\0' ) {
			syslog( LOG_ERR, "Got zero-length username or password" );
			goto out;
		}

		res = 0;

	} else if( argc > 1 ) {

		FILE *fp;
		char *bufp;
		size_t ubuf_eol, pbuf_eol;

		fp = fopen( argv[ 1 ], "r" );
		if( fp == NULL ) {
			syslog( LOG_ERR, "Unable to open password file \"%s\"", argv[ 1 ] );
			goto out;
		}

		bufp = fgets( username, buflen, fp );
		if( bufp == NULL || *bufp == '\r' || *bufp == '\n' ) {
			syslog( LOG_ERR, "Unable to read username from \"%s\"", argv[ 1 ] );
			fclose( fp );
			goto out;
		}

		bufp = fgets( password, buflen, fp );
		if( bufp == NULL ) {
			syslog( LOG_ERR, "Unable to read password from \"%s\"", argv[ 1 ] );
			fclose( fp );
			goto out;
		}

		fclose( fp );

		if( *username == '\r' || *username == '\n' ||
		    *password == '\r' || *username == '\n' ) {
			syslog( LOG_ERR, "Got zero-length username or password" );
			goto out;
		}

		ubuf_eol = strcspn( username, "\r\n" );
		pbuf_eol = strcspn( password, "\r\n" );
		if( username[ ubuf_eol ] == '\0' || password[ pbuf_eol ] == '\0' ) {
			syslog( LOG_ERR, "Error: username/password buffer too small" );
			goto out;
		}

		username[ ubuf_eol ] = '\0';
		password[ pbuf_eol ] = '\0';
		res = 0;

	} else {
		syslog( LOG_ERR, "Unable to get credentials from OpenVPN" );
	}

out:
	return res;
}

const char *peer_address( void ) {

	static char *ipenv = NULL;

	if( ipenv != NULL &&
	    *ipenv != '\0' ) {
		goto out;
	}

	ipenv = getenv( "untrusted_ip" );

	if( ipenv != NULL &&
	    *ipenv == '\0' ) {
		ipenv = NULL;
	}

out:
	return ipenv;
}
