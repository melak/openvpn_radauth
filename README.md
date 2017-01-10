# openvpn_radauth

A small helper application that makes it possible to authenticate
[OpenVPN](http://openvpn.net/) users against a [RADIUS](http://en.wikipedia.org/wiki/RADIUS)
server.

**This is _not_ an OpenVPN _plugin_**. This is an _auth-user-pass-verify **script**_.
A (the?) RADIUS **_plugin_** is available at [NonGNU](http://savannah.nongnu.org/projects/radiusplugin).
The plugin is likely to be more powerful, it is also _much_ larger. This script is
targeted toward scenarios where storage space is at a premium. Package feed for
[OpenWRT](https://openwrt.org/) Attitude Adjustment (and eventually newer versions)
is available at [melak/openwrt-feeds](http://github.com/melak/openwrt-feeds).

## &#x1f435; Configuration

Include

```
auth-user-pass-verify /path/to/openvpn_radauth via-env
```

in openvpn.conf (via-file works too; see the OpenVPN documentation). Set your RADIUS
server(s) in _radius.conf_ (in _/etc/openvpn/_ by default, a different path may be
set at compile-time):

```
auth	192.168.1.10	mekmitasdigoat
```

openvpn_radauth uses Juniper libradius snatched from FreeBSD, the full radius.conf manual
can be found in the [FreeBSD SVN](http://svnweb.freebsd.org/base/stable/11/lib/libradius/radius.conf.5?view=markup)
repository. Only the _auth_ service is used.

There is no way at present to use different RADIUS servers with separate OpenVPN instances
(save for having a dedicated openvpn_radauth for each and every one of them). Should there be
demand for such a feature, it is possible to implement something to that effect.

# Building

GNU Make is needed for building.

libradius has been slightly modified so that it is able to use either [OpenSSL](https://www.openssl.org/),
[LibreSSL](https://www.openssl.org/), [PolarSSL](https://tls.mbed.org/) or
[mbed TLS](https://tls.mbed.org/), just like OpenVPN is in later versions. Issue

```
make WITH_SSL_IMPL=openssl
```

for the OpenSSL version,

```
make WITH_SSL_IMPL=polarssl
```

for the PolarSSL version, and

```
make WITH_SSL_IMPL=mbedtls
```

for the mbed TLS one. LibreSSL is selected with `WITH_SSL_IMPL=openssl`. If you have only LibreSSL
installed, it will automatically be picked up. If you have both OpenSSL and LibreSSL installed (a
scenario that might be common on FreeBSD, possibly elsewhere), you are expected to set `CFLAGS`
and `LDFLAGS` so that the compiler/linker will pick LibreSSL up.

Not specifying an SSL implementation to use compiles the bundled RSA MD5 library (also snatched from
FreeBSD) and bars libradius from creating/verifying authentic RADIUS messages. This should not,
in general, affect the operation of openvpn_radauth. The implications of not using authentic RADIUS
messages (or the general security of RADIUS for that matter) are outside the scope of this document.

Cross-compilation using the OpenWRT SDK is possible. You will likely have to tweak Makefile.rules.

Release tarballs for the moment live at <http://www.wormhole.hu/~ice/openvpn_radauth/>.
