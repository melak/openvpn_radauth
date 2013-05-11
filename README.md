# openvpn_radauth

A small helper application that makes it possible to authenticate
[OpenVPN](http://openvpn.net/) against a [RADIUS](http://en.wikipedia.org/wiki/RADIUS)
server.

**This is _not an OpenVPN plugin_**. This is an _auth-user-pass-verify **script**_.
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
server(s) in _radius.conf_ (in _/etc/openvpn/_ by default, path settable at compile-time):

```
auth	192.168.1.10	mekmitasdigoat
```

openvpn_radauth uses Juniper libradius snatched from FreeBSD, the full radius.conf manual
can be found in [FreeBSD SVN](http://svnweb.freebsd.org/base/stable/9/lib/libradius/radius.conf.5?view=markup).
Only the _auth_ service is used.

There is no way at present to use different RADIUS servers with separate OpenVPN instances
(save for having a dedicated openvpn_radauth for each and every one of them). Should there be
demand for such a feature, it is possible to implement something to that effect.

# Building

libradius has been slightly modified so that it is able to use either OpenSSL or PolarSSL,
just like OpenVPN is in later versions. Issue

```
make WITH_SSL_IMPL=openssl
```

for the OpenSSL version (default) or

```
make WITH_SSL_IMPL=polarssl
```

for the PolarSSL one.

Cross-compilation using the OpenWRT SDK is possible. You will likely have to tweak Makefile.rules.

Release tarballs for the moment live at <http://www.wormhole.hu/~ice/openvpn_radauth/>.
