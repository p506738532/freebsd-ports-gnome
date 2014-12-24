# Created by: Gustau Perez <gustau.perez@gmail.com>
# $FreeBSD$

PORTNAME=	polari
PORTVERSION=	3.15.3
CATEGORIES=	irc gnome
MASTER_SITES=	GNOME
DIST_SUBDIR=	gnome3

MAINTAINER=	gnome@FreeBSD.org
COMMENT=	Internet Relay Chat (IRC) client designed for GNOME 3

LIB_DEPENDS=	libtelepathy-glib.so:${PORTSDIR}/net-im/telepathy-glib \
		libgjs.so:${PORTSDIR}/lang/gjs

USES=		gettext gmake libtool pathfix pkgconfig tar:xz
USE_GNOME=	gnomedesktop3 gnomeprefix gtk30 intlhack \
		introspection:build
USE_LDCONFIG=	yes
GNU_CONFIGURE=	yes
INSTALLS_ICONS=	yes
CPPFLAGS+=	-I${LOCALBASE}/include
LDFLAGS+=	-L${LOCALBASE}/lib
INSTALL_TARGET=	install-strip

GLIB_SCHEMAS=	org.gnome.polari.gschema.xml

.include <bsd.port.mk>
