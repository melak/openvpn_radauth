/*
 * Copyright (c) 2010 Tamas Tevesz <ice@extreme.hu>
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
#ifdef HAVE_LIBBSD
#include <bsd/string.h>
#endif
#include <syslog.h>

#include <radlib.h>

#include "credentials.h"

#ifndef OPENVPN_RADIUS_CONF
#define OPENVPN_RADIUS_CONF		"/etc/openvpn/radius.conf"
#endif

#define OPENVPN_AUTH_SUCC		0
#define OPENVPN_AUTH_FAIL		1
#define OPENVPN_USERPASS_LEN		128

extern char *__progname;
static int check_radius( const char *, const char * );

int main( int argc, char **argv )
{
	char username[ OPENVPN_USERPASS_LEN ], password[ OPENVPN_USERPASS_LEN ];
	int res;

	res = OPENVPN_AUTH_FAIL;

	if( argc >= 2 &&
	    (strcmp(argv[1], "-h") == 0 ||
	     strcmp(argv[1], "-help") == 0 ||
	     strcmp(argv[1], "--help") == 0)
	) {
		fprintf(stderr, "%s: RADIUS authentication script for OpenVPN\n", __progname);
		fprintf(stderr, "See https://github.com/melak/openvpn_radauth for instructions.\n");

		goto out;
	}

	openlog( __progname, LOG_PID|LOG_NDELAY, LOG_AUTH );

	if( getcreds( argc, argv, username, password, OPENVPN_USERPASS_LEN ) < 0 ) {
#ifdef DEBUG
		syslog( LOG_ERR, "Unable to get supplied credentials" );
#endif
		goto out;
	}

	if( check_radius( username, password ) == 1 ) {
		syslog( LOG_INFO, "Accepted password for %s from %s",
			username, peer_address() ? peer_address() : "UNKNOWN" );
		res = OPENVPN_AUTH_SUCC;
	} else {
		syslog( LOG_INFO, "Bad password for %s from %s,",
			username, peer_address() ? peer_address() : "UNKNOWN" );
	}

out:
	explicit_bzero( username, OPENVPN_USERPASS_LEN );
	explicit_bzero( password, OPENVPN_USERPASS_LEN );

	closelog();
	return res;
}

static int check_radius( const char *username, const char *password )
{
	struct rad_handle *rh;
	int res, rad_res;

	rad_res = res = 0;
	rh = NULL;

	rh = rad_auth_open();
	if( rh == NULL ) {
		syslog( LOG_ERR, "Unable to initialise libradius" );
		goto out;
	}

	if( rad_config( rh, OPENVPN_RADIUS_CONF ) < 0 ||
	    rad_create_request( rh, RAD_ACCESS_REQUEST ) < 0 ||
	    rad_put_string( rh, RAD_USER_NAME, username ) < 0 ||
	    rad_put_string( rh, RAD_USER_PASSWORD, password ) < 0 ) {
		syslog( LOG_ERR, "Unable to create RADIUS authn request: %s",
			rad_strerror( rh ) );
		goto out;
	}

	if( peer_address() != NULL &&
	    rad_put_string( rh, RAD_CALLING_STATION_ID, peer_address() ) < 0 ) {
		syslog( LOG_ERR, "Unable to add Calling-Station-Id to RADIUS request: %s",
			rad_strerror( rh ) );
		goto out;
	}

	if( rad_put_message_authentic( rh ) < 0 ) {
		syslog( LOG_ERR, "Unable to add Message-Authenticator to RADIUS request: %s",
			rad_strerror( rh ) );
		goto out;
	}

	rad_res = rad_send_request( rh );
	if( rad_res < 0 ) {
		syslog( LOG_ERR, "RADIUS error: %s, result: %d", rad_strerror( rh ), rad_res );
	} else if( rad_res == RAD_ACCESS_ACCEPT ) {
		res = 1;
	}

#ifdef DEBUG
	syslog( LOG_INFO, "RADIUS %sed %s", res == 1 ? "accept" : "reject", username );
#endif

out:
	if( rh ) {
		rad_close( rh );
	}

	return res;
}
