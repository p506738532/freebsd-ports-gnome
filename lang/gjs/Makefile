# Created by: Pawel Worach <pawel.worach@gmail.com>
# $FreeBSD: head/lang/gjs/Makefile 408881 2016-02-14 18:57:53Z kwm $

PORTNAME=	gjs
PORTVERSION=	1.45.3
CATEGORIES=	lang gnome
MASTER_SITES=	GNOME
DIST_SUBDIR=	gnome3

MAINTAINER=	gnome@FreeBSD.org
COMMENT=	GNOME Javascript binding

LIB_DEPENDS=	libdbus-glib-1.so:devel/dbus-glib \
		libmozjs-24.so:lang/spidermonkey24

USES=		compiler:c++11-lib gettext gmake gnome libtool \
		localbase pathfix pkgconfig python:build tar:xz
USE_GNOME=	cairo glib20 gnomeprefix introspection:build \
		intlhack
GNU_CONFIGURE=	yes
USE_LDCONFIG=	yes
INSTALL_TARGET=	install-strip

post-install:
	@${RM} -rf ${STAGEDIR}${PREFIX}/libexec/gjs

.include <bsd.port.mk>
